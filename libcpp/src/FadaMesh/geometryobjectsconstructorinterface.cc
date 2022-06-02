#include  "FadaMesh/geometryobjectsconstructorinterface.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

GeometryObjectsConstructorInterface::~GeometryObjectsConstructorInterface()
{}

/*--------------------------------------------------------------------------*/

GeometryObjectsConstructorInterface::GeometryObjectsConstructorInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

GeometryObjectsConstructorInterface::GeometryObjectsConstructorInterface( const GeometryObjectsConstructorInterface& geometryobjectsconstructorinterface) : Alat::InterfaceBase(geometryobjectsconstructorinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

GeometryObjectsConstructorInterface& GeometryObjectsConstructorInterface::operator=( const GeometryObjectsConstructorInterface& geometryobjectsconstructorinterface)
{
  InterfaceBase::operator=(geometryobjectsconstructorinterface);
  assert(0);
  return *this;
}
