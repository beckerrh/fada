#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/richardson.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

Richardson::~Richardson()
{}

Richardson::Richardson(double omega) : IterativeSolverWithPreconditioner(), _omega(omega)
{}

Richardson::Richardson( const Richardson& richardson) : IterativeSolverWithPreconditioner(richardson)
{
  assert(0);
}

Richardson& Richardson::operator=( const Richardson& richardson)
{
  IterativeSolverWithPreconditioner::operator=(richardson);
  assert(0);
  return *this;
}

std::string Richardson::getName() const
{
  return "Richardson";
}

Richardson* Richardson::clone() const
{
  assert(0);
//return new Richardson(*this);
}

/*--------------------------------------------------------------------------*/

int Richardson::getNVectors() const
{
  return 2;
}

/*--------------------------------------------------------------------------*/

void Richardson::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& z = getMemory(1);

  bool reached = 0;
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      status = AlatEnums::IterationStatusDiverged;
      return;
    }
    getVisitor()->residual(A, r, u, f);
    double res = getVisitor()->vectorNorm(r);
    info.checkIteration(status, res, iteration);
    bool reached = status==AlatEnums::IterationStatusConverged;
    if(reached)
    {
      return;
    }
    // std::cerr << "_omega="<<_omega<<"\n";
    // getPreconditioner()->solveApproximate(status, A, u, f, iteration);
    getVisitor()->vectorZero(z);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);
    // getVisitor()->vectorZero(u);
    getVisitor()->vectorAdd(u, _omega, z);
  }
  status =  AlatEnums::IterationStatusDiverged;
  return;
}
