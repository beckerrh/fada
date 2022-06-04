#include  "Fada/modelinterface.h"
#include  "FadaMesh/meshselector.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/trianglemesh.h"
#include  "FadaMesh/multilevelmesh.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

MeshSelector::~MeshSelector()
{
}

MeshSelector::MeshSelector() : Alat::InterfaceBase()
{
}

MeshSelector::MeshSelector(const MeshSelector& meshselector) : Alat::InterfaceBase(meshselector)
{
   assert(0);
}

MeshSelector& MeshSelector::operator=(const MeshSelector& meshselector)
{
   Alat::InterfaceBase::operator=(meshselector);

   assert(0);
   return(*this);
}

std::string MeshSelector::getName() const
{
   return("MeshSelector");
}

/*--------------------------------------------------------------------------*/

FadaMesh::MeshInterface * MeshSelector::newMesh(FadaMeshEnums::meshtype& meshtype, std::string meshnamebase) const
{
   FadaMesh::MeshInterface *mesh;
   if (meshtype == FadaMeshEnums::QuadrilateralMesh)
   {
      mesh = new FadaMesh::QuadrilateralMesh;
   }
   else if (meshtype == FadaMeshEnums::TriangleMesh)
   {
      mesh = new FadaMesh::TriangleMesh;
   }
   else if (meshtype == FadaMeshEnums::MultilevelQuadrilateralMesh)
   {
     mesh = new FadaMesh::MultiLevelMesh("FadaMesh::QuadrilateralMesh");
   }
   else if (meshtype == FadaMeshEnums::MultilevelTriangleMesh)
   {
     mesh = new FadaMesh::MultiLevelMesh("FadaMesh::QuadToTri");
   }
   else
   {
      _error_string("newMesh", "unknown meshtype: " + meshtype);
   }
   mesh->readFadaMesh(meshnamebase);
   return mesh;
}
