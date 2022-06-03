#ifndef  __FadaMesh_enum_h
#define  __FadaMesh_enum_h

#include  <string>

/*---------------------------------------------*/

namespace FadaMeshEnums
{
  enum meshtype {LineMesh, TriangleMesh, QuadrilateralMesh};

  std::string meshTypeToString(meshtype m);
}

#endif
