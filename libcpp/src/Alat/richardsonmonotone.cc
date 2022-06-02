#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/richardsonmonotone.h"
#include  "stdlib.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

RichardsonMonotone::~RichardsonMonotone()
{}

RichardsonMonotone::RichardsonMonotone() : IterativeSolverWithPreconditioner()
{}

RichardsonMonotone::RichardsonMonotone( const RichardsonMonotone& richardsonmonotone) : IterativeSolverWithPreconditioner(richardsonmonotone)
{
  assert(0);
}

RichardsonMonotone& RichardsonMonotone::operator=( const RichardsonMonotone& richardsonmonotone)
{
  IterativeSolverWithPreconditioner::operator=(richardsonmonotone);
  assert(0);
  return *this;
}

std::string RichardsonMonotone::getName() const
{
  return "RichardsonMonotone";
}

RichardsonMonotone* RichardsonMonotone::clone() const
{
  assert(0);
//return new RichardsonMonotone(*this);
}

/*--------------------------------------------------------------------------*/

int RichardsonMonotone::getNVectors() const
{
  return 3;
}

/*--------------------------------------------------------------------------*/

void RichardsonMonotone::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& z = getMemory(1);
  Alat::GhostVector& Az = getMemory(2);

  bool reached = 0;
  double res;
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      status = AlatEnums::IterationStatusDiverged;
      return;
    }
    if(iteration == 0)
    {
      getVisitor()->residual(A, r, u, f);
    }
    res = getVisitor()->vectorNorm(r);
    info.checkIteration(status, res, iteration);
    bool reached = status == AlatEnums::IterationStatusConverged;
    if(reached)
    {
      return;
    }

    double rnorm = getVisitor()->vectorNorm(r);
    getVisitor()->vectorScale(r,1./rnorm);

    getVisitor()->vectorZero(z);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);

    getVisitor()->vectorZero(Az);
    getVisitor()->matrixVectorProduct(A, Az, z, 1.0);
    double scprAz = getVisitor()->scalarProduct(r, Az);
    double normAz = getVisitor()->scalarProduct(Az, Az);
    double omega = scprAz/normAz;
    // std::cerr << "iteration omega " << iteration << " " << omega << " scprAz normAz  " << scprAz << " " << normAz << " res=" << res << " omega=" << omega << "\n";
    if(normAz < 1e-28)
    {
      std::cerr << "iteration omega " << iteration << " " << omega << " scprAz normAz  " << scprAz << " " << normAz << " res=" << res << " omega=" << omega << "\n";
      omega = 1.0;
    }
    omega *= rnorm;
    getVisitor()->vectorAdd(u, omega, z);
    getVisitor()->vectorScale(r,rnorm);
    getVisitor()->vectorAdd(r, -omega, Az);
    // getVisitor()->residual(r, u, f);
  }
}
