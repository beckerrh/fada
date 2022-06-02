#include  "Fada/femmanagerinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
FemManagerInterface::~FemManagerInterface(){}
FemManagerInterface::FemManagerInterface() : Alat::InterfaceBase(){}
FemManagerInterface::FemManagerInterface( const FemManagerInterface& femmanagerinterface) : Alat::InterfaceBase(femmanagerinterface)
{
  assert(0);
}
FemManagerInterface& FemManagerInterface::operator=( const FemManagerInterface& femmanagerinterface)
{
  InterfaceBase::operator=(femmanagerinterface);
  assert(0);
  return *this;
}
std::string FemManagerInterface::getInterfaceName() const
{
  return "FemManagerInterface";
}
std::string FemManagerInterface::getName() const
{
  return "FemManagerInterface";
}
FemManagerInterface* FemManagerInterface::clone() const
{
  assert(0);
  return NULL;
  // return new FemManagerInterface(*this);
}
