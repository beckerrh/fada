#include  "Fada/multileveltransferallvariablesinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferAllVariablesInterface::~MultiLevelTransferAllVariablesInterface()
{}
MultiLevelTransferAllVariablesInterface::MultiLevelTransferAllVariablesInterface() : Alat::InterfaceBase()
{}
MultiLevelTransferAllVariablesInterface::MultiLevelTransferAllVariablesInterface( const MultiLevelTransferAllVariablesInterface& multigridtransferinterface) : Alat::InterfaceBase(multigridtransferinterface)
{
  assert(0);
}

MultiLevelTransferAllVariablesInterface& MultiLevelTransferAllVariablesInterface::operator=( const MultiLevelTransferAllVariablesInterface& multigridtransferinterface)
{
  InterfaceBase::operator=(multigridtransferinterface);
  assert(0);
  return *this;
}

std::string MultiLevelTransferAllVariablesInterface::getName() const
{
  return "MultiLevelTransferAllVariablesInterface";
}

std::string MultiLevelTransferAllVariablesInterface::getInterfaceName() const
{
  return "MultiLevelTransferAllVariablesInterface";
}

MultiLevelTransferAllVariablesInterface* MultiLevelTransferAllVariablesInterface::clone() const
{
  assert(0);
  // return new MultiLevelTransferAllVariablesInterface(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferAllVariablesInterface::basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const DofManagerAllVariables* dofmanagerallvariables)
{
  _notWritten("basicInit");
}

void MultiLevelTransferAllVariablesInterface::constructTransfer(int level, const Alat::SystemMatrixInterface* matrixfine, const Alat::SystemMatrixInterface* matrixcoarse)
{
  _notWritten("constructTransfer");
}

bool MultiLevelTransferAllVariablesInterface::isConstant() const
{
  return true;
}

void MultiLevelTransferAllVariablesInterface::interpolate(int level, Alat::SystemVectorInterface* ufines, const Alat::SystemVectorInterface* ucoarses) const
{
  _notWritten("interpolate");
}
