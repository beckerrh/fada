#include  "Alat/simpleiterativesolver.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SimpleIterativeSolver::~SimpleIterativeSolver()
{}

SimpleIterativeSolver::SimpleIterativeSolver() : IterativeSolverWithPreconditioner()
{}

SimpleIterativeSolver::SimpleIterativeSolver( const SimpleIterativeSolver& simpleiterativesolver) : IterativeSolverWithPreconditioner(simpleiterativesolver)
{
  assert(0);
}

SimpleIterativeSolver& SimpleIterativeSolver::operator=( const SimpleIterativeSolver& simpleiterativesolver)
{
  IterativeSolverWithPreconditioner::operator=(simpleiterativesolver);
  assert(0);
  return *this;
}

std::string SimpleIterativeSolver::getName() const
{
  return "SimpleIterativeSolver";
}

SimpleIterativeSolver* SimpleIterativeSolver::clone() const
{
  assert(0);
//return new SimpleIterativeSolver(*this);
}

/*--------------------------------------------------------------------------*/

void SimpleIterativeSolver::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{ 
  // std::cerr << "SimpleIterativeSolver::solve() u="<< u << " f="<< f<<  " prec="<<getPreconditioner()->getName()<<"\n";
  // std::cerr << "SimpleIterativeSolver::solve() precvisitor "<<getPreconditioner()->getVisitor()->getName()<<"\n";
  int iteration=0;
  getPreconditioner()->solveApproximate(status, A, u, f, iteration);
}
