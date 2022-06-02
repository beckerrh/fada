#include  "Alat/newtondata.h"
#include  "Alat/newtonsimple.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <iomanip>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

NewtonSimple::~NewtonSimple()
{}

NewtonSimple::NewtonSimple() : NonlinearSolver()
{}

NewtonSimple::NewtonSimple( const NewtonSimple& newtonsimple) : NonlinearSolver(newtonsimple)
{
  ( *this ).operator=(newtonsimple);
}

NewtonSimple& NewtonSimple::operator=( const NewtonSimple& newtonsimple)
{
  NonlinearSolver::operator=(newtonsimple);
  assert(0);
  return *this;
}

std::string NewtonSimple::getName() const
{
  return "NewtonSimple";
}

NewtonSimple* NewtonSimple::clone() const
{
  return new NewtonSimple(*this);
}

/*--------------------------------------------------------------------------*/
int NewtonSimple::getNVectors() const
{
  return 2;
}

/*--------------------------------------------------------------------------*/
void NewtonSimple::solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  const Alat::IterationInfo& info = getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& du = getMemory(1);

  bool reached = 0;
  info._printbufff = " dxnorm matrix (lintol nlin)\n";
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusTooManyIterations;
      return;
    }
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
    {
      return;
    }
    newtondata.iteration=iteration;
    if(_checkIteration( newtondata, info) )
    {
      return;
    }
    if(_newmatrix)
    {
      getVisitor()->constructMatrixAndLinearSolvers(newtondata.getResidualStatus(), B, A, u);
    }
    getVisitor()->setLinearTolerance(_lintol, info.getGlobalTol(), B);
    _nliniter = getVisitor()->solve(newtondata.getLinearSolverStatus(), B, A, du, r);
    newtondata.getNLinearIteration() += _nliniter;
    _dxnorm =  getVisitor()->vectorNorm(du);
    if( ( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusMaxIter )||( newtondata.getLinearSolverStatus() == AlatEnums::IterationStatusDiverged ) )
    {
      // std::cerr << "NewtonSimple()::solve() newtondata.getLinearSolverStatus()="<<AlatEnums::iterationStatusToString(newtondata.getLinearSolverStatus())<<"\n";
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusLinearNotOk;
      return;
    }
    getVisitor()->vectorAdd(u, 1.0, du);
  }
  assert(0);
}
