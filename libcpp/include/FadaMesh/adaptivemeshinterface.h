#ifndef __FadaMesh_AdaptiveMeshInterface_h
#define __FadaMesh_AdaptiveMeshInterface_h

#include  "geometryobject.h"
#include  "faceinterface.h"
#include  "Alat/map.h"
#include  "Alat/node.h"
#include  "edge.h"
#include  "typedefs.h"
#include  "setdefs.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class AdaptiveMeshInterface: public Alat::InterfaceBase
  {

public:

    std::string getInterfaceName() const
    {
      return "AdaptiveMeshInterface";
    }
    virtual void readAdaptiveMesh(std::string name,std::string last_in)=0;
    virtual void writeAdaptiveMesh(std::string name, std::string datatype = "binary")=0;
    virtual void readFadaMeshAndInitTrees(std::string name)=0;
    virtual void writeFadaMesh(std::string name, std::string datatype)=0;
    virtual void constructNumbering()=0;
    virtual void constructCellMap()=0;
    virtual void updateHangingInfo()=0;
    virtual void reInitFadaMesh()=0;
    virtual NodeSet & getNodes()=0;
    virtual tree<Edge*> & getEdges()=0;
    virtual tree<FaceInterface*> & getFaces()=0;
    virtual tree<VolumeInterface*> & getVolumes()=0;
    virtual int& getLastNodeId()=0;
    virtual int& getLastEdgeId()=0;
    virtual int& getLastFaceId()=0;
    virtual int& getLastVolumeId()=0;
    virtual Alat::IntMap& getNodeId2Id()=0;
    virtual Alat::IntMap& getEdgeId2Id()=0;
    virtual Alat::IntMap& getFaceId2Id()=0;
    virtual Alat::IntMap& getVolumeId2Id()=0;
    virtual std::map<int, face_pointer>&  getCellMap2d() =0;
    virtual std::map<int, volume_pointer>&  getCellMap3d() =0;
    virtual bool&  getCellMapOk()=0;
    virtual bool&  getNumberingOk()=0;
    virtual FadaMesh::GeometryObject* getGeometryObject(std::string name) =0;
    virtual bool geometryObjectExists(std::string name) const =0;
    virtual void createGeometryObject(std::string name) =0;
    virtual void writeVtk(std::string name) =0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
