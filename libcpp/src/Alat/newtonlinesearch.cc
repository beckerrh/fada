#include  "Alat/filescanner.h"
#include  "Alat/newtondata.h"
#include  "Alat/newtonlinesearch.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <limits>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
NewtonLineSearch::~NewtonLineSearch()
{}

NewtonLineSearch::NewtonLineSearch(std::string linesearchtype) : NonlinearSolver(), _linesearchtype(linesearchtype)
{
  assert( ( linesearchtype == "monotonicty" )or(linesearchtype == "armijo") or(linesearchtype == "wolfe") );
}

NewtonLineSearch::NewtonLineSearch( const NewtonLineSearch& newtonsimple) : NonlinearSolver(newtonsimple)
{
  ( *this ).operator=(newtonsimple);
}

NewtonLineSearch& NewtonLineSearch::operator=( const NewtonLineSearch& newtonsimple)
{
  NonlinearSolver::operator=(newtonsimple);
  assert(0);
  return *this;
}

std::string NewtonLineSearch::getName() const
{
  return "NewtonLineSearch";
}

NewtonLineSearch* NewtonLineSearch::clone() const
{
  return new NewtonLineSearch(*this);
}

/*--------------------------------------------------------------------------*/
int NewtonLineSearch::getNVectors() const
{
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
void NewtonLineSearch::solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  const Alat::IterationInfo& info = getIterationInfo();
  newtondata.use_linesearch = true;

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& du = getMemory(1);

  bool reached = 0;
  double kappa;
  int irelax = 0;
  info._printbufff = " dxnorm irelax matrix (lintol nlin)\n";
  // std::cerr << "FFFF NewtonLineSearch::solve() \n";
  // getVisitor()->vectorWrite(std::cerr, f);
  // std::cerr << "\n";
  std::cerr << "NewtonLineSearch::solve() info =" << info << "\n";
  for(int iteration = 0; !reached; iteration++)
  {
    // std::cerr << "NewtonLineSearch::solve() iteration ="<<iteration<< " u =" << getVisitor()->vectorNorm(u)<< " f =" << getVisitor()->vectorNorm(f) << "\n";

    if( iteration == info.getMaxiter() )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusTooManyIterations;
      return;
    }
    newtondata.residual_old = newtondata.residual;
    // std::cerr << "UUUU NewtonLineSearch::solve() \n";
    // getVisitor()->vectorWrite(std::cerr, u);
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    if( newtondata.residual >= std::numeric_limits<double>::max() )
    {
      std::cerr << "#NewtonLineSearch::solve() newtondata.residual=" << newtondata.residual<<"\n";
      newtondata.getResidualStatus() = AlatEnums::ResidualStatusNotOk;
    }
    // std::cerr << "RRRR NewtonLineSearch::solve() r= \n";
    // getVisitor()->vectorWrite(std::cerr, r);
    if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusResidualStatusNotOk;
      std::cerr << "NewtonLineSearch::solve() " << AlatEnums::residualStatusToString( newtondata.getResidualStatus() )<<"\n";
      std::cerr << "NewtonLineSearch::solve() " << AlatEnums::newtonStatusToString( newtondata.getNewtonStatus() )<<"\n";
      if(iteration)
      {
        getVisitor()->vectorAdd(u, -1.0, du);
      }
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
      if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
      {
        newtondata.getNewtonStatus() = AlatEnums::NewtonStatusResidualStatusNotOk;
        std::cerr << "#NewtonLineSearch::solve() " << AlatEnums::newtonStatusToString( newtondata.getNewtonStatus() )<<"\n";
        assert(0);
        return;
      }
      // if(newtondata.getNewtonStatus() == AlatEnums::NewtonStatusMaxLineSearchAttained)
      // {
      //   return;
      // }
    }
    newtondata.iteration = iteration;
    if( _checkIteration( newtondata, info) )
    {
      return;
    }

    // same ???
    if(_newmatrix)
    {
      getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
    }
    if(newtondata.getMatrixStatus() == AlatEnums::MatrixStatusNotOk)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusResidualStatusNotOk;
      return;
    }
    getVisitor()->setLinearTolerance(_lintol, info.getGlobalTol(), B);
    // std::cerr << "NewtonLineSearch::solve() " << getVisitor()->getName() << "\n";
    _nliniter = getVisitor()->solve(newtondata.getLinearSolverStatus(), B, A, du, r);
    // std::cerr << "NewtonLineSearch::solve() _nliniter="<<_nliniter<<"\n";
    newtondata.getNLinearIteration() += _nliniter;
    _dxnorm =  getVisitor()->vectorNorm(du);
    // std::cerr << "NewtonLineSearch: LINEAR status=" << AlatEnums::iterationStatusToString(newtondata.getLinearSolverStatus()) << "\n";
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
