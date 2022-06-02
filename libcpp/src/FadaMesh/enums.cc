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
    else if(m == HexahedralMesh)
    {
      return "HexahedralMesh";
    }
    else if(m == TetrahedralMesh)
    {
      return "TetrahedralMesh";
    }
    else if(m == CouplingMesh2D)
    {
      return "CouplingMesh2D";
    }
    else if(m == CouplingMesh3D)
    {
      return "CouplingMesh3D";
    }
  }
}
