#include  "Alat/bicgstab.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

BiCgStab::~BiCgStab()
{}

BiCgStab::BiCgStab() : IterativeSolverWithPreconditioner()
{}

BiCgStab::BiCgStab( const BiCgStab& bicgstab) : IterativeSolverWithPreconditioner(bicgstab)
{
  assert(0);
}

BiCgStab& BiCgStab::operator=( const BiCgStab& bicgstab)
{
  IterativeSolverWithPreconditioner::operator=(bicgstab);
  assert(0);
  return *this;
}

std::string BiCgStab::getName() const
{
  return "BiCgStab";
}

BiCgStab* BiCgStab::clone() const
{
  assert(0);
//return new BiCgStab(*this);
}

/*--------------------------------------------------------------------------*/

int BiCgStab::getNVectors() const
{
  return 7;
}

/*--------------------------------------------------------------------------*/


void BiCgStab::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();
  double epsilon = 1e-32;

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& rtilde = getMemory(1);
  Alat::GhostVector& p = getMemory(2);
  Alat::GhostVector& v = getMemory(3);
  Alat::GhostVector& precvec = getMemory(4);
  Alat::GhostVector& s = getMemory(5);
  Alat::GhostVector& t = getMemory(6);

  getVisitor()->vectorEqual(r, f);
  getVisitor()->matrixVectorProduct(A, r, u, -1.0);
  double res = getVisitor()->vectorNorm(r);
  // std::cerr << "BiCgStab res " << res << "\n";
  if( res < info.getGlobalTol() )
  {
    status = AlatEnums::IterationStatusNone;
    return;
  }
  getVisitor()->vectorEqual(rtilde, r);

  double rho, rhoold;
  double alpha, omega;

  bool reached = 0;
  for(int iteration = 0; !reached; iteration++)
  {
    rhoold = rho;
    rho = getVisitor()->scalarProduct(r, rtilde);
    if(fabs(rho) < epsilon)
    {
      std::cerr << "¨*** ERROR in BiCgStab: rho is zero iteration rho " << iteration << " " << rho << "\n";
      assert(0);
    }
    if(iteration == 0)
    {
      getVisitor()->vectorEqual(p, r);
    }
    else
    {
      double beta = ( rho/rhoold )*( alpha/omega );
      getVisitor()->vectorAdd(p, beta-1.0, p);
      getVisitor()->vectorAdd(p, 1.0, r);
      getVisitor()->vectorAdd(p, -beta*omega, v);
    }
    // getVisitor()->precondition(precvec, p);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, precvec, p, iteration);
    getVisitor()->vectorZero(v);
    getVisitor()->matrixVectorProduct(A, v, precvec, 1.0);

    alpha = rho / getVisitor()->scalarProduct(v, rtilde);
    getVisitor()->vectorEqual(s, r);
    getVisitor()->vectorAdd(s, -alpha, v);
    res = getVisitor()->vectorNorm(s);
    getVisitor()->vectorAdd(u, alpha, precvec);
    info.checkIteration(status, res, iteration);
    if( ( status == AlatEnums::IterationStatusConverged ) or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
    {
      return;
    }
    // getVisitor()->precondition(precvec, s);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, precvec, s, iteration);
    getVisitor()->vectorZero(t);
    getVisitor()->matrixVectorProduct(A, t, precvec, 1.0);

    omega = getVisitor()->scalarProduct(t, s)/getVisitor()->scalarProduct(t, t);
    getVisitor()->vectorAdd(u, omega, precvec);
    getVisitor()->vectorEqual(r, s);
    getVisitor()->vectorAdd(r, -omega, t);
    if(fabs(omega) < epsilon)
    {
      std::cerr << "BiCgStab: omega is zero\n";
      assert(0);
    }
  }
  status = AlatEnums::IterationStatusDiverged;
  return;
}
