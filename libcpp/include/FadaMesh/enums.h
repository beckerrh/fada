#ifndef  __FadaMesh_enum_h
#define  __FadaMesh_enum_h

#include  <string>

/*---------------------------------------------*/

namespace FadaMeshEnums
{
  enum meshtype {LineMesh, TriangleMesh, QuadrilateralMesh, HexahedralMesh, TetrahedralMesh, CouplingMesh2D,CouplingMesh3D};

  std::string meshTypeToString(meshtype m);
}

#endif
