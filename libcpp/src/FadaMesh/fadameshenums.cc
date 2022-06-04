#include  "FadaMesh/fadameshenums.h"
#include  <assert.h>
#include  <iostream>

namespace FadaMeshEnums
{
  std::string meshTypeToString(meshtype m)
  {
    if(m == LineMesh)
    {
      return "LineMesh";
    }
    else if(m == TriangleMesh)
    {
      return "TriangleMesh";
    }
    else if(m == QuadrilateralMesh)
    {
      return "QuadrilateralMesh";
    }
    else if(m == MultilevelTriangleMesh)
    {
      return "MultilevelTriangleMesh";
    }
    else if(m == MultilevelQuadrilateralMesh)
    {
      return "MultilevelQuadrilateralMesh";
    }
    std::cerr<<"****meshTypeToString: meshtype not defined "<<m<<'\n';
    assert(0);
  }
  meshtype stringToMeshType(std::string string)
  {
    if(string == "LineMesh")
    {
      return LineMesh;
    }
    else if(string == "TriangleMesh")
    {
      return TriangleMesh;
    }
    else if(string == "QuadrilateralMesh")
    {
      return QuadrilateralMesh;
    }
    else if(string == "MultilevelTriangleMesh")
    {
      return MultilevelTriangleMesh;
    }
    else if(string == "MultilevelQuadrilateralMesh")
    {
      return MultilevelQuadrilateralMesh;
    }
    std::cerr<<"****stringToMeshType: meshtype not found for "<<string<<'\n';
    assert(0);
  }
}
