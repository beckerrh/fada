#include  "Fada/multilevelsolverinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

MultiLevelSolverInterface::~MultiLevelSolverInterface()
{}

MultiLevelSolverInterface::MultiLevelSolverInterface()
{}

MultiLevelSolverInterface::MultiLevelSolverInterface( const MultiLevelSolverInterface& multilevelsolverinterface)
{
  assert(0);
}

MultiLevelSolverInterface& MultiLevelSolverInterface::operator=( const MultiLevelSolverInterface& multilevelsolverinterface)
{
  // LinearSolverInterface::operator=(multilevelsolverinterface);
  assert(0);
  return *this;
}

std::string MultiLevelSolverInterface::getInterfaceName() const
{
  return "MultiLevelSolverInterface";
}

std::string MultiLevelSolverInterface::getName() const
{
  return "MultiLevelSolverInterface";
}

MultiLevelSolverInterface* MultiLevelSolverInterface::clone() const
{
  assert(0);
// return new MultiLevelSolverInterface(*this);
}

/*--------------------------------------------------------------------------*/

void MultiLevelSolverInterface::reInit(const Alat::ParameterFile* parameterfile, const Alat::GhostMatrix& ghostmatrix, Fada::DomainSolverInterface* solver)
{
  Alat::InterfaceBase::_notWritten("reInit");
}
