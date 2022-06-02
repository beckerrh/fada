#include  "Integrators/diffusionmatrixinterface.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

DiffusionMatrixInterface::~DiffusionMatrixInterface()
{}

/*--------------------------------------------------------------------------*/

DiffusionMatrixInterface::DiffusionMatrixInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

DiffusionMatrixInterface::DiffusionMatrixInterface( const DiffusionMatrixInterface& diffusionmatrixinterface) : Alat::InterfaceBase(diffusionmatrixinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

DiffusionMatrixInterface& DiffusionMatrixInterface::operator=( const DiffusionMatrixInterface& diffusionmatrixinterface)
{
  Alat::InterfaceBase::operator=(diffusionmatrixinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string DiffusionMatrixInterface::getName() const
{
  return "DiffusionMatrixInterface";
}

/*--------------------------------------------------------------------------*/

std::string DiffusionMatrixInterface::getInterfaceName() const
{
  return "DiffusionMatrixInterface";
}
