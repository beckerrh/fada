#ifndef ___SolverManager_h
#define ___SolverManager_h

#include  "Fada/solvermanager.h"

/*--------------------------------------------------------------------------*/
class SolverManager : public Fada::SolverManager
{
protected:
  Fada::DomainSolverInterface* newDomainSolver(const FadaMesh::MeshInterface* mesh) const;

public:
  ~SolverManager();
  SolverManager();
  SolverManager( const SolverManager& solvermanager);
  SolverManager& operator=( const SolverManager& solvermanager);
  std::string getName() const;
  SolverManager* clone() const;
  void projectToAllLevels(Alat::GhostVector& u) const;
  void testProlongate(Alat::GhostVector& u) const;
};

/*--------------------------------------------------------------------------*/
#endif
