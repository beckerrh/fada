#include  "Alat/filescanner.h"
#include  "Alat/newtondata.h"
#include  "Alat/newtonlavrentievstab.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

NewtonLavrentievStab::~NewtonLavrentievStab()
{}

NewtonLavrentievStab::NewtonLavrentievStab() : NewtonLavrentiev()
{}

NewtonLavrentievStab::NewtonLavrentievStab( const NewtonLavrentievStab& newtonsimple) : NewtonLavrentiev(newtonsimple)
{
  ( *this ).operator=(newtonsimple);
}

NewtonLavrentievStab& NewtonLavrentievStab::operator=( const NewtonLavrentievStab& newtonsimple)
{
  NewtonLavrentiev::operator=(newtonsimple);
  assert(0);
  return *this;
}

std::string NewtonLavrentievStab::getName() const
{
  return "NewtonLavrentievStab";
}

NewtonLavrentievStab* NewtonLavrentievStab::clone() const
{
  return new NewtonLavrentievStab(*this);
}

/*--------------------------------------------------------------------------*/
int NewtonLavrentievStab::getNVectors() const
{
  return 4;
}

/*--------------------------------------------------------------------------*/
bool NewtonLavrentievStab::_changeRegularization(int iteration, double& regularization, double modelfactor)
{
  bool parameterchanged = true;
  double rfactor = fabs(modelfactor-1.0);
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
    regularization *= 20.0;
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
void NewtonLavrentievStab::solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  const Alat::IterationInfo& info = getIterationInfo();
  newtondata.use_linesearch = true;
  newtondata.use_regularization = true;

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& du = getMemory(1);
  Alat::GhostVector& h = getMemory(2);
  Alat::GhostVector& uold = getMemory(3);

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
    getVisitor()->vectorZero(h);
    getVisitor()->vectorAdd(h, -1.0, r);

    rescor = getVisitor()->vectorNorm(r);

    if(iteration)
    {
      _lineSearchMonotonicity(newtondata, u, du, r, f);
    }
    if(newtondata.getNewtonStatus() == AlatEnums::NewtonStatusMaxLineSearchAttained)
    {
      getVisitor()->vectorEqual(u, uold);
      _error_string("solve", "maximum linesearch attained");
    }
    newtondata.iteration = iteration;
    if( _checkIteration( newtondata, info) )
    {
      return;
    }
    if(iteration)
    {
      // rescor = getVisitor()->vectorNorm(r);
      // std::cerr << "_omega="<<_omega << "rescor="<<rescor<<" respred2="<<respred2<< " ?" << respred2/rescor<< "\n";
      rrpredicted = respred2;
      newtondata.modelfactor = rrpredicted/rescor;

      // std::cerr << "newtondata.modelfactor="<<newtondata.modelfactor<<" newtondata.regularization="<<newtondata.regularization<<" rrpredicted="<<rrpredicted<<" rescor="<<rescor<<"\n";
      if(  newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusConverged )
      {
        parameterchanged = _changeRegularization(iteration, newtondata.regularization, newtondata.modelfactor);
      }
      if (newtondata.regularization>_maxregularization)
      {
        std::cerr << "NewtonLavrentievStab::solve() maxregularization attained ("<<_maxregularization<<")\n";
        newtondata.getNewtonStatus() = AlatEnums::NewtonStatusDiverged;
        return;
      }
    }
    // std::cerr  <<"_newmatrix="<<_newmatrix<<" parameterchanged="<<parameterchanged<<" newtondata.regularization"<<newtondata.regularization<<"\n";
    if(_newmatrix or parameterchanged)
    {
      getVisitor()->setLavrentievParameter(newtondata.regularization);
      getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
    }
    // if(iteration)
    // {
    //   getVisitor()->integrateTimeRhs(r, u, du, 0.5);
    // }
    getVisitor()->setLinearTolerance(_lintol, info.getGlobalTol(), B);

    newtondata.getLinearSolverStatus() = AlatEnums::IterationStatusRunning;
    while(newtondata.getLinearSolverStatus() != AlatEnums::IterationStatusConverged)
    {
      _nliniter = getVisitor()->solve(newtondata.getLinearSolverStatus(), B, A, du, r);
      if(newtondata.getLinearSolverStatus() != AlatEnums::IterationStatusConverged)
      {
        newtondata.regularization *= 4.0;
        getVisitor()->setLavrentievParameter(newtondata.regularization);
        getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
        std::cerr << "INCREASING DUE TO LINEAR SOLVER status"<< AlatEnums::iterationStatusToString( newtondata.getLinearSolverStatus() ) << " regularization="<<newtondata.regularization<<"\n";
      }
      if(newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusMaxIter)
      {
        break;
      }
    }


    newtondata.getNLinearIteration() += _nliniter;
    _dxnorm =  getVisitor()->vectorNorm(du);
    // if( ( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusMaxIter )||( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusDiverged ) )
    if( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusDiverged )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusLinearNotOk;
      return;
    }
    AlatEnums::residualstatus status;
    getVisitor()->computeLinearization(status, h, u, du);
    respred2 =  getVisitor()->vectorNorm(h);

    getVisitor()->vectorEqual(uold, u);
    getVisitor()->vectorAdd(u, 1.0, du);
  }

  assert(0);
}
