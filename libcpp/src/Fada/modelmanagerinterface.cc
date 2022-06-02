#include  "Fada/modelmanagerinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
ModelManagerInterface::~ModelManagerInterface()
{}
ModelManagerInterface::ModelManagerInterface() : Alat::InterfaceBase()
{}
ModelManagerInterface::ModelManagerInterface( const ModelManagerInterface& modelmanagerinterface) : Alat::InterfaceBase(modelmanagerinterface)
{
  assert(0);
}

ModelManagerInterface& ModelManagerInterface::operator=( const ModelManagerInterface& modelmanagerinterface)
{
  InterfaceBase::operator=(modelmanagerinterface);
  assert(0);
  return *this;
}

std::string ModelManagerInterface::getInterfaceName() const
{
  return "ModelManagerInterface";
}

/*--------------------------------------------------------------------------*/
int ModelManagerInterface::getLeftDomain(int icoupling) const
{
  _notWritten("getLeftDomain(int icoupling)");
  return 0;
}

int ModelManagerInterface::getRightDomain(int icoupling) const
{
  _notWritten("getRightDomain(int icoupling) ");
  return 0;
}

/*---------------------------------------------------------*/
Fada::DomainModelInterface* ModelManagerInterface::constructModelByName(const std::string& modelname) const
{
  _notWritten("ModelManager::constructModelByName");
  return NULL;
}
