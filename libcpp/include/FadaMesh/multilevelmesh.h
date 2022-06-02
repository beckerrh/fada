#ifndef __FadaMesh_MultiLevelMesh_h
#define __FadaMesh_MultiLevelMesh_h

#include  "meshinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MultiLevelMesh : public FadaMesh::MeshInterface
  {
protected:
    mutable int _activelevel;
    std::string _type;
    Alat::Vector<FadaMesh::MeshInterface*> _meshes;

public:
    ~MultiLevelMesh();
    MultiLevelMesh(std::string type);
    MultiLevelMesh( const MultiLevelMesh& multilevelmesh);
    MultiLevelMesh& operator=( const MultiLevelMesh& multilevelmesh);
    std::string getName() const;
    MultiLevelMesh* clone() const;

    Alat::Vector<FadaMesh::MeshInterface*>& getMeshes();
    std::string getCellType() const;
    int getNLevels() const;
    const FadaMesh::MeshInterface* getMesh(int level) const;
    FadaMesh::MeshInterface* getMesh(int level);
    FadaMesh::MeshInterface*& getMeshPointer(int level);
    std::string getInfo() const;
    int getDimension() const;
    void readFadaMesh(const std::string& basefilename);
    void writeFadaMesh(const std::string& basefilename, std::string type = "binary") const;
    const FadaMesh::BoundaryInfo* getBoundaryInfo() const;
    void setResolution(int level) const;
    int getResolution() const;
    int getNNodes() const;
    int getNEdges() const;
    int getNSides() const;
    int getNCells() const;
    int getNNodesPerCell(int iK) const;
    int getNNodesPerSide(int iS) const;
    int getNSidesPerCell(int iK) const;
    int getNEdgesPerCell(int iK) const;
    int getNEdgesPerSide(int iS) const;
    int getNodeIdOfCell(int i, int ii) const;
    int getNodeIdOfSide(int i, int ii) const;
    int getNodeIdOfEdge(int i, int ii) const;
    int getSideIdOfCell(int i, int ii) const;
    int getEdgeIdOfCell(int i, int ii) const;
    int getEdgeIdOfSide(int i, int ii) const;
    int getCellIdOfSide(int i, int ii) const;
    int getLocalIndexOfSideInCell(int iK, int iS) const;
    void getNodesOfCell(int iK, Alat::Vector<Alat::Node>& F) const;
    // void writeH5(H5::H5File& file) const;
    void writeH5(std::string filename) const;
    void writeMeshInfo(std::string filename) const;
    bool geometryObjectExists(std::string name) const;
    const FadaMesh::GeometryObject* getGeometryObject(std::string name) const;
    bool isMultilevel() const;
    int getCouplingOffset(int iS) const;
    void setVisuType(const std::string& visutype) const;

    Alat::Node getNodeOfCell(int iK) const;
    Alat::Node getNodeOfSide(int iS) const;

    int localIndexOfNodeInCell(int iK, int in) const;
    int localIndexOfSideInCell(int iK, int is) const;
    int localIndexOfEdgeInCell(int iK, int ie) const;
    const Alat::Node& getNodeOfSide(int is, int ii) const;
    const Alat::Node& getNode(int i) const;
    FadaMeshEnums::meshtype getType() const;
    bool cellIsCurved(int iK) const;
    void getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
    void getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
