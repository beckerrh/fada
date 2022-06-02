#include  "Alat/ghostmatrix.h"
#include  "Fada/solverinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

SolverInterface::~SolverInterface()
{}

/*--------------------------------------------------------------------------*/

SolverInterface::SolverInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

SolverInterface::SolverInterface( const SolverInterface& solverinterface) : Alat::InterfaceBase(solverinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

SolverInterface& SolverInterface::operator=( const SolverInterface& solverinterface)
{
  InterfaceBase::operator=(solverinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string SolverInterface::getInterfaceName() const
{
  return "SolverInterface";
}

/*--------------------------------------------------------------------------*/

void SolverInterface::setLavrentievParameter(double parameter) const
{
  _notWritten("setLavrentievParameter");
}
// 
// /*--------------------------------------------------------------------------*/
// 
// void SolverInterface::matrixVectorProductMass(Alat::GhostVector& gf, const Alat::GhostVector& gu, double d) const
// {
//   _notWritten("matrixVectorProductMass");
// }

/*--------------------------------------------------------------------------*/

void SolverInterface::restrict(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf ) const
{
  _notWritten("restrict");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::prolongate(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf, double d) const
{
  _notWritten("prolongate");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::reInitForInterpolation(const FadaMesh::MeshInterface* mesh)
{
  _notWritten("reInitForInterpolation");
}

/*--------------------------------------------------------------------------*/

DofInformationInterface* SolverInterface::newDofInformation(const Fada::FemInterface* fem) const
{
  _notWritten("newDofInformation");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::setMesh(const FadaMesh::MeshInterface* mesh) const
{
  _notWritten("setMesh");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const
{
  _notWritten("integrateInTimePostProcess");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::reInitVariableMatrix(int level, Alat::VariableMatrixInterface* matrix, const Fada::VariableInterface* vari, const Fada::VariableInterface* varj) const
{
  _notWritten("reInitVariableMatrix");
}

/*--------------------------------------------------------------------------*/

void SolverInterface::postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level) const
{
  _notWritten("postProcess");
}
//
// /*--------------------------------------------------------------------------*/
//
// void SolverInterface::reInitInterpolation()
// {}

/*--------------------------------------------------------------------------*/
std::string SolverInterface::getCouplingType() const
{
  _notWritten("postProcess");
  
}
