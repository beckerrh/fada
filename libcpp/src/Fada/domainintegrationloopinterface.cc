#include  "Fada/domainintegrationloopinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainIntegrationLoopInterface::~DomainIntegrationLoopInterface(){}
DomainIntegrationLoopInterface::DomainIntegrationLoopInterface() : IntegrationLoopInterface(){}
DomainIntegrationLoopInterface::DomainIntegrationLoopInterface( const DomainIntegrationLoopInterface& domainintegrationloopinterface) : IntegrationLoopInterface(domainintegrationloopinterface)
{
  assert(0);
}
DomainIntegrationLoopInterface& DomainIntegrationLoopInterface::operator=( const DomainIntegrationLoopInterface& domainintegrationloopinterface)
{
  IntegrationLoopInterface::operator=(domainintegrationloopinterface);
  assert(0);
  return *this;
}
std::string DomainIntegrationLoopInterface::getInterfaceName() const
{
  return "DomainIntegrationLoopInterface";
}
std::string DomainIntegrationLoopInterface::getName() const
{
  return "DomainIntegrationLoopInterface";
}
DomainIntegrationLoopInterface* DomainIntegrationLoopInterface::clone() const
{
  assert(0);
}
