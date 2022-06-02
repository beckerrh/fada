#include  "FadaMesh/coarseninfo.h"
#include  "FadaMesh/curvedboundaryinformation.h"
#include  "FadaMesh/geometryobjectsconstructor.h"
#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/hangingsideinfo.h"
#include  "FadaMesh/refineinfo.h"
#include  <string>

using namespace FadaMesh;

 /*--------------------------------------------------------------------------*/
 GeometryObjectsConstructor::GeometryObjectsConstructor() : FadaMesh::GeometryObjectsConstructorInterface() {}
 std::string GeometryObjectsConstructor::getName() const
    {
      return "GeometryObjectsConstructor";
    }

/*--------------------------------------------------------------------------*/
void FadaMesh::GeometryObjectsConstructor::constructGeometryObject(std::map<std::string, FadaMesh::GeometryObject*>& geo, const std::string& name ) const
{
  geo[name] = newGeometryObject(name);
}

/*--------------------------------------------------------------------------*/
FadaMesh::GeometryObject* GeometryObjectsConstructor::newGeometryObject( const std::string& name ) const
{

  if(name == "HangingSideInfo")
  {
    return  new FadaMesh::HangingSideInfo;
  }
  else if(name == "HangingNodeInfo")
  {
    return new FadaMesh::HangingNodeInfo;
  }
  else if(name == "RefineInfo")
  {
    return  new FadaMesh::RefineInfo;
  }
  else if(name == "CoarsenInfo")
  {
    return  new FadaMesh::CoarsenInfo;
  }
  else if(name == "CurvedBoundaryInformation")
  {
    return new FadaMesh::CurvedBoundaryInformation;
  }
    _notWritten("createGeometryObject("+name+")");
}
