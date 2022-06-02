#ifndef __FadaMesh_GeometryObjectsConstructor_h
#define __FadaMesh_GeometryObjectsConstructor_h

#include  "geometryobjectsconstructorinterface.h"
#include  "Alat/interfacebase.h"
#include  <map>
#include  <string>

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class GeometryObjectsConstructor : public virtual FadaMesh::GeometryObjectsConstructorInterface
  {
public:
	GeometryObjectsConstructor();
    std::string getName() const;
FadaMesh::GeometryObject* newGeometryObject( const std::string& name ) const;
   void constructGeometryObject(std::map<std::string, FadaMesh::GeometryObject*>& geo, const std::string& name) const;
   };
}

/*--------------------------------------------------------------------------*/

#endif
