#include  "Fada/maininterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MainInterface::~MainInterface()
{}
MainInterface::MainInterface() : Alat::InterfaceBase()
{}
MainInterface::MainInterface( const MainInterface& maininterface) : Alat::InterfaceBase(maininterface)
{
  assert(0);
}
std::string MainInterface::getInterfaceName() const
{
  return "Fada::MainInterface";
}
std::string MainInterface::getName() const
{
  return "MainInterface";
}
MainInterface& MainInterface::operator=( const MainInterface& maininterface)
{
  InterfaceBase::operator=(maininterface);
  assert(0);
  return *this;
}
