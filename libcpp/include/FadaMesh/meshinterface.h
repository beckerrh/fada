#ifndef __FadaMesh_MeshInterface_h
#define __FadaMesh_MeshInterface_h

#include  "Alat/interfacebase.h"
#include  "Alat/node.h"
#include  "Alat/sparsitypatternfixarray.h"
#include  "enums.h"

/*--------------------------------------------------------------------------*/
namespace H5
{
  class H5File;
}
namespace Alat
{
  class StringVector;
}
namespace FadaMesh
{
  class BoundaryInfo;
  class GeometryObject;
  class GeometryObjectsConstructorInterface;
  class SparsityPattern;

  class MeshInterface : public Alat::InterfaceBase
  {
private:
    static Alat::Node _dummynode;
    GeometryObjectsConstructorInterface* dummy_geometryobjects_constructor;

protected:
    std::string getInterfaceName() const;

public:
    ~MeshInterface();
    MeshInterface();
    MeshInterface( const MeshInterface& meshinterface);
    MeshInterface& operator=( const MeshInterface& meshinterface);

    virtual const std::string& getInfileName() const;
    virtual void setResolution(int level) const;
    virtual int getResolution() const;
    virtual int getNLevels() const;
    virtual std::string getInfo() const = 0;
    virtual void getNodesOfCell(int iK, Alat::Vector<Alat::Node>& F) const;
    virtual int getDimension() const = 0;
    virtual Alat::Node getNodeOfSide(int iS) const;
    virtual int getNodeIdOfSideOfCell(int iK, int iis, int ii) const;
    virtual Alat::Node getNodeOfCell(int iK) const;
    virtual Alat::Node getNodeOfEdge(int iK) const;
    virtual int getNNodes() const;
    virtual int getNEdges() const;
    virtual int getNSides() const;
    virtual int getNCells() const;
    virtual const Alat::Node& getNode(int i) const;
    virtual const Alat::Node& getNodeOfCell(int i, int ii) const;
    virtual const Alat::Node& getNodeOfEdge(int i, int ii) const;
    virtual const Alat::Node& getNodeOfSide(int i, int ii) const;

    virtual Alat::Vector<Alat::Node>& getAllNodes();

    virtual std::string getCellType() const;
    virtual int getNNodesPerCell(int iK) const;
    virtual int getNNodesPerSide(int iS) const;
    virtual int getNSidesPerCell(int iK) const;
    virtual int getNEdgesPerCell(int iK) const;
    virtual int getNEdgesPerSide(int iS) const;
    virtual const FadaMesh::BoundaryInfo* getBoundaryInfo() const;
    virtual int getNodeIdOfCell(int i, int ii) const;
    virtual int getNodeIdOfSide(int i, int ii) const;
    virtual int getNodeIdOfEdge(int i, int ii) const;
    virtual int getSideIdOfCell(int i, int ii) const;
    virtual int getEdgeIdOfCell(int i, int ii) const;
    virtual int getEdgeIdOfSide(int i, int ii) const;
    virtual int getCellIdOfSide(int i, int ii) const;
    virtual int getLocalIndexOfSideInCell(int iK, int iS) const;
    virtual bool geometryObjectExists(std::string name) const;
    virtual const FadaMesh::GeometryObject* getGeometryObject(std::string name) const;
    virtual FadaMesh::GeometryObject* getGeometryObject(std::string name);
    virtual void createGeometryObject(std::string name);
    virtual void read(std::string filename);
    virtual void write(std::string filename) const;
    virtual void readFadaMesh(const std::string& basefilename);
    virtual void writeFadaMesh(const std::string& basefilename, std::string type = "binary") const;
    virtual void writeVtk(std::string filename) const;
    virtual void writeBoundaryVtk(std::string filename) const;
    virtual void writeMeshInfo(std::string filename) const;
    // virtual void writeH5(H5::H5File& file) const;
    virtual void writeH5(std::string filename) const;
    virtual void setVisuType(const std::string& visutype) const = 0;

    virtual int getVtkType() const;
    virtual int getBoundaryVtkType() const;
    virtual bool cellIsCurved(int iK) const;
    virtual int findNeightborHangingCells(int iK, int iS, Alat::Node pt);
    virtual int getLocalNodeIndiceOfSide(int ii, int isl) const;
    virtual int getCouplingOffset(int iS) const;
    virtual bool isMultilevel() const = 0;

    virtual int localIndexOfNodeInCell(int iK, int in) const;
    virtual int localIndexOfSideInCell(int iK, int is) const;
    virtual int localIndexOfEdgeInCell(int iK, int ie) const;

    virtual FadaMeshEnums::meshtype getType() const;
    virtual void getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
    virtual void getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
