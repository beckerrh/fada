#include  "Alat/matrixinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
MatrixInterface::~MatrixInterface(){}
MatrixInterface::MatrixInterface() : Alat::InterfaceBase(){}
MatrixInterface::MatrixInterface( const MatrixInterface& variablematrixinterface) : Alat::InterfaceBase(variablematrixinterface){}
MatrixInterface& MatrixInterface::operator=( const MatrixInterface& variablematrixinterface)
{
  InterfaceBase::operator=(variablematrixinterface);
  assert(0);
  return *this;
}
std::string MatrixInterface::getName() const
{
  return "MatrixInterface";
}
std::string MatrixInterface::getInterfaceName() const
{
  return "MatrixInterface";
}
MatrixInterface* MatrixInterface::clone() const
{
  assert(0);
//return new MatrixInterface(*this);
}
