#include  "Fada/applicationinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
ApplicationInterface::~ApplicationInterface()
{}
ApplicationInterface::ApplicationInterface() : Alat::InterfaceBase()
{}
ApplicationInterface::ApplicationInterface( const ApplicationInterface& applicationinterface) : Alat::InterfaceBase(applicationinterface)
{
  *this = applicationinterface;
}

ApplicationInterface& ApplicationInterface::operator=( const ApplicationInterface& applicationinterface)
{
  InterfaceBase::operator=(applicationinterface);
  return *this;
}

std::string ApplicationInterface::getInterfaceName() const
{
  return "ApplicationInterface";
}

std::string ApplicationInterface::getName() const
{
  return "ApplicationInterface";
}

/*--------------------------------------------------------------------------*/
Fada::DataInterface* ApplicationInterface::defineData(std::string name) const
{
  return NULL;
}
void ApplicationInterface::defineIntegrators(Fada::IntegratorManager* integratormanager) const{}
