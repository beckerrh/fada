#include  "Alat/iterationinfo.h"
#include  "Fada/linearsolversysteminterface.h"
#include  <cassert>
#include  <iostream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearSolverSystemInterface::~LinearSolverSystemInterface()
{}
LinearSolverSystemInterface::LinearSolverSystemInterface() : Alat::LinearSolverInterface()
{}
LinearSolverSystemInterface::LinearSolverSystemInterface( const LinearSolverSystemInterface& linearsolverinterface) : Alat::LinearSolverInterface(linearsolverinterface)
{}
LinearSolverSystemInterface& LinearSolverSystemInterface::operator=( const LinearSolverSystemInterface& linearsolverinterface)
{
  Alat::LinearSolverInterface::operator=(linearsolverinterface);
  assert(0);
  return *this;
}
std::string LinearSolverSystemInterface::getInterfaceName() const {return "LinearSolverSystemInterface::";}

/*--------------------------------------------------------------------------*/
void LinearSolverSystemInterface::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _notWritten("solve");
}
