#include  "FadaMesh/enums.h"
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
    return "not found";
  }
}
