#include  "Fada/integrationloopinterface.h"
#include  "Fada/integratorinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegrationLoopInterface::~IntegrationLoopInterface()
{}
IntegrationLoopInterface::IntegrationLoopInterface() : Alat::InterfaceBase()
{}
IntegrationLoopInterface::IntegrationLoopInterface( const IntegrationLoopInterface& integrationloopinterface) : Alat::InterfaceBase(integrationloopinterface)
{
  assert(0);
}
IntegrationLoopInterface& IntegrationLoopInterface::operator=( const IntegrationLoopInterface& integrationloopinterface)
{
  InterfaceBase::operator=(integrationloopinterface);
  assert(0);
  return *this;
}
std::string IntegrationLoopInterface::getInterfaceName() const
{
  return "Fada::IntegrationLoopInterface";
}

/*--------------------------------------------------------------------------*/
void IntegrationLoopInterface::matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* matrix) const
{
  _notWritten("matrixConnectivityPerLevel");
}

/*--------------------------------------------------------------------------*/
void IntegrationLoopInterface::matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::SparsityPattern* sparsitypattern) const
{
  _notWritten("matrixConnectivityPerLevel");
}
