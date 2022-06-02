#include  "Alat/filescanner.h"
#include  "Alat/newtondata.h"
#include  "Alat/newtonrb.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

NewtonRb::~NewtonRb()
{}

NewtonRb::NewtonRb(std::string linesearchtype) : NonlinearSolver(), _linesearchtype(linesearchtype)
{
  assert( ( linesearchtype == "monotonicty" )or(linesearchtype == "armijo") or(linesearchtype == "wolfe") );
}

NewtonRb::NewtonRb( const NewtonRb& newtonsimple) : NonlinearSolver(newtonsimple)
{
  ( *this ).operator=(newtonsimple);
}

NewtonRb& NewtonRb::operator=( const NewtonRb& newtonsimple)
{
  NonlinearSolver::operator=(newtonsimple);
  assert(0);
  return *this;
}

std::string NewtonRb::getName() const
{
  return "NewtonRb";
}

NewtonRb* NewtonRb::clone() const
{
  return new NewtonRb(*this);
}

/*--------------------------------------------------------------------------*/
int NewtonRb::getNVectors() const
{
  std::cerr << "NewtonRb::basicInit() _linesearchtype="<<_linesearchtype<<"\n";
  if(_linesearchtype == "monotonicty")
  {
    return 2;
  }
  else
  {
    return 3;
  }
}

/*--------------------------------------------------------------------------*/
void NewtonRb::solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  const Alat::IterationInfo& info = getIterationInfo();
  newtondata.use_linesearch=true;

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& du = getMemory(1);

  bool reached = 0;
  double kappa;
  int irelax = 0;
  info._printbufff = " dxnorm irelax matrix (lintol nlin)\n";
  // std::cerr << "FFFF NewtonRb::solve() \n";
  // getVisitor()->vectorWrite(std::cerr, f);
  // std::cerr << "\n";
  for(int iteration = 0; !reached; iteration++)
  {
    // std::cerr << "NewtonRb::solve() iteration ="<<iteration<< " u =" << getVisitor()->vectorNorm(u)<< " f =" << getVisitor()->vectorNorm(f) << "\n";

    if( iteration == info.getMaxiter() )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusTooManyIterations;
      return;
    }
    newtondata.residual_old = newtondata.residual;
    // std::cerr << "UUUU NewtonRb::solve() \n";
    // getVisitor()->vectorWrite(std::cerr, u);
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    // std::cerr << "RRRR NewtonRb::solve() r= \n";
    // getVisitor()->vectorWrite(std::cerr, r);
    if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
    {
      return;
    }
    if(iteration)
    {
      if(_linesearchtype == "monotonicty")
      {
        _lineSearchMonotonicity(newtondata, u, du, r, f);
      }
      else if(_linesearchtype == "armijo")
      {
        _lineSearchArmijo(newtondata, u, du, r, f, kappa);
      }
      else
      {
        _lineSearchWolfe(newtondata, u, du, r, f, kappa);
      }
      // if(newtondata.getNewtonStatus() == AlatEnums::NewtonStatusMaxLineSearchAttained)
      // {
      //   return;
      // }
    }
    newtondata.iteration=iteration;
    if( _checkIteration( newtondata, info) )
    {
      return;
    }

    // same ???
    if(_newmatrix)
    {
      getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
    }
    getVisitor()->setLinearTolerance(_lintol, info.getGlobalTol(), B);
    _nliniter = getVisitor()->solve(newtondata.getLinearSolverStatus(), B, A, du, r);
    newtondata.getNLinearIteration() += _nliniter;
    _dxnorm =  getVisitor()->vectorNorm(du);
    // std::cerr << "NewtonRb: LINEAR status=" << AlatEnums::iterationStatusToString(newtondata.getLinearSolverStatus()) << "\n";
    if( ( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusMaxIter )||( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusDiverged )||( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusProblem ) )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusLinearNotOk;
      return;
    }
    // same ???
    
    
    
    if(_linesearchtype != "monotonicty")
    {
      Alat::GhostVector& h = getMemory(2);
      AlatEnums::residualstatus status_linearization = AlatEnums::ResidualStatusOk;
      getVisitor()->vectorZero(h);
      getVisitor()->computeLinearization(status_linearization, h, u, du);
      if(status_linearization != AlatEnums::ResidualStatusOk)
      {
        _error_string( "solve", "linearization not ok", AlatEnums::residualStatusToString(status_linearization) );
      }
      kappa = getVisitor()->scalarProduct(r, h);
    }
    getVisitor()->vectorAdd(u, 1.0, du);
  }
  assert(0);
}
