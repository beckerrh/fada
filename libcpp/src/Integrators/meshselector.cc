#include  "Fada/modelmanagerinterface.h"
#include  "Integrators/meshselector.h"
#include  "FadaMesh/singlemeshcomposition.h"
#include  "FadaMesh/singlemultilevelmeshcomposition.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

MeshSelector::~MeshSelector()
{}
MeshSelector::MeshSelector() : FadaMesh::MeshSelectorInterface()
{}
MeshSelector::MeshSelector( const MeshSelector& meshselector) : FadaMesh::MeshSelectorInterface(meshselector)
{
  assert(0);
}

MeshSelector& MeshSelector::operator=( const MeshSelector& meshselector)
{
  FadaMesh::MeshSelectorInterface::operator=(meshselector);
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
    assert(0);
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
