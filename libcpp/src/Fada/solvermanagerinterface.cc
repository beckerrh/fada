#include  "Alat/ghostmatrix.h"
#include  "Fada/solvermanagerinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

SolverManagerInterface::~SolverManagerInterface()
{}

/*--------------------------------------------------------------------------*/

SolverManagerInterface::SolverManagerInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

SolverManagerInterface::SolverManagerInterface( const SolverManagerInterface& solvermanagerinterface) : Alat::InterfaceBase(solvermanagerinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

SolverManagerInterface& SolverManagerInterface::operator=( const SolverManagerInterface& solvermanagerinterface)
{
  InterfaceBase::operator=(solvermanagerinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string SolverManagerInterface::getInterfaceName() const
{
  return "SolverManagerInterface";
}

/*--------------------------------------------------------------------------*/

void SolverManagerInterface::constructLinearSolver(Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& u)
{
  _notWritten("constructLinearSolver");
}

// /*--------------------------------------------------------------------------*/
// 
// void SolverManagerInterface::computeLinearizationPoint(Alat::GhostVector& u) const
// {
//   _notWritten("computeLinearizationPoint");
// }

/*--------------------------------------------------------------------------*/

void SolverManagerInterface::setLavrentievParameter(double parameter) const
{
  _notWritten("setLavrentievParameter");
}

/*--------------------------------------------------------------------------*/

void SolverManagerInterface::reInitForInterpolation(const FadaMesh::MeshCompositionInterface* meshcomposition)
{
  _notWritten("reInitForInterpolation");
}

/*--------------------------------------------------------------------------*/

void SolverManagerInterface::integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const
{
  _notWritten("integrateInTimePostProcess");
}

void SolverManagerInterface::integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const
{
  _notWritten("integrateInTimePostProcess");
}
void SolverManagerInterface::constructVectorOfDomains(const Alat::GhostVector& gu) const
{}
