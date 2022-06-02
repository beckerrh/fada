#include  "Alat/iterationinfo.h"
#include  "Fada/linearsolvervariableinterface.h"
#include  <cassert>
#include  <iostream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearSolverVariableInterface::~LinearSolverVariableInterface()
{}
LinearSolverVariableInterface::LinearSolverVariableInterface() : Alat::LinearSolverInterface()
{}
LinearSolverVariableInterface::LinearSolverVariableInterface( const LinearSolverVariableInterface& linearsolverinterface) : Alat::LinearSolverInterface(linearsolverinterface)
{}
LinearSolverVariableInterface& LinearSolverVariableInterface::operator=( const LinearSolverVariableInterface& linearsolverinterface)
{
  Alat::LinearSolverInterface::operator=(linearsolverinterface);
  assert(0);
  return *this;
}
std::string LinearSolverVariableInterface::getInterfaceName() const {return "LinearSolverVariableInterface::";}

/*--------------------------------------------------------------------------*/
void LinearSolverVariableInterface::solve(AlatEnums::iterationstatus& status, const Alat::VariableMatrixInterface* A, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const
{
  _notWritten("solve");
}
