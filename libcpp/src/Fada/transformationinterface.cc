#include  "Fada/transformationinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

TransformationInterface::~TransformationInterface()
{}

/*--------------------------------------------------------------------------*/

TransformationInterface::TransformationInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

TransformationInterface::TransformationInterface( const TransformationInterface& transformationinterface) : Alat::InterfaceBase(transformationinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

TransformationInterface& TransformationInterface::operator=( const TransformationInterface& transformationinterface)
{
  InterfaceBase::operator=(transformationinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string TransformationInterface::getInterfaceName() const
{
  return "Fada::TransformationInterface";
}
