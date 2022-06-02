#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
NonlinearSolverVisitorInterface::~NonlinearSolverVisitorInterface()
{}

NonlinearSolverVisitorInterface::NonlinearSolverVisitorInterface() : Alat::InterfaceBase()
{}

NonlinearSolverVisitorInterface::NonlinearSolverVisitorInterface( const NonlinearSolverVisitorInterface& nonlinearsolvervisitorinterface) : Alat::InterfaceBase(nonlinearsolvervisitorinterface)
{
  ( *this ).operator=(nonlinearsolvervisitorinterface);
}

NonlinearSolverVisitorInterface& NonlinearSolverVisitorInterface::operator=( const NonlinearSolverVisitorInterface& nonlinearsolvervisitorinterface)
{
  InterfaceBase::operator=(nonlinearsolvervisitorinterface);
  assert(0);
  return *this;
}

std::string NonlinearSolverVisitorInterface::getName() const
{
  return "NonlinearSolverVisitorInterface";
}

std::string NonlinearSolverVisitorInterface::getInterfaceName() const
{
  return "NonlinearSolverVisitorInterface";
}

NonlinearSolverVisitorInterface* NonlinearSolverVisitorInterface::clone() const
{
  assert(0);
  // return new NonlinearSolverVisitorInterface(*this);
}

/*--------------------------------------------------------------------------*/
void NonlinearSolverVisitorInterface::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _notWritten("basicInit");
}

std::ostream& NonlinearSolverVisitorInterface::printLoopInformation(std::ostream& os) const
{
  _notWritten("printLoopInformation");
}

std::string NonlinearSolverVisitorInterface::getVectorType() const
{
  return "unknowns";
}
