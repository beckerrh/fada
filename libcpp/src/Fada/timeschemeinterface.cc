#include  "Fada/timeschemeinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeSchemeInterface::~TimeSchemeInterface(){}
TimeSchemeInterface::TimeSchemeInterface() : Alat::InterfaceBase(){}
TimeSchemeInterface::TimeSchemeInterface( const TimeSchemeInterface& timeschemeinterface) : Alat::InterfaceBase(timeschemeinterface)
{
  assert(0);
}
TimeSchemeInterface& TimeSchemeInterface::operator=( const TimeSchemeInterface& timeschemeinterface)
{
  InterfaceBase::operator=(timeschemeinterface);
  assert(0);
  return *this;
}
std::string TimeSchemeInterface::getInterfaceName() const
{
  return "TimeSchemeInterface";
}
std::string TimeSchemeInterface::getName() const
{
  return "TimeSchemeInterface";
}
