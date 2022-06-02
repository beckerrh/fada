#include  "Fada/dofinformationinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

DofInformationInterface::~DofInformationInterface()
{}

/*--------------------------------------------------------------------------*/

DofInformationInterface::DofInformationInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

DofInformationInterface::DofInformationInterface( const DofInformationInterface& dofinformationinterface) : Alat::InterfaceBase(dofinformationinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

DofInformationInterface& DofInformationInterface::operator=( const DofInformationInterface& dofinformationinterface)
{
  InterfaceBase::operator=(dofinformationinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string DofInformationInterface::getInterfaceName() const
{
  return "DofInformationInterface";
}

/*--------------------------------------------------------------------------*/

std::string DofInformationInterface::getName() const
{
  return "DofInformationInterface";
}

/*--------------------------------------------------------------------------*/

DofInformationInterface* DofInformationInterface::clone() const
{
  assert(0);
// return new DofInformationInterface(*this);
}

void DofInformationInterface::setDofIdsOnSide(int iS, Alat::IntVector& ids) const
{
  _notWritten("setDofIdsOnSide");
}
void DofInformationInterface::getCellIndicesInterpolate(int iK, Alat::IntVector& indices) const
{
  getCellIndices(iK, indices);
}
