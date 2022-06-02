#ifndef __FadaMesh_GeometryObject_h
#define __FadaMesh_GeometryObject_h

#include  "Alat/interfacebase.h"
// #include  "meshinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshInterface;

  class GeometryObject : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~GeometryObject();
    GeometryObject();
    GeometryObject( const GeometryObject& geometryobject);
    GeometryObject& operator=( const GeometryObject& geometryobject);

    virtual void read(std::string filename) = 0;
    virtual void write(std::string filename, std::string datatype) const = 0;
    virtual void constructGeometryObject(const FadaMesh::MeshInterface* mesh);
  };
}

/*--------------------------------------------------------------------------*/

#endif
