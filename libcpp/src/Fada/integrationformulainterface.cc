#include  "Fada/integrationformulainterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegrationFormulaInterface::~IntegrationFormulaInterface(){}
IntegrationFormulaInterface::IntegrationFormulaInterface() : Alat::InterfaceBase(){}
IntegrationFormulaInterface::IntegrationFormulaInterface( const IntegrationFormulaInterface& integrationformulainterface) : Alat::InterfaceBase(integrationformulainterface)
{
  assert(0);
}
IntegrationFormulaInterface& IntegrationFormulaInterface::operator=( const IntegrationFormulaInterface& integrationformulainterface)
{
  InterfaceBase::operator=(integrationformulainterface);
  assert(0);
  return *this;
}
std::string IntegrationFormulaInterface::getInterfaceName() const
{
  return "Fada::IntegrationFormulaInterface";
}
std::string IntegrationFormulaInterface::getName() const
{
  return "IntegrationFormulaInterface";
}
