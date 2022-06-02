#include  "Alat/systemvectorinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SystemVectorInterface::~SystemVectorInterface(){}
SystemVectorInterface::SystemVectorInterface() : VectorInterface(){}
SystemVectorInterface::SystemVectorInterface( const Alat::SystemVectorInterface& vectorinterface) : VectorInterface(vectorinterface){}
SystemVectorInterface* Alat::SystemVectorInterface::clone() const
{
  _notWritten("clone");
  return NULL;
}

std::string Alat::SystemVectorInterface::getInterfaceName() const
{
  return "SystemVectorInterface";
}

/*--------------------------------------------------------------------------*/
SystemVectorInterface& Alat::SystemVectorInterface::operator=( const Alat::SystemVectorInterface& vectorinterface)
{
  VectorInterface::operator=(vectorinterface);
  assert(0);
  return *this;
}
