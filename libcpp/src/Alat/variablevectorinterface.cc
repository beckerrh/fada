#include  "Alat/variablevectorinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableVectorInterface::~VariableVectorInterface(){}
VariableVectorInterface::VariableVectorInterface() : Alat::InterfaceBase(){}
VariableVectorInterface::VariableVectorInterface( const Alat::VariableVectorInterface& vectorinterface) : Alat::InterfaceBase(vectorinterface){}
VariableVectorInterface* VariableVectorInterface::clone() const
{
  _notWritten("clone");
  return NULL;
}
std::string VariableVectorInterface::getInterfaceName() const
{
  return "VariableVectorInterface";
}

/*--------------------------------------------------------------------------*/
VariableVectorInterface& Alat::VariableVectorInterface::operator=( const Alat::VariableVectorInterface& vectorinterface)
{
  InterfaceBase::operator=(vectorinterface);
  assert(0);
  return *this;
}


/*--------------------------------------------------------------------------*/
void VariableVectorInterface::set(const Alat::AssembleVector& vloc, const Alat::IntVector& indices)
{
  _notWritten("set");
}
void VariableVectorInterface::scaleIntVector(const Alat::IntVector& d)
{
  _notWritten("scaleIntVector(const Alat::IntVector & d)");
}

/*--------------------------------------------------------------------------*/
void VariableVectorInterface::assemble(const Alat::AssembleVector& vloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale, double d)
{
  _notWritten("assemble");
}

/*--------------------------------------------------------------------------*/
void VariableVectorInterface::extract(Alat::AssembleVector& uloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale) const
{
  _notWritten("extract");
}

/*--------------------------------------------------------------------------*/
void VariableVectorInterface::addVectorRhsForDirectSolver(int offset, Alat::DoubleVector& f) const
{
  _notWritten("addVectorRhsForDirectSolver");
}

void VariableVectorInterface::setSingleVectorFromDirectSolver(int offset, const Alat::DoubleVector& u)
{
  _notWritten("setSingleVectorFromDirectSolver");
}

/*--------------------------------------------------------------------------*/
int VariableVectorInterface::getNComponents() const
{
  _notWritten("getNComponents");
}

/*--------------------------------------------------------------------------*/
std::ostream& VariableVectorInterface::writeAscii(std::ostream& s) const
{
  _notWritten("writeAscii");
}
