#include  "FadaMesh/meshcompositioninterface.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

MeshCompositionInterface::~MeshCompositionInterface()
{}

/*--------------------------------------------------------------------------*/

MeshCompositionInterface::MeshCompositionInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

MeshCompositionInterface::MeshCompositionInterface( const MeshCompositionInterface& meshcompositioninterface) : Alat::InterfaceBase(meshcompositioninterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

MeshCompositionInterface& MeshCompositionInterface::operator=( const MeshCompositionInterface& meshcompositioninterface)
{
  InterfaceBase::operator=(meshcompositioninterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string MeshCompositionInterface::getInterfaceName() const
{
  return "MeshCompositionInterface";
}

/*--------------------------------------------------------------------------*/

std::string MeshCompositionInterface::getName() const
{
  return "MeshCompositionInterface";
}

/*--------------------------------------------------------------------------*/

const FadaMesh::MeshInterface* MeshCompositionInterface::getMacroMesh() const
{
  _notWritten("getMacroMesh");
  return NULL;
}

/*--------------------------------------------------------------------------*/

MeshCompositionInterface* MeshCompositionInterface::clone() const
{
  assert(0);
// return new MeshCompositionInterface(*this);
}

/*--------------------------------------------------------------------------*/

void MeshCompositionInterface::writeMeshInfo(std::string filename) const
{
  _notWritten("writeMeshInfo");
}

// /*--------------------------------------------------------------------------*/
//
// void MeshCompositionInterface::initCouplingGrids(const Alat::DoubleVectorModelManagerInterface* _modelmanager)
// {
// _notWritten("initCouplingGrids");
// }

/*--------------------------------------------------------------------------*/

void MeshCompositionInterface::constructFadaMesh(const std::string& meshname)
{
  _notWritten("constructFadaMesh");
}
