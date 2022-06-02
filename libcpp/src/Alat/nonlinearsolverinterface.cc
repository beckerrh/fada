#include  "Alat/nonlinearsolverinterface.h"
#include  <cassert>
#include  <iostream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
NonlinearSolverInterface::~NonlinearSolverInterface()
{}
NonlinearSolverInterface::NonlinearSolverInterface() : Alat::InterfaceBase()
{}
NonlinearSolverInterface::NonlinearSolverInterface( const NonlinearSolverInterface& linearsolverinterface) : Alat::InterfaceBase(linearsolverinterface)
{}
NonlinearSolverInterface& NonlinearSolverInterface::operator=( const NonlinearSolverInterface& linearsolverinterface)
{
  InterfaceBase::operator=(linearsolverinterface);
  assert(0);
  return *this;
}
std::string NonlinearSolverInterface::getInterfaceName() const
{
  return "NonlinearSolverInterface";
}

/*--------------------------------------------------------------------------*/
std::ostream& NonlinearSolverInterface::printLoopInformation(std::ostream& os) const
{
  os << "\t" << getName() << "\n";
  return os;
}

/*--------------------------------------------------------------------------*/
Alat::NonlinearSolverVisitorInterface*& NonlinearSolverInterface::newVisitorPointer()
{
  _notWritten("newVisitorPointer");
}

void NonlinearSolverInterface::setVisitorPointer(Alat::NonlinearSolverVisitorInterface* visitor)
{
  _notWritten("setVisitorPointer");
}

/*--------------------------------------------------------------------------*/

int NonlinearSolverInterface::getNVectors() const
{
  _notWritten("getNVectors");
}

/*--------------------------------------------------------------------------*/

std::ostream& NonlinearSolverInterface::write(std::ostream& os) const
{
  _notWritten("write");
  return os;
}

/*--------------------------------------------------------------------------*/

void NonlinearSolverInterface::reInit()
{
  _notWritten("reInit");
}

/*--------------------------------------------------------------------------*/

void NonlinearSolverInterface::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _notWritten("basicInit");
}

void NonlinearSolverInterface::addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const
{
  _notWritten("addUpdate");
}

// const IterationInfo* NonlinearSolverInterface::getIterationInfo() const
// {
//   _notWritten("getIterationInfo");
// }
// 
// IterationInfo* NonlinearSolverInterface::getIterationInfo()
// {
//   _notWritten("getIterationInfo");
// }

double NonlinearSolverInterface::getRhoMatrix() const
{
  _notWritten("getRhoMatrix");
}
