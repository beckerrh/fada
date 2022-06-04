#ifndef __FadaMesh_FadaMeshBase_h
#define __FadaMesh_FadaMeshBase_h

#include  "boundaryinfo.h"
#include  "geometryobjectsconstructorinterface.h"
#include  "meshinterface.h"
#include  <sstream>

/*---------------------------------------------------------*/

namespace FadaMesh
{
  template<int N>
  class CellBase : public Alat::FixArray<N, int>
  {
public:
    int node(int i) const;
  };

  /*---------------------------------------------------------*/


  template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
  class FadaMeshBase : public virtual FadaMesh::MeshInterface
  {
public:
    typedef CellBase<NODESPERCELL> Cell;
    typedef CellBase<NODESPERSIDE> Side;
    typedef CellBase<SIDESPERCELL> SideCell;
    typedef CellBase<2> CellSide;
    typedef Alat::Map<Side, int> BoundarySideToColor;

protected:
    mutable std::string _visutype;
    static Side _dummyside;
    std::string _infilename;
    Alat::Vector<Alat::Node>   _nodes;
    Alat::Vector<Cell>   _cells;
    Alat::Vector<Side>   _sides;
    Alat::Vector<SideCell>   _sides_of_cells;
    Alat::Vector<CellSide>   _cells_of_sides;

    FadaMesh::BoundaryInfo _boundaryinfo;

    FadaMesh::GeometryObjectsConstructorInterface* _geometryobjects_constructor;
    std::map<std::string, FadaMesh::GeometryObject*>   _geometryobjects;

    //! this function is need to construct the side information from _cells
    virtual const Side& _getSideOfCell(int i, int ii) const;

    void checkGeoFile(std::string filename) const;

public:
    FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>( );
    FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>( const FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>&fadameshbase );
    ~FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>( );

    const std::string& getInfileName() const;
    void setVisuType(const std::string& visutype) const;
    int getNLevels() const;
    void setResolution(int level) const;
    int getResolution() const;
    void getNodesOfCell(int iK, Alat::Vector<Alat::Node>& F) const;

    // void reInit();
    void createGeometryObject(std::string name);
    bool geometryObjectExists(std::string name) const;
    const FadaMesh::GeometryObject* getGeometryObject(std::string name) const;
    FadaMesh::GeometryObject* getGeometryObject(std::string name);

    // fonctions d'accès au geometryobjects_constructor
    // const FadaMesh::GeometryObjectsConstructorInterface* getGeometryObjectsConstructor() const;
    // FadaMesh::GeometryObjectsConstructorInterface*& getGeometryObjectsConstructor();
    // void addGeometryObject(const std::string& name, FadaMesh::GeometryObject* geo );
    // const std::map<std::string, FadaMesh::GeometryObject*>&  getGeometryObjects() const;
    // std::map<std::string, FadaMesh::GeometryObject*>&  getGeometryObjects();
    Alat::Vector<Alat::Node>&   getAllNodes();
    Alat::Vector<Cell>&   getCells();
    Alat::Vector<Side>&   getSides();
    Alat::Vector<SideCell>&  getSidesOfCells();
    Alat::Vector<CellSide>&  getCellsOfSides();
    const Alat::Vector<Alat::Node>&   getNodes() const;
    const Alat::Vector<Cell>&   getCells() const;
    const Alat::Vector<Side>&   getSides() const;
    const Alat::Vector<SideCell>&  getSidesOfCells() const;
    const Alat::Vector<CellSide>&  getCellsOfSides() const;
    const Cell&   getCell(int i) const;
    const Side&   getSide(int i) const;
    const SideCell&  getSidesOfCell(int i) const;
    std::string getName() const;
    std::string getInfo() const;
    int getDimension() const;
    int getNNodesPerCell(int i) const;
    int getNNodesPerSide(int i) const;
    int getNSidesPerCell(int i) const;
    int getNNodes() const;
    int getNCells() const;
    int getNSides() const;
    int getNEdges() const;
    int getNodeIdOfCell(int iK, int ii) const;
    int getNodeIdOfSide(int iS, int ii) const;
    int getSideIdOfCell(int iK, int ii) const;
    int getCellIdOfSide(int iS, int ii) const;
    const Alat::Node& getNode(int i) const;
    const Alat::Node& getNodeOfSide(int is, int ii) const;
    const Alat::Node& getNodeOfCell(int iK, int ii) const;
    Alat::Node getNodeOfCell(int iK) const;
    Alat::Node getNodeOfSide(int iS) const;

    int getLocalIndexOfSideInCell(int iK, int iS) const;
    const FadaMesh::BoundaryInfo* getBoundaryInfo() const;
    FadaMesh::BoundaryInfo* getBoundaryInfo();
    const Alat::IntVector& getBoundaryColors() const;
    const Alat::IntVector& getBoundarySides(int color) const;
    bool cellIsCurved(int iK) const;
    int findNeightborHangingCells(int iK, int iS, Alat::Node pt);
    void readFadaMesh(const std::string& basefilename);
    void writeFadaMesh(const std::string& basefilename, std::string datatype = "binary") const;

    ///! supposes that _cells is filled; fills _sides and _sides_of_cells.
    void constructSidesFromCells(BoundarySideToColor& bsides, int color_default = 0);
    void constructSidesFromCells(BoundarySideToColor& bsides, const BoundarySideToColor& icsides, int color_default = 0);
    // void computeCellNeighbours();
    // void computeCellConnectivity(Alat::SparsityPattern& SPC) const;

    void writeVtk(std::string filename) const;
    void writeMeshInfo(std::string filename) const;
    // void writeH5(H5::H5File& file) const;
    void writeH5(std::string filename) const;

    void writeBoundaryVtk(std::string filename) const;
    // void addGeometryObjects(const std::string& filename, const Alat::StringVector& names_of_geometry_objects, std::string datatype);
    bool isMultilevel() const;

    int localIndexOfNodeInCell(int iK, int in) const;
    int localIndexOfSideInCell(int iK, int is) const;
    int localIndexOfEdgeInCell(int iK, int ie) const;

    // const Alat::SparsityPatternFixArray<2>& getCellNeighbours() const;
  };
}

/*---------------------------------------------------------*/

#endif
