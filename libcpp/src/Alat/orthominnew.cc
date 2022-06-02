#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/orthominnew.h"
#include  "Alat/preconditionerinterface.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

OrthominNew::~OrthominNew()
{}

OrthominNew::OrthominNew() : IterativeSolverWithPreconditioner()
{}

OrthominNew::OrthominNew( const OrthominNew& orthomin) : IterativeSolverWithPreconditioner(orthomin)
{
  assert(0);
}

OrthominNew& OrthominNew::operator=( const OrthominNew& orthomin)
{
  IterativeSolverWithPreconditioner::operator=(orthomin);
  assert(0);
  return *this;
}

std::string OrthominNew::getName() const
{
  return "OrthominNew";
}

OrthominNew* OrthominNew::clone() const
{
  assert(0);
//return new OrthominNew(*this);
}

/*--------------------------------------------------------------------------*/

int OrthominNew::getNVectors() const
{
  return 7;
}

/*--------------------------------------------------------------------------*/

void OrthominNew::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& z = getMemory(1);
  Alat::GhostVector& p = getMemory(2);
  Alat::GhostVector& y = getMemory(3);
  Alat::GhostVector& Az = getMemory(4);
  Alat::GhostVector& h = getMemory(5);
  Alat::GhostVector& r2 = getMemory(6);

  getVisitor()->residual(A, r, u, f);
  getVisitor()->vectorEqual(r2, r);
  double res = getVisitor()->vectorNorm(r);
  if( res < info.getGlobalTol() )
  {
    status = AlatEnums::IterationStatusNone;
    return;
  }
  int iteration=0;
  getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);
  
  getVisitor()->vectorEqual(p, z);
  getVisitor()->vectorEqual(y, z);
  getVisitor()->vectorScale(y, -1.0);
  // double nu = getVisitor()->scalarProduct(z,z);
  
  
  double omega, eta, nu;
  bool reached = 0;
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      status = AlatEnums::IterationStatusDiverged;
      return;
    }
    getVisitor()->vectorZero(h);
    getVisitor()->matrixVectorProduct(A, h, z, 1.0);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, Az, h, iteration);

    double scpzAz = getVisitor()->scalarProduct(Az,z);
    double scpAzAz = getVisitor()->scalarProduct(Az,Az);
    if(iteration==0)
    {
      omega = scpzAz/scpAzAz;
      eta = 0.0;
      nu = omega*scpzAz;
    }
    else
    {
      double omegaold=omega;
      double scpyAz = getVisitor()->scalarProduct(Az,y);
      double denominateur = nu*scpAzAz - scpyAz*scpyAz;
      omega = nu*scpzAz/denominateur;
      eta = -scpyAz*scpzAz/denominateur;
      getVisitor()->vectorScale(p, eta*omegaold/omega);
      getVisitor()->vectorAdd(p, 1.0, z);
    }
    getVisitor()->vectorAdd(u, omega, p);
    getVisitor()->vectorAdd(r2, eta, Az);
    getVisitor()->residual(A, r, u, f);
    res = getVisitor()->vectorNorm(r);
    // std::cerr << "res res2 " << res << " =? " << getVisitor()->vectorNorm(r2) << "\n";
    info.checkIteration(status, res, iteration);
    bool reached = status==AlatEnums::IterationStatusConverged;
    if(reached)
    {
      return;
    }
    getVisitor()->vectorScale(y, eta);
    getVisitor()->vectorAdd(y, omega, Az);
    getVisitor()->vectorAdd(z, -1.0, y);
    
  }
  status = AlatEnums::IterationStatusDiverged;
  return;
}
