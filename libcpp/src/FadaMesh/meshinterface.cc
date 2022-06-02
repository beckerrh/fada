#include  "FadaMesh/meshinterface.h"
#include  <cassert>

using namespace FadaMesh;

Alat::Node FadaMesh::MeshInterface::_dummynode;

/*--------------------------------------------------------------------------*/
MeshInterface::~MeshInterface()
{}
MeshInterface::MeshInterface() : Alat::InterfaceBase()
{}
MeshInterface::MeshInterface( const MeshInterface& meshinterface) : Alat::InterfaceBase(meshinterface)
{
  assert(0);
}
MeshInterface& MeshInterface::operator=( const MeshInterface& meshinterface)
{
  InterfaceBase::operator=(meshinterface);
  assert(0);
  return *this;
}
std::string MeshInterface::getInterfaceName() const
{
  return "MeshInterface";
}


/*--------------------------------------------------------------------------*/
const std::string& MeshInterface::getInfileName() const
{
  _notWritten("getInfileName");
}

/*--------------------------------------------------------------------------*/
std::string MeshInterface::getCellType() const
{
  _notWritten("getCellType");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::getNodesOfCell(int iK, Alat::Vector<Alat::Node>& F) const
{
  _notWritten("getNodesOfCell");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::setResolution(int level) const
{
  _notWritten("setResolution");
}
int MeshInterface::getResolution() const
{
  _notWritten("getResolution");
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNLevels() const
{
  _notWritten("getNLevels");
}

/*--------------------------------------------------------------------------*/

Alat::Node MeshInterface::getNodeOfSide(int iS) const
{
  _notWritten("getNodeOfSide");
  return _dummynode;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNodeIdOfSideOfCell(int iK, int iis, int ii) const
{
  _notWritten("getNodeIdOfSideOfCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

Alat::Node MeshInterface::getNodeOfCell(int iK) const
{
  _notWritten("getNodeOfCell");
  return _dummynode;
}

/*--------------------------------------------------------------------------*/

Alat::Node MeshInterface::getNodeOfEdge(int iK) const
{
  _notWritten("getNodeOfEdge");
  return _dummynode;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNNodes() const
{
  _notWritten("getNNodes");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNEdges() const
{
  _notWritten("getNEdges");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNSides() const
{
  _notWritten("getNSides");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNCells() const
{
  _notWritten("getNCells");
  return 0;
}

/*--------------------------------------------------------------------------*/

const Alat::Node& MeshInterface::getNode(int i) const
{
  _notWritten("getNode");
  return _dummynode;
}

/*--------------------------------------------------------------------------*/

const Alat::Node& MeshInterface::getNodeOfCell(int iK, int ii) const
{
  return getNode( getNodeIdOfCell(iK, ii) );
}

/*--------------------------------------------------------------------------*/

const Alat::Node& MeshInterface::getNodeOfEdge(int iE, int ii) const
{
  return getNode( getNodeIdOfEdge(iE, ii) );
}

/*--------------------------------------------------------------------------*/

const Alat::Node& MeshInterface::getNodeOfSide(int iS, int ii) const
{
  return getNode( getNodeIdOfSide(iS, ii) );
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNNodesPerCell(int i) const
{
  _notWritten("getNNodesPerCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNNodesPerSide(int i) const
{
  _notWritten("getNNodesPerSide");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNSidesPerCell(int i) const
{
  _notWritten("getNSidesPerCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNEdgesPerCell(int i) const
{
  _notWritten("getNEdgesPerCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNEdgesPerSide(int i) const
{
  _notWritten("getNEdgesPerSide");
  return 0;
}

/*--------------------------------------------------------------------------*/

const FadaMesh::BoundaryInfo* MeshInterface::getBoundaryInfo() const
{
  _notWritten("getBoundaryInfo");
  return NULL;
}
const FadaMesh::CurvedInteriorSideInfo* MeshInterface::getCurvedInteriorSideInfo() const
{
  _notWritten("getCurvedInteriorSideInfo");
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNodeIdOfCell(int i, int ii) const
{
  _notWritten("getNodeIdOfCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNodeIdOfSide(int i, int ii) const
{
  _notWritten("getNodeIdOfSide");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getNodeIdOfEdge(int i, int ii) const
{
  _notWritten("getNodeIdOfEdge");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getSideIdOfCell(int i, int ii) const
{
  _notWritten("getSideIdOfCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getEdgeIdOfCell(int i, int ii) const
{
  _notWritten("getEdgeIdOfCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getEdgeIdOfSide(int i, int ii) const
{
  _notWritten("getEdgeIdOfSide");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getCellIdOfSide(int i, int ii) const
{
  _notWritten("getCellIdOfSide");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getLocalIndexOfSideInCell(int iK, int iS) const
{
  _notWritten("getLocalIndexOfSideInCell");
  return 0;
}

/*--------------------------------------------------------------------------*/

bool MeshInterface::geometryObjectExists(std::string name) const
{
  _notWritten("geometryObjectExists");
  return 0;
}

/*--------------------------------------------------------------------------*/

const FadaMesh::GeometryObject* MeshInterface::getGeometryObject(std::string name) const
{
  _notWritten("getGeometryObject");
  return NULL;
}

/*--------------------------------------------------------------------------*/

FadaMesh::GeometryObject* MeshInterface::getGeometryObject(std::string name)
{
  _notWritten("getGeometryObject");
  return NULL;
}

// /*--------------------------------------------------------------------------*/
//
// const FadaMesh::GeometryObjectsConstructorInterface* MeshInterface::getGeometryObjectsConstructor() const
// {
//   _notWritten("getGeometryObjectsConstructor");
//   return NULL;
// }
//
// /*--------------------------------------------------------------------------*/
//
// FadaMesh::GeometryObjectsConstructorInterface*& MeshInterface::getGeometryObjectsConstructor()
// {
//   _notWritten("getGeometryObjectsConstructor");
//   return dummy_geometryobjects_constructor;
// }

/*--------------------------------------------------------------------------*/

void MeshInterface::createGeometryObject(std::string name)
{
  _notWritten("CreateGeometryObject");
}

/*--------------------------------------------------------------------------*/

const FadaMesh::CurvedBoundaryInformation* MeshInterface::getCurvedBoundaryInformation() const
{
  _notWritten("getCurvedBoundaryInformation");
  return NULL;
}

/*--------------------------------------------------------------------------*/

void MeshInterface::read(std::string filename)
{
  _notWritten("read");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::write(std::string filename) const
{
  _notWritten("write");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::readFadaMesh(const std::string& basefilename)
{
  _notWritten("readFadaMesh");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::writeFadaMesh(const std::string& basefilename, std::string type) const
{
  _notWritten("writeFadaMesh");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::writeVtk(std::string filename) const
{
  _notWritten("writeVtk");
}
//
// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::writeEnsightGeometry(std::string filename) const
// {
//   _notWritten("writeEnsightGeometry");
// }

/*--------------------------------------------------------------------------*/
void MeshInterface::writeMeshInfo(std::string filename) const
{
  _notWritten("writeMeshInfo");
}

/*--------------------------------------------------------------------------*/

void MeshInterface::writeBoundaryVtk(std::string filename) const
{
  _notWritten("writeBoundaryVtk");
}

/*--------------------------------------------------------------------------*/

// void MeshInterface::writeH5(H5::H5File& file) const
void MeshInterface::writeH5(std::string filename) const
{
  _notWritten("writeH5");
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getVtkType() const
{
  _notWritten("getVtkType");
  return 0;
}

// /*--------------------------------------------------------------------------*/
//
// std::string MeshInterface::getEnsightType() const
// {
//   _notWritten("getEnsightType");
//   return "none";
// }

/*--------------------------------------------------------------------------*/

int MeshInterface::getBoundaryVtkType() const
{
  _notWritten("getBoundaryVtkType");
  return 0;
}

// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::computeCellConnectivity(Alat::SparsityPattern& SPC) const
// {
//   _notWritten("computeCellConnectivity");
// }
//
// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::computeCellNeighbours()
// {
//   _notWritten("computeCellNeighbours(");
// }

/*--------------------------------------------------------------------------*/

bool MeshInterface::cellIsCurved(int iK) const
{
  _notWritten("cellIsCurved");
  return 0;
}
//
// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::addGeometryObjects(const std::string& filename, const Alat::StringVector& names_of_geometry_objects, std::string datatype)
// {
//   _notWritten("addGeometryObjects");
// }
//
// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::addGeometryObject(const std::string& name, FadaMesh::GeometryObject* geo)
// {
//   _notWritten("addGeometryObject");
// }

// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::writeEnsightGeometryObjectDescFile(const std::string& basefilename)
// {
//   _notWritten("writeEnsightGeometryObjectDescFile");
// }

/*--------------------------------------------------------------------------*/

int MeshInterface::findNeightborHangingCells(int iK, int iS, Alat::Node pt)
{
  _notWritten("findNeightborHangingCells");
  return 0;
}

/*--------------------------------------------------------------------------*/

int MeshInterface::getLocalNodeIndiceOfSide(int ii, int isl) const
{
  _notWritten("getLocalNodeIndiceOfSide");
  return 0;
}

//
// /*--------------------------------------------------------------------------*/
//
// void MeshInterface::getMeshSizeForStabilization(double& hs, int iS, int iK, int iil) const
// {
//   _notWritten("getMeshSizeForStabilization");
// }

/*--------------------------------------------------------------------------*/

int MeshInterface::getCouplingOffset(int iS) const
{
  _notWritten("getCouplingOffset");
}

/*--------------------------------------------------------------------------*/

Alat::Vector<Alat::Node>& MeshInterface::getAllNodes()
{
  _notWritten("getAllNodes");
}

/*--------------------------------------------------------------------------*/

int MeshInterface::localIndexOfNodeInCell(int iK, int in) const
{
  _notWritten("localIndexOfNodeInCell");
}

int MeshInterface::localIndexOfSideInCell(int iK, int is) const
{
  _notWritten("localIndexOfSideInCell");
}

int MeshInterface::localIndexOfEdgeInCell(int iK, int ie) const
{
  _notWritten("localIndexOfEdgeInCell");
}

/*--------------------------------------------------------------------------*/

// const Alat::SparsityPatternFixArray<2>& MeshInterface::getCellNeighbours() const
// {
//   _notWritten("getCellNeighbours");
// }

FadaMeshEnums::meshtype MeshInterface::getType() const
{
  _notWritten("getType");
}

void MeshInterface::getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const
{
  _notWritten("getLocalIndicesOfSidesInCell");
}

void MeshInterface::getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const
{
  _notWritten("getLocalIndicesOfSidesAndDiagonalsInCell");
}
