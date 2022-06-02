#include  "Fada/domainsolverinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainSolverInterface::~DomainSolverInterface(){}
DomainSolverInterface::DomainSolverInterface() : SolverInterface(){}
DomainSolverInterface::DomainSolverInterface( const DomainSolverInterface& domainsolverinterface) : SolverInterface(domainsolverinterface)
{
  assert(0);
}
DomainSolverInterface& DomainSolverInterface::operator=( const DomainSolverInterface& domainsolverinterface)
{
  SolverInterface::operator=(domainsolverinterface);
  assert(0);
  return *this;
}
std::string DomainSolverInterface::getInterfaceName() const
{
  return "DomainSolverInterface";
}
std::string DomainSolverInterface::getName() const
{
  return "DomainSolverInterface";
}

/*--------------------------------------------------------------------------*/
DomainIntegrationLoopInterface* DomainSolverInterface::newDomainIntegrationLoop() const
{
  _notWritten("newDomainIntegrationLoop");
  return NULL;
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::addDiagonalCoupling(int level, Alat::GhostMatrix& gA, const Alat::SystemMatrixInterface* matrix) const
{
  _notWritten("addDiagonalCoupling");
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::reInitCouplingSolvers(int n)
{
  _notWritten("reInitCouplingSolvers");
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::reInitLinearSolvers()
{
  _notWritten("reInitLinearSolvers");
}

/*--------------------------------------------------------------------------*/
MultiLevelTransferAllVariablesInterface* DomainSolverInterface::newMultiLevelTransferAllVariables()
{
  _notWritten("newMultiLevelTransferAllVariables");
}
MultiLevelTransferSingleFemInterface* DomainSolverInterface::newMultiLevelTransferSingleFem(const Fada::FemInterface* fem) const
{
  _notWritten("newMultiLevelTransferSingleFem");
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::reInitVariableVectorForInterpolation(int level, Alat::VariableVectorInterface* u, const Fada::VariableInterface* var, const FadaMesh::MeshInterface* mesh) const
{
  _notWritten("reInitVariableVectorForInterpolation");
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const
{
  _notWritten("interpolateSolution");
}

/*--------------------------------------------------------------------------*/
void DomainSolverInterface::strongDirichletVectorSolution(Alat::GhostVector& u) const{}

void DomainSolverInterface::strongDirichletVectorZero(Alat::GhostVector& f) const{}

void DomainSolverInterface::strongDirichletMatrix(Alat::GhostMatrix& gA) const{}

void DomainSolverInterface::newtonProject(Alat::GhostVector& gu) const{}
void DomainSolverInterface::beforeTimestep(Alat::GhostVector& gu) const{}
void DomainSolverInterface::interpolateOneVariable(const DofInformationInterface* dofinformation, Alat::VariableVectorInterface* unew, const Alat::VariableVectorInterface* uold) const
{
  _notWritten("interpolateOneVariable");
}
