#ifndef __FadaMesh_GeometryObjectsConstructorInterface_h
#define __FadaMesh_GeometryObjectsConstructorInterface_h

#include  "Alat/interfacebase.h"
#include  <map>

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class GeometryObject;
  class GeometryObjectsConstructorInterface : public Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const
    {
      return "GeometryObjectsConstructorInterface";
    }

public:
    ~GeometryObjectsConstructorInterface();
    GeometryObjectsConstructorInterface();
    GeometryObjectsConstructorInterface( const GeometryObjectsConstructorInterface& geometryobjectsconstructorinterface);
    GeometryObjectsConstructorInterface& operator=( const GeometryObjectsConstructorInterface& geometryobjectsconstructorinterface);
    virtual FadaMesh::GeometryObject* newGeometryObject( const std::string& name ) const=0;
virtual void constructGeometryObject(std::map<std::string, FadaMesh::GeometryObject*>& geo, const std::string& name) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
