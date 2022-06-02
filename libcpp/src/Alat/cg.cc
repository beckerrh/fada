#include  "Alat/cg.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

Cg::~Cg() 
{
}

Cg::Cg(): IterativeSolverWithPreconditioner()
{
}

Cg::Cg( const Cg& cg): IterativeSolverWithPreconditioner(cg)
{
assert(0);
}

Cg& Cg::operator=( const Cg& cg) 
{
IterativeSolverWithPreconditioner::operator=(cg);
assert(0);
return *this;
}

std::string Cg::getName() const 
{
return "Cg";
}

Cg* Cg::clone() const 
{
assert(0);
//return new Cg(*this);
}

/*--------------------------------------------------------------------------*/

int Cg::getNVectors() const
{
  return 4;
}

/*--------------------------------------------------------------------------*/

void Cg::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& z = getMemory(1);
  Alat::GhostVector& d = getMemory(2);
  Alat::GhostVector& Ad = getMemory(3);

  getVisitor()->residual(A, r, u, f);
  double res = getVisitor()->vectorNorm(r);

  if( res < info.getGlobalTol() )
  {
    status = AlatEnums::IterationStatusNone;
    return;
  }

  int iteration=0;
  getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);
  double zr  = getVisitor()->scalarProduct(z, r);
  getVisitor()->vectorEqual(d, z);

  bool reached = 0;

  for(int iteration = 0; !reached; iteration++)
  {
    getVisitor()->vectorZero(Ad);
    getVisitor()->matrixVectorProduct(A, Ad, d, 1.0);

    double alpha = zr / getVisitor()->scalarProduct(d, Ad);

    getVisitor()->vectorAdd(r, -alpha, Ad);
    getVisitor()->vectorAdd(u, alpha, d);
    res = getVisitor()->vectorNorm(r);

    info.checkIteration(status, res, iteration);
    if( ( status == AlatEnums::IterationStatusConverged ) or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
    {
      return;
    }

    getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);

    double zrold = zr;
    zr   = getVisitor()->scalarProduct(z, r);
    double beta = zr/zrold;

    getVisitor()->vectorAdd(d, beta-1.0, d);
    getVisitor()->vectorAdd(d, 1.0, z);
  }
  status = AlatEnums::IterationStatusDiverged;
  return;
}

