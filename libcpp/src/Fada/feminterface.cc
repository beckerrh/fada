#include  "Fada/feminterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

FemInterface::~FemInterface()
{}

FemInterface::FemInterface() : Alat::InterfaceBase()
{}

FemInterface::FemInterface( const FemInterface& finiteelementinterface) : Alat::InterfaceBase(finiteelementinterface)
{}

std::string FemInterface::getInterfaceName() const
{
  return "FemInterface";
}

FemInterface* FemInterface::clone() const
{
  _notWritten("clone");
}

FemInterface& FemInterface::operator=( const FemInterface& finiteelementinterface)
{
  InterfaceBase::operator=(finiteelementinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/
void FemInterface::getReferenceCenterPoint(Alat::Node& xhat) const
{
  _notWritten("getReferenceCenterPoint");
}

/*--------------------------------------------------------------------------*/

TransformationInterface* FemInterface::newTransformationCurved() const
{
  return newTransformation();
}

Fada::IntegrationFormulaInterface* FemInterface::newIntegrationFormulaTransfer() const
{
  _notWritten("newIntegrationFormulaTransfer");
  return NULL;
}
/*--------------------------------------------------------------------------*/
bool FemInterface::interpolationMatrixWrong() const
{
  return false;
}
void FemInterface::interpolate(Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const
{
  _notWritten("interpolate");  
}
