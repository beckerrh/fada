#include  "Alat/filescanner.h"
#include  "Alat/newtondata.h"
#include  "Alat/newtonlavrentiev.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

NewtonLavrentiev::~NewtonLavrentiev()
{}

NewtonLavrentiev::NewtonLavrentiev(std::string type) : NonlinearSolver(), _type(type)
{
  if(type == "test")
  {
    _LINEARIZATION_TEST = true;
    _USE_LINEARIZATION = true;
  }
  else if(type == "lin")
  {
    _LINEARIZATION_TEST = false;
    _USE_LINEARIZATION = true;
  }
  else if(type == "mat")
  {
    _LINEARIZATION_TEST = false;
    _USE_LINEARIZATION = false;
  }
  else
  {
    _error_string("NewtonLavrentiev", "unknown type \"" + type + "\"");
  }
}

NewtonLavrentiev::NewtonLavrentiev( const NewtonLavrentiev& newtonsimple) : NonlinearSolver(newtonsimple)
{
  ( *this ).operator=(newtonsimple);
}

NewtonLavrentiev& NewtonLavrentiev::operator=( const NewtonLavrentiev& newtonsimple)
{
  NonlinearSolver::operator=(newtonsimple);
  assert(0);
  return *this;
}

std::string NewtonLavrentiev::getName() const
{
  return "NewtonLavrentiev";
}

NewtonLavrentiev* NewtonLavrentiev::clone() const
{
  return new NewtonLavrentiev(*this);
}

/*--------------------------------------------------------------------------*/
int NewtonLavrentiev::getNVectors() const
{
  return 3;
}

/*--------------------------------------------------------------------------*/
void NewtonLavrentiev::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  // std::cerr << "NewtonLavrentiev::basicInit() blockname="<<blockname<<"\n";
  NonlinearSolver::basicInit(parameterfile, blockname);
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("firstregularization", &_firstregularization, 1.0);
  dataformathandler.insert("maxregularization", &_maxregularization, 1.e6);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, blockname, 0);
}

/*--------------------------------------------------------------------------*/
bool NewtonLavrentiev::_changeRegularization(int iteration, double& regularization, double modelfactor)
{
  double rfactor = fabs(modelfactor-1.0);
  // std::cerr << "_changeRegularization() newtondata.regularization="<<newtondata.regularization<<" modelfactor="<<modelfactor<<" rfactor="<<rfactor<<"\n";
  bool parameterchanged = true;
  if( rfactor < 0.001)
  {
    regularization *= 0.1;
  }
  else if( rfactor < 0.1)
  {
    regularization *= 0.3;
  }
  else if( rfactor < 0.2)
  {
    regularization *= 0.4;
  }
  else if( rfactor < 0.3)
  {
    regularization *= 0.8;
  }
  else if( rfactor > 0.9)
  {
    regularization *= 100.0;
  }
  else if( rfactor > 0.8)
  {
    regularization *= 10.0;
  }
  else if( rfactor > 0.6)
  {
    regularization *= 5.0;
  }
  else if( rfactor > 0.5)
  {
    regularization *= 3.0;
  }
  else if( rfactor > 0.4)
  {
    regularization *= 2.0;
  }
  else if( rfactor > 0.3)
  {
    regularization *= 1.5;
  }
  else
  {
    parameterchanged = false;
  }
  return parameterchanged;
}

/*--------------------------------------------------------------------------*/
void NewtonLavrentiev::solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  const Alat::IterationInfo& info = getIterationInfo();
  newtondata.use_regularization=true;

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& du = getMemory(1);
  Alat::GhostVector& h = getMemory(2);

  newtondata.regularization = _firstregularization;
  bool reached = false, parameterchanged;
  double kappa, wnorm, respred2, rescor, rrpredicted;
  info._printbufff = " dxnorm irelax [mf param] matrix (lintol nlin)\n";
  int irelax = 0;
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusTooManyIterations;
      return;
    }
    newtondata.residual_old = newtondata.residual;
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
    {
      return;
    }
    if(_USE_LINEARIZATION)
    {
      getVisitor()->vectorZero(h);
      getVisitor()->vectorAdd(h, -1.0, r);
    }
    newtondata.iteration=iteration;
    if(_checkIteration( newtondata, info) )
    {
      return;
    }
    if(iteration)
    {
      if(_LINEARIZATION_TEST or not _USE_LINEARIZATION)
      {
        rescor = fabs( getVisitor()->scalarProduct(r, du) );
      }
      else
      {
        rescor = getVisitor()->vectorNorm(r);
      }
      if(not _USE_LINEARIZATION or _LINEARIZATION_TEST)
      {
        wnorm = getVisitor()->computeNormSquared(newtondata.getResidualStatus(), u, du);
      }
      if(_LINEARIZATION_TEST)
      {
        // std::cerr << "respred2="<<respred2 << " =?= newtondata.regularization*wnorm=" << newtondata.regularization*wnorm << "\n";
        if( fabs(respred2-newtondata.regularization*wnorm) > fmax(1e-4*respred2, 1e-8) )
        {
          std::cerr << "fabs(respred2-newtondata.regularization*wnorm)="<<fabs(respred2-newtondata.regularization*wnorm) << "\n";
          assert(0);
        }
      }
      if(_USE_LINEARIZATION)
      {
        // std::cerr << "_omega="<<_omega << "rescor="<<rescor<<" respred2="<<respred2<< " ?" << respred2/rescor<< "\n";
        rrpredicted = respred2;
      }
      else
      {
        rrpredicted = newtondata.regularization*wnorm;
      }
      newtondata.modelfactor = rrpredicted/rescor;

      // std::cerr << "newtondata.modelfactor="<<newtondata.modelfactor<<" newtondata.regularization="<<newtondata.regularization<<" rrpredicted="<<rrpredicted<<" rescor="<<rescor<<"\n";
      parameterchanged = _changeRegularization(iteration, newtondata.regularization, newtondata.modelfactor);

      if(newtondata.regularization > 1e10)
      {
        std::cerr << "newtondata.regularization wnorm " << newtondata.regularization << " " << wnorm << "\n";
        std::cerr << "newtondata.modelfactor rrpredicted " << newtondata.modelfactor << " " << rrpredicted << "\n";
        assert(0);
      }
    }
    // std::cerr  <<"_newmatrix="<<_newmatrix<<" parameterchanged="<<parameterchanged<<" newtondata.regularization"<<newtondata.regularization<<"\n";
    if(_newmatrix or parameterchanged)
    {
      getVisitor()->setLavrentievParameter(newtondata.regularization);
      getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
    }
    getVisitor()->setLinearTolerance(_lintol, info.getGlobalTol(), B);
    _nliniter = getVisitor()->solve(newtondata.getLinearSolverStatus(), B, A, du, r);
    newtondata.getNLinearIteration() += _nliniter;
    _dxnorm =  getVisitor()->vectorNorm(du);
    if( ( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusMaxIter )||( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusDiverged ) )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusLinearNotOk;
      return;
    }
    if(_USE_LINEARIZATION)
    {
      AlatEnums::residualstatus status;
      getVisitor()->computeLinearization(status, h, u, du);
      if(_LINEARIZATION_TEST)
      {
        respred2 =  fabs( getVisitor()->scalarProduct(h, du) );
      }
      else
      {
        respred2 =  getVisitor()->vectorNorm(h);
      }
    }
    getVisitor()->vectorAdd(u, 1.0, du);
  }

  assert(0);
}
