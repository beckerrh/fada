#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/orthomin.h"
#include  "Alat/preconditionerinterface.h"
#include  <cassert>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

Orthomin::~Orthomin()
{}

Orthomin::Orthomin() : IterativeSolverWithPreconditioner()
{}

Orthomin::Orthomin( const Orthomin& orthomin) : IterativeSolverWithPreconditioner(orthomin)
{
  assert(0);
}

Orthomin& Orthomin::operator=( const Orthomin& orthomin)
{
  IterativeSolverWithPreconditioner::operator=(orthomin);
  assert(0);
  return *this;
}

std::string Orthomin::getName() const
{
  return "Orthomin";
}

Orthomin* Orthomin::clone() const
{
  assert(0);
//return new Orthomin(*this);
}

/*--------------------------------------------------------------------------*/

int Orthomin::getNVectors() const
{
  return 5;
}

/*--------------------------------------------------------------------------*/

void Orthomin::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  const Alat::IterationInfo& info = *getIterationInfo();

  Alat::GhostVector& r = getMemory(0);
  Alat::GhostVector& z = getMemory(1);
  Alat::GhostVector& Az = getMemory(2);
  Alat::GhostVector& zold = getMemory(3);
  Alat::GhostVector& Azold = getMemory(4);

  getVisitor()->residual(A, r, u, f);
  double res = getVisitor()->vectorNorm(r);
  if( res < info.getGlobalTol() )
  {
    status = AlatEnums::IterationStatusNone;
    return;
  }
  // getVisitor()->vectorZero(z); 
  int iteration=0;
  getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);
  bool reached = 0;
  for(int iteration = 0; !reached; iteration++)
  {
    if( iteration == info.getMaxiter() )
    {
      status = AlatEnums::IterationStatusDiverged;
      return;
    }
    if(iteration)
    {
      getVisitor()->vectorEqual(Azold, Az);
    }
    getVisitor()->vectorZero(Az);
    getVisitor()->matrixVectorProduct(A, Az, z, 1.0);
    if(iteration)
    {
      double beta = getVisitor()->scalarProduct(Az, Azold)/getVisitor()->scalarProduct(Azold, Azold);
      getVisitor()->vectorAdd(z, -beta, zold);
      getVisitor()->vectorAdd(Az, -beta, Azold);
    }
    double scprAz = getVisitor()->scalarProduct(r, Az);
    double normAz = getVisitor()->scalarProduct(Az, Az);
    double omega = scprAz/normAz;
    // std::cerr << "iteration omega " << iteration << " " << omega << " scprAz normAz  " << scprAz << " " << normAz << "\n";
    getVisitor()->vectorAdd(u, omega, z);
    getVisitor()->vectorAdd(r, -omega, Az);
    res = getVisitor()->vectorNorm(r);
    // std::cerr << "Orthomin info res " << iteration << " " << res << "\n";
    info.checkIteration(status, res, iteration);
    bool reached = status==AlatEnums::IterationStatusConverged;
    if(reached)
    {
      return;
    }
    getVisitor()->vectorEqual(zold, z);
    getVisitor()->vectorZero(z);
    getPreconditioner()->solveApproximate(_statuspreconditioner, A, z, r, iteration);
  }
  status = AlatEnums::IterationStatusDiverged;
}
