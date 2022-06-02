#include  "Fada/domainmodelinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainModelInterface::~DomainModelInterface()
{}
DomainModelInterface::DomainModelInterface() : ModelInterface()
{}
DomainModelInterface::DomainModelInterface( const DomainModelInterface& modelinterface) : ModelInterface(modelinterface)
{
}

DomainModelInterface& DomainModelInterface::operator=( const DomainModelInterface& modelinterface)
{
  ModelInterface::operator=(modelinterface);
  return *this;
}

std::string DomainModelInterface::getInterfaceName() const
{
  return "DomainModelInterface";
}
