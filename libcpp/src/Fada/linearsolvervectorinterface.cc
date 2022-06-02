#include  "Fada/linearsolvervectorinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

LinearSolverVectorInterface::~LinearSolverVectorInterface()
{}

LinearSolverVectorInterface::LinearSolverVectorInterface() : Alat::InterfaceBase()
{}

LinearSolverVectorInterface::LinearSolverVectorInterface( const LinearSolverVectorInterface& linearsolvervector) : Alat::InterfaceBase(linearsolvervector)
{
  assert(0);
}

LinearSolverVectorInterface& LinearSolverVectorInterface::operator=( const LinearSolverVectorInterface& linearsolvervector)
{
  // LinearSolverInterface::operator=(linearsolvervector);
  assert(0);
  return *this;
}

std::string LinearSolverVectorInterface::getInterfaceName() const
{
  return "LinearSolverVectorInterface";
}

std::string LinearSolverVectorInterface::getName() const
{
  return "LinearSolverVectorInterface";
}

LinearSolverVectorInterface* LinearSolverVectorInterface::clone() const
{
  assert(0);
// return new LinearSolverVectorInterface(*this);
}

/*--------------------------------------------------------------------------*/
// void LinearSolverVectorInterface::restart()
// {
//   _notWritten("restart");
// }
