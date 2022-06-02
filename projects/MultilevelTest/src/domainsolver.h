#ifndef ___DomainSolver_h
#define ___DomainSolver_h

#include  "Fada/domainsolver.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class DomainIntegrationLoopInterface;
}
class DomainSolver : public Fada::DomainSolver
{
public:
  ~DomainSolver();
  DomainSolver();
  DomainSolver( const DomainSolver& domainsolver);
  DomainSolver& operator=( const DomainSolver& domainsolver);
  std::string getName() const;
  DomainSolver* clone() const;

  Fada::DomainIntegrationLoopInterface* newDomainIntegrationLoop() const;
  void interpolateSolution( Alat::GhostVector& unew, const Alat::GhostVector& uold) const;
  void testProlongate(Alat::GhostVector& u) const;
  void project( const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine) const;
};

/*--------------------------------------------------------------------------*/
#endif
