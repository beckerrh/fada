#include  "Alat/vectorinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
VectorInterface::~VectorInterface(){}
VectorInterface::VectorInterface() : Alat::InterfaceBase(){}
VectorInterface::VectorInterface( const Alat::VectorInterface& vectorinterface) : Alat::InterfaceBase(vectorinterface){}
VectorInterface* VectorInterface::clone() const
{
  _notWritten("clone");
  return NULL;
}
std::string VectorInterface::getInterfaceName() const
{
  return "VectorInterface";
}
VectorInterface& VectorInterface::operator=( const Alat::VectorInterface& vectorinterface)
{
  InterfaceBase::operator=(vectorinterface);
  assert(0);
  return *this;
}
/*--------------------------------------------------------------------------*/
double VectorInterface::norm() const{_notWritten("norm");}
double VectorInterface::scalarProduct(const Alat::VectorInterface* v) const{_notWritten("scalarProduct");}
void VectorInterface::equal(const Alat::VectorInterface* v){_notWritten("equal");}
void VectorInterface::equal(double d){_notWritten("equal");}
void VectorInterface::add(const double& d, const Alat::VectorInterface* v){_notWritten("add");}
void VectorInterface::scale(const double& d){_notWritten("scale");}
void VectorInterface::zeros(){_notWritten("zeros");}
