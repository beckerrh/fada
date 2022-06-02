#include  "Alat/preconditionerinterface.h"
#include  "Alat/stringvector.h"
#include  <cassert>
#include  <iostream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
PreconditionerInterface::~PreconditionerInterface(){}
PreconditionerInterface::PreconditionerInterface() : Alat::InterfaceBase(){}
PreconditionerInterface::PreconditionerInterface( const PreconditionerInterface& vectorinterface) : Alat::InterfaceBase(vectorinterface)
{
  assert(0);
}
PreconditionerInterface& PreconditionerInterface::operator=( const PreconditionerInterface& vectorinterface)
{
  InterfaceBase::operator=(vectorinterface);
  assert(0);
  return *this;
}
std::string PreconditionerInterface::getInterfaceName() const
{
  return "PreconditionerInterface";
}
std::string PreconditionerInterface::getName() const
{
  return "PreconditionerInterface";
}

std::ostream& PreconditionerInterface::printLoopInformation(std::ostream& os) const
{
  os << getName();
  return os;
}                                                                             

Alat::IterativeSolverVisitorInterface* PreconditionerInterface::getVisitor() {_notWritten("getVisitor");}
const Alat::IterativeSolverVisitorInterface* PreconditionerInterface::getVisitor() const {_notWritten("getVisitor");}

/*--------------------------------------------------------------------------*/
void PreconditionerInterface::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  _notWritten("basicInit");
}

void PreconditionerInterface::reInit()
{
  _notWritten("reInit");
}

void PreconditionerInterface::computePreconditioner()
{
  _notWritten("computePreconditioner");
}

void PreconditionerInterface::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  _notWritten("solveApproximate");
}

std::ostream& PreconditionerInterface::write(std::ostream& os) const
{
  _notWritten("write");
}

void PreconditionerInterface::zeros() const
{
  _notWritten("zero");
}
