#include  "Fada/modelinterface.h"
#include  "Integrators/meshselector.h"
#include  "FadaMesh/singlemeshcomposition.h"
#include  "FadaMesh/singlemultilevelmeshcomposition.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

MeshSelector::~MeshSelector()
{}
MeshSelector::MeshSelector() : Alat::InterfaceBase()
{}
MeshSelector::MeshSelector( const MeshSelector& meshselector) : Alat::InterfaceBase(meshselector)
{
  assert(0);
}

MeshSelector& MeshSelector::operator=( const MeshSelector& meshselector)
{
  Alat::InterfaceBase::operator=(meshselector);
  assert(0);
  return *this;
}

std::string MeshSelector::getName() const
{
  return "MeshSelector";
}

/*--------------------------------------------------------------------------*/

FadaMesh::MeshCompositionInterface* MeshSelector::newMeshComposition(std::string meshtype, std::string meshnamebase) const
{
  FadaMesh::MeshCompositionInterface* meshcomposition;
  if(meshtype == "unstructured")
  {
    meshcomposition = new FadaMesh::SingleMeshComposition;
    meshcomposition->constructFadaMesh(meshnamebase);
  }
  else if(meshtype == "unstructured_multilevel")
  {
    meshcomposition = new FadaMesh::SingleMultiLevelMeshComposition;
    meshcomposition->constructFadaMesh(meshnamebase);
  }
  else
  {
    _error_string("newMeshComposition", "unknown meshtype", meshtype);
  }
  meshcomposition->read(meshnamebase);
  return meshcomposition;
}
