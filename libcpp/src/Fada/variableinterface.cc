#include  "Fada/feminterface.h"
#include  "Fada/variableinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

VariableInterface::~VariableInterface()
{}

/*--------------------------------------------------------------------------*/

VariableInterface::VariableInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

VariableInterface::VariableInterface( const VariableInterface& variableinterface) : Alat::InterfaceBase(variableinterface)
{
}

/*--------------------------------------------------------------------------*/

VariableInterface& VariableInterface::operator=( const VariableInterface& variableinterface)
{
  InterfaceBase::operator=(variableinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string VariableInterface::getInterfaceName() const
{
  return "VariableInterface";
}

/*--------------------------------------------------------------------------*/

int VariableInterface::getN() const
{
  _notWritten("getN");
}

//
// /*--------------------------------------------------------------------------*/
//
// int VariableInterface::getNDofPerComponent() const
// {
//   return getFem()->ndof();
// }
//
// /*--------------------------------------------------------------------------*/
//
// std::string VariableInterface::getFemName() const
// {
//   return getFem()->getName();
// }
