#include  "Alat/iterationinfo.h"
#include  "Alat/linearsolverinterface.h"
#include  <cassert>
#include  <iostream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
LinearSolverInterface::~LinearSolverInterface()
{}
LinearSolverInterface::LinearSolverInterface() : Alat::InterfaceBase()
{}
LinearSolverInterface::LinearSolverInterface( const LinearSolverInterface& linearsolverinterface) : Alat::InterfaceBase(linearsolverinterface)
{}
LinearSolverInterface& LinearSolverInterface::operator=( const LinearSolverInterface& linearsolverinterface)
{
  InterfaceBase::operator=(linearsolverinterface);
  assert(0);
  return *this;
}

std::ostream& LinearSolverInterface::printLoopInformation(std::ostream& os) const
{
  os << "\t" << getName() << "\n";
  return os;
}

std::string LinearSolverInterface::getInterfaceName() const
{
  return "LinearSolverInterface";
}

/*--------------------------------------------------------------------------*/
int LinearSolverInterface::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
std::ostream& LinearSolverInterface::write(std::ostream& os) const
{
  _notWritten("write");
  return os;
}

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _notWritten("basicInit");
}

void LinearSolverInterface::reInit()
{
  _notWritten("reInit");
}

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::compute()
{
  _notWritten("compute");
}

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _notWritten("solve");
}

// void LinearSolverInterface::solve(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const
// {
//   _notWritten("solve");
// }

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const
{
  _notWritten("addUpdate");
}

const IterationInfo* LinearSolverInterface::getIterationInfo() const
{
  _notWritten("getIterationInfo");
}

IterationInfo* LinearSolverInterface::getIterationInfo()
{
  _notWritten("getIterationInfo");
}

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::restart()
{
  // _notWritten("restart");
}

/*--------------------------------------------------------------------------*/
void LinearSolverInterface::setTolerance(double rtol, double gtol)
{
  getIterationInfo()->getRTol() = rtol;
  getIterationInfo()->getGlobalTol() = gtol;
}

int LinearSolverInterface::getNumberOfIterations() const
{
  return getIterationInfo()->getNumberOfIterations();
}
