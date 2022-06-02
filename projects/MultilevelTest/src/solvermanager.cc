#include  "solvermanager.h"
#include  "domainsolver.h"
#include  "Fada/domainsolverinterface.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
SolverManager::~SolverManager()
{
}

SolverManager::SolverManager(): Fada::SolverManager()
{
}

SolverManager::SolverManager( const SolverManager& solvermanager): Fada::SolverManager(solvermanager)
{
(*this).operator=(solvermanager);
}

SolverManager& SolverManager::operator=( const SolverManager& solvermanager)
{
Fada::SolverManager::operator=(solvermanager);
assert(0);
return *this;
}

std::string SolverManager::getName() const
{
return "SolverManager";
}

SolverManager* SolverManager::clone() const
{
return new SolverManager(*this);
}

/*--------------------------------------------------------------------------*/
Fada::DomainSolverInterface* SolverManager::newDomainSolver(const FadaMesh::MeshInterface* mesh) const
{
  return new DomainSolver;
}
void SolverManager::projectToAllLevels(Alat::GhostVector& u) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->projectToAllLevels( u );
  }
}
/*--------------------------------------------------------------------------*/
void SolverManager::testProlongate(Alat::GhostVector& u) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    const DomainSolver* domainsolver = dynamic_cast<const DomainSolver*>(getDomainSolver(i));
    assert(domainsolver);
    domainsolver->testProlongate( u );
  }
}
