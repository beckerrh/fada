#include  "Fada/integrationformulaselectorinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorInterface::~IntegrationFormulaSelectorInterface() 
{
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorInterface::IntegrationFormulaSelectorInterface(): InterfaceBase()
{
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorInterface::IntegrationFormulaSelectorInterface( const IntegrationFormulaSelectorInterface& integrationformulaselectorinterface): InterfaceBase(integrationformulaselectorinterface)
{
assert(0);
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorInterface& IntegrationFormulaSelectorInterface::operator=( const IntegrationFormulaSelectorInterface& integrationformulaselectorinterface) 
{
InterfaceBase::operator=(integrationformulaselectorinterface);
assert(0);
return *this;
}

/*--------------------------------------------------------------------------*/

std::string IntegrationFormulaSelectorInterface::getInterfaceName() const 
{
return "IntegrationFormulaSelectorInterface";
}

/*--------------------------------------------------------------------------*/

std::string IntegrationFormulaSelectorInterface::getName() const 
{
return "IntegrationFormulaSelectorInterface";
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorInterface* IntegrationFormulaSelectorInterface::clone() const 
{
  assert(0);
// return new IntegrationFormulaSelectorInterface(*this);
}
