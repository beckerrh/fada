#include  "Alat/ghostmatrix.h"
#include  "Fada/loopinterface.h"
#include  "Fada/modelmanagerinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LoopInterface::~LoopInterface(){}
LoopInterface::LoopInterface() : Alat::InterfaceBase(){}
LoopInterface::LoopInterface( const LoopInterface& loopinterface) : Alat::InterfaceBase(loopinterface)
{
  assert(0);
}
LoopInterface& LoopInterface::operator=( const LoopInterface& loopinterface)
{
  InterfaceBase::operator=(loopinterface);
  assert(0);
  return *this;
}
std::string LoopInterface::getInterfaceName() const
{
  return "LoopInterface";
}

/*--------------------------------------------------------------------------*/
void LoopInterface::constructMatrixAndLinearSolvers(Alat::GhostMatrix& A,  Alat::GhostVector& u)
{
  _notWritten("constructMatrixAndLinearSolvers");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A,  Alat::GhostVector& u)
{
  _notWritten("constructJacobianMatrix");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::constructLinearSolver(Alat::GhostMatrix& A, const Alat::GhostVector& u)
{
  _notWritten("constructLinearSolver");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostVector& x, const Alat::GhostVector& b)
{
  _notWritten("linearSolve");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  _notWritten("basicInit");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::residual(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x) const
{
  _notWritten("residual");
}

/*--------------------------------------------------------------------------*/

void LoopInterface::nonlinearSolve(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& f, int& nliter, int& liniter)
{
  _notWritten("nonlinearSolve");
}

/*--------------------------------------------------------------------------*/

FadaMesh::MeshCompositionInterface*& LoopInterface::getRefinedMeshCompositionPointer()
{
  _notWritten("getRefinedMeshCompositionPointer");
}

/*--------------------------------------------------------------------------*/

const FadaMesh::MeshCompositionInterface* LoopInterface::getRefinedMeshComposition() const
{
  _notWritten("getRefinedMeshComposition");
}
