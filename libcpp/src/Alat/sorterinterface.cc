#include  "Alat/sorterinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/

SorterInterface::~SorterInterface()
{}

SorterInterface::SorterInterface() : Alat::InterfaceBase()
{}

SorterInterface::SorterInterface( const SorterInterface& sorterinterface) : Alat::InterfaceBase(sorterinterface)
{
  assert(0);
}

SorterInterface& SorterInterface::operator=( const SorterInterface& sorterinterface)
{
  Alat::InterfaceBase::operator=(sorterinterface);
  assert(0);
  return *this;
}

std::string SorterInterface::getInterfaceName() const
{
  return "SorterInterface";
}

SorterInterface* SorterInterface::clone() const
{
  assert(0);
//return new SorterInterface(*this);
}

/*--------------------------------------------------------------------------*/

void SorterInterface::sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const
{
  _notWritten("sort");
}

void SorterInterface::sort(Alat::IntVector& p) const
{
  _notWritten("sort");
}
