#include  "Fada/modelinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
ModelInterface::~ModelInterface()
{}
ModelInterface::ModelInterface() : Alat::InterfaceBase()
{}
ModelInterface::ModelInterface( const ModelInterface& modelinterface) : Alat::InterfaceBase(modelinterface)
{
  *this = modelinterface;
}

ModelInterface& ModelInterface::operator=( const ModelInterface& modelinterface)
{
  InterfaceBase::operator=(modelinterface);
  return *this;
}

std::string ModelInterface::getInterfaceName() const
{
  return "ModelInterface";
}
Fada::DomainIntegrationLoopInterface* ModelInterface::newDiscretization() const
{
  _notWritten("newDiscretization");
}
void ModelInterface::init(){}
