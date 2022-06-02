#include  "FadaMesh/geometryobject.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
GeometryObject::~GeometryObject(){}
GeometryObject::GeometryObject() : Alat::InterfaceBase(){}
GeometryObject::GeometryObject( const GeometryObject& geometryobject) : Alat::InterfaceBase(geometryobject)
{
  assert(0);
}
GeometryObject& GeometryObject::operator=( const GeometryObject& geometryobject)
{
  InterfaceBase::operator=(geometryobject);
  assert(0);
  return *this;
}
std::string GeometryObject::getInterfaceName() const
{
  return "GeometryObject";
}

/*--------------------------------------------------------------------------*/

void GeometryObject::constructGeometryObject(const FadaMesh::MeshInterface* mesh)
{
  _notWritten("constructGeometryObject");
}
