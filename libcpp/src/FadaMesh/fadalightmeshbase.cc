#include  "Alat/directoryandfileexists.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "FadaMesh/coarseninfo.h"
#include  "FadaMesh/fadalightmeshbase.h"
#include  "FadaMesh/geometryobjectsconstructor.h"
#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/hangingsideinfo.h"
#include  "FadaMesh/refineinfo.h"
#include  "Alat/doublevector.h"
#include  "Alat/stringvector.h"
#include  "Alat/tokenize.h"
#include  <algorithm>
#include  <fstream>
#include  <stdio.h>
#include  <string.h>

using namespace FadaMesh;
using namespace std;

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
CellBase<NODESPERSIDE> FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::_dummyside;

/*---------------------------------------------------------*/

template<int N>
int CellBase<N>::node(int i) const
{
  assert(i < N);
  return Alat::FixArray<N, int>::operator[](i);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::~FadaMeshBase()
{
  for(std::map<std::string, FadaMesh::GeometryObject*>::iterator p = _geometryobjects.begin(); p != _geometryobjects.end(); p++)
  {
    if(p->second)
    {
      delete p->second;
      p->second = NULL;
    }
  }
  delete _geometryobjects_constructor;
  _geometryobjects_constructor = NULL;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::FadaMeshBase() : FadaMesh::MeshInterface(), _infilename("none"), _visutype("cg")
{
  _geometryobjects_constructor = new GeometryObjectsConstructor;
  createGeometryObject("HangingNodeInfo");
  createGeometryObject("HangingSideInfo");
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::FadaMeshBase(const FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>& fadalightmeshbase) : FadaMesh::MeshInterface(fadalightmeshbase), _infilename("none")
{
  assert(0);
}


/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::setResolution(int level) const
{}


template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getResolution() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
bool FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::isMultilevel() const
{
  return false;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodesOfCell(int iK, Alat::Vector<Alat::Node>& F) const
{
  F.set_size(NODESPERCELL);
  for(int ii = 0; ii < NODESPERCELL; ii++)
  {
    F[ii] = getNode( getNodeIdOfCell(iK, ii) );
  }
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
bool FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::geometryObjectExists(std::string name) const
{
  bool exists = _geometryobjects.find(name) != _geometryobjects.end();
  return exists;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const FadaMesh::GeometryObject* FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getGeometryObject(std::string name) const
{
  for(std::map<std::string, FadaMesh::GeometryObject*>::const_iterator p = _geometryobjects.begin(); p != _geometryobjects.end(); p++)
  {
    if(p->first == name)
    {
      return p->second;
    }
  }
  std::cerr << "*** FadaMesh::FadaMeshBase::getGeometryObject() : object \""<<name<<"\" not found\n";
  std::cerr << _geometryobjects.size() << "\n";
  for(std::map<std::string, FadaMesh::GeometryObject*>::const_iterator p = _geometryobjects.begin(); p != _geometryobjects.end(); p++)
  {
    std::cerr << p->first << "\n";
  }
  return NULL;
  //      assert(0);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
FadaMesh::GeometryObject* FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getGeometryObject(std::string name)
{
  if( _geometryobjects.find(name) == _geometryobjects.end() )
  {
    std::cerr << "*** FadaMesh::FadaMeshBase::getGeometryObject() : object \""<<name<<"\" not found\n";
    assert(0);
    exit(1);
  }
  return _geometryobjects[name];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Vector<Alat::Node>& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getAllNodes()
{
  return _nodes;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Cell>& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCells()
{
  return _cells;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Side>& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSides()
{
  return _sides;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::SideCell>& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSidesOfCells()
{
  return _sides_of_cells;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::CellSide>&  FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCellsOfSides()
{
  return _cells_of_sides;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Vector<Alat::Node>&   FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodes() const
{
  return _nodes;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Cell>&   FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCells() const
{
  return _cells;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Side>&   FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSides() const
{
  return _sides;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::SideCell>&  FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSidesOfCells() const
{
  std::cerr<<"getSidesOfCells "<<DIM<<" "<<NODESPERCELL<<" "<<SIDESPERCELL<<" "<<NODESPERSIDE<<'\n';
  return _sides_of_cells;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Vector<typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::CellSide>&  FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCellsOfSides() const
{
  return _cells_of_sides;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Cell& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCell(int i) const
{
  return _cells[i];
}


/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Side& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSide(int i) const
{
  return _sides[i];
}


/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::SideCell& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSidesOfCell(int i) const
{
  return _sides_of_cells[i];
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const std::string& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getInfileName() const
{
  return _infilename;
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
std::string FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getName() const
{
  return "FadaMesh::FadaMeshBase";
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
std::string FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getInfo() const
{
  std::string info;
  std::stringstream ss1, ss2, ss3;
  ss1 << getNNodes();
  ss2 << getNCells();
  ss3 << getNSides();
  return _infilename + "_" + ss1.str()+"_"+ss2.str()+"_"+ss3.str();
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getDimension() const
{
  return DIM;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNNodesPerCell(int i) const
{
  return NODESPERCELL;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNNodesPerSide(int i) const
{
  return NODESPERSIDE;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNSidesPerCell(int i) const
{
  return SIDESPERCELL;
}

// /*---------------------------------------------------------*/
//
// template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
// const Alat::SparsityPatternFixArray<2>& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCellNeighbours() const
// {
//   return _neighbors_of_cells;
// }

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNNodes() const
{
  return _nodes.size();
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNCells() const
{
  return _cells.size();
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNSides() const
{
  return _sides.size();
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNEdges() const
{
  return 0;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeIdOfCell(int iK, int ii) const
{
  return _cells[iK][ii];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeIdOfSide(int iS, int ii) const
{
  return _sides[iS][ii];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getSideIdOfCell(int iK, int ii) const
{
  return _sides_of_cells[iK][ii];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getCellIdOfSide(int iS, int ii) const
{
  return _cells_of_sides[iS][ii];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Node& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNode(int i) const
{
  return _nodes[i];
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Node& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeOfSide(int is, int ii) const
{
  int i =  getNodeIdOfSide(is, ii);
  return getNode(i);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::Node& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeOfCell(int iK, int ii) const
{
  int i =  getNodeIdOfCell(iK, ii);
  assert(i >= 0);
  assert( i < getNNodes() );
  return getNode(i);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getLocalIndexOfSideInCell(int iK, int iS) const
{
  int ii = -1;
  for(int jj = 0; jj < getNSidesPerCell(iK); jj++)
  {
    if(getSideIdOfCell(iK, jj) == iS)
    {
      ii = jj;
    }
  }
  assert(ii != -1);
  return ii;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const FadaMesh::BoundaryInfo* FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getBoundaryInfo() const
{
  return &_boundaryinfo;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
FadaMesh::BoundaryInfo* FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getBoundaryInfo()
{
  return &_boundaryinfo;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::IntVector& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getBoundaryColors() const
{
  return getBoundaryInfo()->getColors();
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const Alat::IntVector& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getBoundarySides(int color) const
{
  return getBoundaryInfo()->getSidesOfColor(color);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
bool FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::cellIsCurved(int iK) const
{
  return false;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
const typename FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::Side& FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::_getSideOfCell(int i, int ii) const
{
  _notWritten("_getSideOfCell");
  return _dummyside;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNLevels() const
{
  return 1;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::findNeightborHangingCells(int iK, int iS, Alat::Node pt)
{
  assert(getDimension() == 2);
  const FadaMesh::GeometryObject* geo = getGeometryObject("HangingSideInfo");
  const FadaMesh::HangingSideInfo* hsinfo = dynamic_cast<const FadaMesh::HangingSideInfo*>( geo );
  assert(hsinfo);
  int iKr = hsinfo->getCellNumber(0);
  int ils = hsinfo->getLocalSide(0);
  int iSr = getSideIdOfCell(iKr, ils);
  int ih = 0;
  while( iKr != iK && iS != iSr && ih < hsinfo->n() )
  {
    iKr = hsinfo->getCellNumber(ih);
    ils = hsinfo->getLocalSide(ih);
    iSr = getSideIdOfCell(iKr, ils);
    ih++;
  }
  assert(iK == iKr && iS == iSr);
  int nbhs = hsinfo->getNumberOfHangingSides(ih-1);
  for(int js = 0; js < nbhs; js++)
  {
    int iSl = hsinfo->getHangingSides(ih-1, js);
    int iKl = getCellIdOfSide(iSl, 0);
    std::cerr<<"FadaMeshBase::findNeightborHangingCells iKl "<<iKl<<'\n';
    Alat::Node extrem1, extrem2;
    extrem1 = getNodeOfSide(iSl, 0);
    extrem2 = getNodeOfSide(iSl, 1);
    // extrem1.add(-1.0, pt);
    // extrem2.add(-1.0, pt);
    extrem1 -= pt;
    extrem2 -= pt;
    // if(extrem1*extrem2 < 0)
    if(arma::dot(extrem1,extrem2) < 0)
    {
      return iKl;
    }
  }
  assert(0);
  return -1;
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::createGeometryObject(std::string name)
{
  if( geometryObjectExists(name) )
  {
    /// Changement par Robert et Roland 17/1/2011 (on appelle la fonction plusieurs fois, certains GO sont contruits dans le constructeur)
    // std::cerr<<"*** FadaMeshBase::createGeometryObject(): ::GeometryObject exists already "<<name<<"\n";
    return;

    assert(0);
  }

  // *****
  _geometryobjects_constructor->constructGeometryObject(_geometryobjects, name);
}

/*-------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Node FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeOfCell(int iK) const
{
  Alat::Node v;
  double d = 1.0/double(NODESPERCELL);
  for(int ii = 0; ii < NODESPERCELL; ii++)
  {
    const Alat::Node& vii = getNodeOfCell(iK, ii);
    v.x() += d*vii.x();
    v.y() += d*vii.y();
    v.z() += d*vii.z();
  }
  return v;
}

/*-------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Node FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeOfSide(int is) const
{
  Alat::Node v;
  double d = 1.0/double(NODESPERSIDE);
  for(int ii = 0; ii < NODESPERSIDE; ii++)
  {
    const Alat::Node& vii = getNodeOfSide(is, ii);
    v.x() += d*vii.x();
    v.y() += d*vii.y();
    v.z() += d*vii.z();
  }
  return v;
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::readFadaMesh(const std::string& basefilename)
{
  _infilename = basefilename;
  string dirname = basefilename+".fadalightmesh";
  // std::cerr << "@@@@@@@@@@@ _infilename " << _infilename << "\n";
  string filename;
  std::ifstream file;

  //! check name !
  filename = dirname+"/name";
  file.open( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** ERROR in FadaMeshBase::readFadaMesh() : cannot open file "<<filename<<"\n";
    assert(0);
  }
  std::string name, datatype;
  file>>name>>datatype;
  if( name != getName() )
  {
    std::cerr<<"*** ERROR in FadaMeshBase::readFadaMesh() : meshname(file) is \""<<name<<"\" but I am \""<<getName()<<"\"\n";
    assert(0);
  }
  if( ( datatype != "ascii" )&&( datatype != "binary" ) )
  {
    std::cerr<<"*** FadaMeshBase::readFadaMesh() unknown datatype "<<datatype<<"\n";
    assert(0);
  }
  file.close();
  if(datatype == "ascii")
  {
    std::cout<<"FadaMeshBase::readFadaMesh() \'" << getName()<< "\' reading "<<basefilename<<" : "<<datatype<<"\n";
  }

  //! read geometry-objects decription file and geometry objects
  filename = dirname+"/geometry_objects.desc";
  file.open( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** ERROR in FadaMeshBase::readFadaMesh() : cannot open file "<<filename<<"\n";
    assert(0);
  }
  int n_geometric_objects;
  file>>n_geometric_objects;
  // std::cout << "FadaMeshBase::readFadaMesh() : n_geometric_objects= " << n_geometric_objects << "\n";
  // name;
  for(int i = 0; i < n_geometric_objects; i++)
  {
    file>>name;
    // std::cout << "FadaMeshBase::readFadaMesh() : creating geometric object: " << name << "\n";
    createGeometryObject(name);
    getGeometryObject(name)->read(dirname+"/"+name);
  }
  file.close();

  //! read nodes
  filename = dirname+"/nodes";
  file.open( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** FadaMeshBase::readFadaMesh() : cannot open file "<<filename<<std::endl;
    assert(0);
  }
  _nodes.loadFada(file);
  file.close();
  // std::cerr << "geom\n";
  // std::cerr << "***nodes ok\n";

  //! read cells
  filename = dirname+"/cells";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _cells.loadFada(file);
  // _measureofcell.read(file);
  file.close();
  // std::cerr << "***cells ok\n";

  //! read sides
  filename = dirname+"/sides";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _sides.loadFada(file);
  file.close();
  // std::cerr << "***sides ok\n";

  //! read sides_of_cells
  filename = dirname+"/sides_of_cells";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _sides_of_cells.loadFada(file);
  file.close();
  // std::cerr << "***sides_of_cells ok\n";

  //! read cells_of_sides
  filename = dirname+"/cells_of_sides";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _cells_of_sides.loadFada(file);
  file.close();
  // std::cerr << "***cells_of_sides ok\n";

  //! read boundaryinfo
  filename = dirname+"/boundaryinfo";
  _boundaryinfo.read(filename);
}

/*---------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeFadaMesh(const std::string& basefilename, std::string datatype) const
{
  if(datatype == "ascii")
  {
    std::cout<<"FadaMeshBase::writeFadaMesh() \'" << getName()<< "\' writing "<<basefilename<<" : "<<datatype<<"\n";
  }
  //! create directory
  string dirname = basefilename+".fadalightmesh";
  string cmd = "rm -rf "+dirname;
  int error = system( cmd.c_str() );
  assert(!error);
  cmd = "mkdir "+dirname;
  error = system( cmd.c_str() );
  if(error)
  {
    std::cerr<<"*** FadaMeshBase::writeFadaMesh(): command failed "<<cmd<<"\n";
    exit(1);
  }
  assert(!error);

  string filename;
  std::ofstream file;

  //! write decription file
  filename = dirname+"/name";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file<<getName()<<" "<<datatype<<endl;
  file<<getInfo()<<endl;
  file.close();

  //! write geometry-objects decription file and geometry objects
  filename = dirname+"/geometry_objects.desc";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file<<_geometryobjects.size()<<"\n";
  for(std::map<std::string, FadaMesh::GeometryObject*>::const_iterator p = _geometryobjects.begin(); p != _geometryobjects.end(); p++)
  {
    // std::cerr << "FadaMeshBase writing " << p->first << " =? " << p->second->getName() << "\n";
    file<<p->first<<"\n";
    p->second->write(dirname+"/"+p->second->getName(), datatype);
  }
  file.close();

  /*************************************************/

  //! write nodes
  filename = dirname+"/nodes";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file.precision(12);
  file.setf(ios::scientific);
  _nodes.saveFada(file, datatype);
  file.close();
  //! write cells
  filename = dirname+"/cells";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file.precision(12);
  file.setf(ios::scientific);
  _cells.saveFada(file, datatype);
  // _measureofcell.write(file, datatype);
  file.close();

  //! write sides
  filename = dirname+"/sides";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _sides.saveFada(file, datatype);
  file.close();

  //! write sides_of_cells
  filename = dirname+"/sides_of_cells";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _sides_of_cells.saveFada(file, datatype);
  file.close();

  //! write cells_of_sides
  filename = dirname+"/cells_of_sides";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _cells_of_sides.saveFada(file, datatype);
  file.close();

  //! write boundaryinfo
  filename = dirname+"/boundaryinfo";
  _boundaryinfo.write(filename, datatype);
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::checkGeoFile(std::string filename) const
{
  ifstream file( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** FadaMeshBase::checkGeoFile() : cannot read file \""<<filename<<"\"\n";
    assert(0);
  }

}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::constructSidesFromCells(BoundarySideToColor& bstc, const BoundarySideToColor& icsides, int color_default)
{
  constructSidesFromCells(bstc, color_default);

  Alat::IntMap size_of_color;
  for(typename BoundarySideToColor::const_iterator p = icsides.begin(); p != icsides.end(); p++)
  {
    int col = p->second;
    if( size_of_color.find(col) == size_of_color.end() )
    {
      size_of_color[col] = 0;
    }
    size_of_color[col]++;
  }
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::constructSidesFromCells(BoundarySideToColor& bstc, int color_default)
{
  //! Assumption: bstc_key is sorted !!
  //!
  //! Assumption: no hanging nodes !!
  //!
  //! BoundarInfo::reInit is done here !

  // helpers
  typedef map<Side, Alat::FixArray<2, int> >  SideToInfo;
  SideToInfo _found;

  // first round : how many interor sides ?
  // interior == in two cells
  // boundary == only in one cell
  int ninteriorsides = 0;
  for(int i = 0; i < _cells.size(); i++)
  {
    for(int ii = 0; ii < SIDESPERCELL; ii++)
    {
      Side s = _getSideOfCell(i, ii);
      sort( s.begin(), s.end() );
      typename SideToInfo::iterator p = _found.find(s);
      if( p == _found.end() )
      {
        Alat::FixArray<2, int> index;
        _found.insert( make_pair(s, index) );
      }
      else
      {
        ninteriorsides++;
        _found.erase(p);
      }
    }
  }
  // boundary sides
  int nbsides = _found.size();
  // for(typename SideToInfo::iterator p = _found.begin(); p != _found.end(); p++)  nbsides++;
  // cout<<"constructSidesFromCells(): # interior sides, # boundary sides : "<<ninteriorsides<<" "<<nbsides<<endl;

  int nsides = ninteriorsides+nbsides;
  // cout<<"nsides = "<<nsides<<"\n";
  _sides.set_size(nsides);

  // second round : insert data
  _found.clear();
  _sides_of_cells.set_size( getNCells() );
  int count = 0;
  for(int i = 0; i < _cells.size(); i++)
  {
    for(int ii = 0; ii < SIDESPERCELL; ii++)
    {
      // The side plus a sorted copy
      // The sorted copy is necessary in order to retrieve the information
      Side s = _getSideOfCell(i, ii);
      Side ssort = s;
      sort( ssort.begin(), ssort.end() );
      typename SideToInfo::iterator p = _found.find(ssort);
      if( p == _found.end() )
      {
        Alat::FixArray<2, int> index;
        index[0] = i;
        index[1] = ii;
        _found.insert( make_pair(ssort, index) );
      }
      else
      {
        // we put in the NON-SORTED !
        // cerr << nsides << " ?  " << count << "\n";
        // std::cerr << "side found " << s << "\n";
        _sides[count] = s;
        int k = p->second[0];
        int kk = p->second[1];
        // cerr << getNCells() << " ?>  " << k << " " << i << " -- " << kk << " " << ii << "\n";
        _sides_of_cells[k][kk] = count;
        _sides_of_cells[i][ii] = count;
        _found.erase(p);
        count++;
      }
    }
  }
  // for(typename SideToInfo::const_iterator p = _found.begin(); p != _found.end(); p++)  cerr << p->first << " -- > " << p->second << "\n";
  //
  // for(typename BoundarySideToColor::iterator pp = bstc.begin(); pp != bstc.end(); pp++)  cerr << " , " << pp->first << " -- > " << pp->second << "\n";

  // now on "_found" are only sides which have been found ounce
  // We suppose the read mesh does not contain "hanging nodes", so the only-ounce found sides
  // are the boundary sides !

  // we add all boundary-sides, which are not yet defined, by giving it the color 0 !!

  // std::cout<<"bstc.size()="<<bstc.size()<<"\n";
  // std::cout<<"_found.size()="<<_found.size()<<"\n";
  int nadditionalboundarysides = 0;
  for(int i = 0; i < _cells.size(); i++)
  {
    for(int ii = 0; ii < SIDESPERCELL; ii++)
    {
      Side s = _getSideOfCell(i, ii);
      Side ssort = s;
      sort( ssort.begin(), ssort.end() );
      typename SideToInfo::iterator p = _found.find(ssort);
      if( p != _found.end() )
      {
        typename map<Side, int>::const_iterator pb = bstc.find(ssort);
        if( pb == bstc.end() )
        {
          cerr<<"*** not found "<<ssort<<"\n";
          bstc[ssort] = color_default;
          nadditionalboundarysides++;
        }
      }
    }
  }
  // cout<<"constructSidesFromCells(): # boundary sides without color (given color " <<  color_default << " ): "<<nadditionalboundarysides<<endl;

  std::map<int, int> size_of_color;
  for(typename BoundarySideToColor::const_iterator p = bstc.begin(); p != bstc.end(); p++)
  {
    int col = p->second;
    if( size_of_color.find(col) == size_of_color.end() )
    {
      size_of_color[col] = 0;
    }
    size_of_color[col]++;
  }
  FadaMesh::BoundaryInfo* BI = getBoundaryInfo();
  BI->set_size(size_of_color);
  // boundary sides
  map<int, int> col2size_bsides, col2size_bcells, col2size_bsideids;
  const Alat::IntVector& colors = _boundaryinfo.getColors();
  for(int ii = 0; ii < colors.size(); ii++)
  {
    int color = colors[ii];
    col2size_bsides[color] = 0;
    col2size_bcells[color] = 0;
    col2size_bsideids[color] = 0;
  }

  for(int i = 0; i < _cells.size(); i++)
  {
    for(int ii = 0; ii < SIDESPERCELL; ii++)
    {
      // The side plus a sorted copy
      // The sorted copy is necessary in order to retrieve the information
      Side s = _getSideOfCell(i, ii);
      Side ssort = s;
      sort( ssort.begin(), ssort.end() );
      typename SideToInfo::iterator p = _found.find(ssort);
      if( p != _found.end() )
      {
        typename map<Side, int>::const_iterator pb = bstc.find(p->first);
        assert( pb != bstc.end() );
        int color = pb->second;
        Alat::IntVector& bsides = _boundaryinfo.getSidesOfColor(color);
        Alat::IntVector& bcells = _boundaryinfo.getCellsOfColor(color);
        Alat::IntVector& bsidesid = _boundaryinfo.getSidesIdOfCellsOfColor(color);

        _sides[count] = s;
        // std::cerr << "boundary side found " << s << "\n";
        int i = p->second[0];
        int ii = p->second[1];
        _sides_of_cells[i][ii] = count;

        bsides[col2size_bsides[color]++] = count;
        bcells[col2size_bcells[color]++] = i;
        bsidesid[col2size_bsideids[color]++] = ii;

        count++;
      }
    }
  }
  _cells_of_sides.set_size(nsides);
  for(int is = 0; is < nsides; is++)
  {
    _cells_of_sides[is][0] = -1;
    _cells_of_sides[is][1] = -1;
  }
  for(int i = 0; i < _cells.size(); i++)
  {
    for(int ii = 0; ii < SIDESPERCELL; ii++)
    {
      int is = _sides_of_cells[i][ii];
      // std::cerr << i << " " << ii << " " << is << "@\n";
      if(_cells_of_sides[is][0] == -1)
      {
        _cells_of_sides[is][0] = i;
      }
      else
      {
        int help = _cells_of_sides[is][0];
        _cells_of_sides[is][0] = i;
        _cells_of_sides[is][1] = help;
      }
    }
  }
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::setVisuType(const std::string& visutype) const
{
  _visutype = visutype;
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
// void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeH5(H5::H5File& file) const
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeH5(std::string filename) const
{
  // setResolution(0);
  int nnodes = getNNodes();
  int ncells = getNCells();
  int nnodespercell = NODESPERCELL;
  int dimension = getDimension();

  Alat::DoubleVector node_coord;
  Alat::IntVector connectivity;
  hsize_t dimscoord[2];
  if(_visutype == "dg")
  {
    dimscoord[0] = nnodespercell*ncells;
    dimscoord[1] = dimension;
    node_coord.set_size(dimension*ncells*nnodespercell);
    connectivity.set_size(nnodespercell*ncells);
    // coordinates
    // connectivities
    Alat::Node N;
    int count = 0;
    for(int iK = 0; iK < ncells; iK++)
    {
      for(int ii = 0; ii < nnodespercell; ii++)
      {
        N = getNode( getNodeIdOfCell(iK, ii) );
        for(int idim = 0; idim < dimension; idim++)
        {
          node_coord[dimension*count+idim] = N[idim];
        }
        connectivity[nnodespercell*iK+ii] = count;
        count++;
      }
    }
  }
  else if(_visutype == "cg")
  {
    dimscoord[0] = nnodes;
    dimscoord[1] = dimension;
    node_coord.set_size(dimension*nnodes);
    connectivity.set_size(nnodespercell*ncells);
    // coordinates
    Alat::Node N;
    for(int iN = 0; iN < nnodes; iN++)
    {
      N = getNode(iN);
      for(int idim = 0; idim < dimension; idim++)
      {
        node_coord[dimension*iN+idim] = N[idim];
      }
    }
    for(int icell = 0; icell < ncells; icell++)
    {
      for(int ii = 0; ii < nnodespercell; ii++)
      {
        connectivity[nnodespercell*icell+ii] = getNodeIdOfCell(icell, ii);
      }
    }
  }
  else
  {
    assert(0);
  }
  // write nodes coordinates
  std::string nodes_coord_name;
  if(dimension == 2)
  {
    nodes_coord_name = "XY";
  }
  else
  {
    nodes_coord_name = "XYZ";
  }

  // std::cerr << "@@@@@@@@@@@@@@@ filename="<<filename<<"\n";

  node_coord.save(arma::hdf5_name(filename, "/"+nodes_coord_name));
  connectivity.save(arma::hdf5_name(filename, "/connectivities", arma::hdf5_opts::append));

  // H5::DataSpace fcoordspace(2, dimscoord );   //file dataspace
  // const H5std_string h5filename(filename);
  // H5::H5File file( h5filename, H5F_ACC_TRUNC );
  // H5::IntType datatype( H5::PredType::NATIVE_DOUBLE );
  // datatype.setOrder( H5T_ORDER_LE );
  // const H5std_string nodes_coord_datasetname(nodes_coord_name);
  // H5::DataSet nodes_coord_dataset = file.createDataSet( nodes_coord_datasetname, datatype, fcoordspace );
  // nodes_coord_dataset.write( reinterpret_cast<const double*>( &( *node_coord.begin() ) ), H5::PredType::NATIVE_DOUBLE, fcoordspace, fcoordspace);
  // nodes_coord_dataset.close();
  // hsize_t dimsconnect[2];
  // dimsconnect[0] = ncells;
  // dimsconnect[1] = nnodespercell;
  // H5::DataSpace fconnectspace(2, dimsconnect );
  // // write connectivities
  // const H5std_string connectivity_datasetname("connectivities");
  // H5::DataSet connectivity_dataset = file.createDataSet( connectivity_datasetname, datatype, fconnectspace );
  // connectivity_dataset.write( reinterpret_cast<const double*>( &( *connectivity.begin() ) ), H5::PredType::NATIVE_INT, fconnectspace, fconnectspace);
  // connectivity_dataset.close();
  // file.close();
}

/*---------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeMeshInfo(std::string filename) const
{
  // setResolution(0);
  std::ofstream file( filename.c_str() );
  // mesh type
  FadaMeshEnums::meshtype meshtype = getType();
  if(meshtype == FadaMeshEnums::TriangleMesh)
  {
    file<<"Triangle"<< "\n";
  }
  else if(meshtype == FadaMeshEnums::QuadrilateralMesh)
  {
    file<<"Quadrilateral"<< "\n";
  }
  else
  {
    _error_string( "writeMeshInfo", "unknown mesh", FadaMeshEnums::meshTypeToString(meshtype) );
  }
  if(_visutype == "cg")
  {
    // topology dimensions
    file<<getNCells()<<'\n';
    // // meshdata dimensions
    // file<<getNNodes()<<" "<<getDimension()<<'\n';
    // nodes dimensions
    file<<getNNodes()<<'\n';
  }
  else if(_visutype == "dg")
  {
    // topology dimensions
    file<<getNCells()<<'\n';
    // // meshdata dimensions
    // file<<getNCells()*NODESPERCELL<<" "<<getDimension()<<'\n';
    // nodes dimensions
    file<<getNCells()*NODESPERCELL<<'\n';
  }
  else
  {
    assert(0);
  }
  // cells dimensions
  file<<getNCells()<<'\n';
  // nnodes per cell
  file<<NODESPERCELL<<'\n';
  file.close();
}
//
// /*---------------------------------------------------------*/
// template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
// void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeEnsightGeometry(std::string filename) const
// {
//   return;
//
//   string name = filename+".case";
//   vector<string> v = Alat::Tokenize(filename, "/");
//   string geoname = v[v.size()-1]+".geo";
//
//   ofstream casefile( name.c_str() );
//   casefile<<"FORMAT"<<'\n';
//   casefile<<"type: ensight gold"<<'\n';
//   casefile<<"GEOMETRY"<<'\n';
//   casefile<<"model: "<<geoname<<'\n';
//   casefile.close();
//
//   name = filename+".geo";
//   FILE* file = fopen(name.c_str(), "wb");
//
//   char buffer[80];
//   strcpy(buffer, "C Binary");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "Ensight geometry file");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "description");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "node id off");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "element id off");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "part");
//   fwrite(buffer, sizeof( char ), 80, file);
//   int part_number = 1;
//   fwrite(&part_number, sizeof( int ), 1, file);
//   strcpy(buffer, "description");
//   fwrite(buffer, sizeof( char ), 80, file);
//   strcpy(buffer, "coordinates");
//   fwrite(buffer, sizeof( char ), 80, file);
//   int nn = getNNodes();
//   fwrite(&nn, sizeof( int ), 1, file);
//   typename Alat::Vector<Alat::Node>::const_iterator firstn = getNodes().begin();
//   typename Alat::Vector<Alat::Node>::const_iterator lastn = getNodes().end();
//   while(firstn != lastn)
//   {
//     float vx = float( ( *firstn ).x() );
//     fwrite(&vx, sizeof( float ), 1, file);
//     firstn++;
//   }
//   firstn = getNodes().begin();
//   while(firstn != lastn)
//   {
//     float vy = float( ( *firstn ).y() );
//     fwrite(&vy, sizeof( float ), 1, file);
//     firstn++;
//   }
//   firstn = getNodes().begin();
//   while(firstn != lastn)
//   {
//     float vz = float( ( *firstn ).z() );
//     fwrite(&vz, sizeof( float ), 1, file);
//     firstn++;
//   }
//   strcpy( buffer, getEnsightType().c_str() );
//   fwrite(buffer, sizeof( char ), 80, file);
//   int nel = getNCells();
//   fwrite(&nel, sizeof( int ), 1, file);
//   for(int i = 0; i < nel; i++)
//   {
//     int npc = getNNodesPerCell(i);
//     for(int ii = 0; ii < npc; ii++)
//     {
//       int in = getNodeIdOfCell(i, ii)+1;
//       fwrite(&in, sizeof( int ), 1, file);
//     }
//   }
// }

/*--------------------------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeVtk(std::string filename) const
{
  string name = filename;
  name += ".vtk";

  ofstream file( name.c_str() );
  assert( file.is_open() );


  file<<"# vtk DataFile Version 4.0 "<<std::endl;
  file<<"output from QuadrilateralMesh"<<std::endl;
  file<<"ASCII"<<std::endl;
  //     file << "binary" << std::endl;
  file<<"DATASET UNSTRUCTURED_GRID"<<std::endl;
  file<<std::endl;

  int nn = getNNodes();

  file<<"POINTS "<<nn;
  file<<" FLOAT"<<std::endl;
  for(int i = 0; i < nn; i++)
  {
    const Alat::Node& v = getNode(i);
    file<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<std::endl;
  }
  file<<std::endl;

  int ne = getNCells();
  int nle = NODESPERCELL;
  int lenght = ne*( nle+1 );

  file<<std::endl<<"CELLS "<<ne<<" "<<lenght<<std::endl;

  for(int ie = 0; ie < ne; ie++)
  {
    file<<nle<<" ";
    for(int ii = 0; ii < nle; ii++)
    {
      file<<getNodeIdOfCell(ie, ii)<<" ";
    }
    file<<std::endl;
  }
  file<<std::endl<<"CELL_TYPES "<<ne<<std::endl;
  for(int c = 0; c < ne; c++)
  {
    file<<getVtkType()<<" ";
  }
  file<<std::endl;

  file.close();
}

/*---------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
void FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::writeBoundaryVtk(std::string filename) const
{
  string name = filename;
  name += "-boundary.vtk";

  ofstream file( name.c_str() );
  assert( file.is_open() );

  file<<"# vtk DataFile Version 4.0 "<<std::endl;
  file<<"output from QuadrilateralMesh"<<std::endl;
  file<<"ASCII"<<std::endl;
  //     file << "binary" << std::endl;
  file<<"DATASET UNSTRUCTURED_GRID"<<std::endl;
  file<<std::endl;

  int nn = getNNodes();

  file<<"POINTS "<<nn;
  file<<" FLOAT"<<std::endl;
  for(int i = 0; i < nn; i++)
  {
    const Alat::Node& v = getNode(i);
    file<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<std::endl;
  }
  file<<std::endl;

  const FadaMesh::BoundaryInfo* BI = getBoundaryInfo();
  int nsides = BI->getNSides();
  const Alat::IntVector& colors = BI->getColors();

  int nle = NODESPERSIDE;
  int lenght = nsides*( nle+1 );
  file<<std::endl<<"CELLS "<<nsides<<" "<<lenght<<std::endl;

  for(int i = 0; i < colors.size(); i++)
  {
    int color = colors[i];
    const Alat::IntVector& sides = BI->getSidesOfColor(color);
    for(int j = 0; j < sides.size(); j++)
    {
      file<<nle<<" ";
      for(int ii = 0; ii < nle; ii++)
      {
        file<<getNodeIdOfSide(sides[j], ii)<<" ";
      }
      file<<std::endl;
    }
  }
  file<<std::endl<<"CELL_TYPES "<<nsides<<std::endl;
  for(int c = 0; c < nsides; c++)
  {
    file<<getBoundaryVtkType()<<" ";
  }
  file<<std::endl;
  file<<std::endl<<"CELL_DATA "<<nsides<<std::endl;
  file<<std::endl<<"SCALARS "<<" bdry_colors "<<" int "<<1<<std::endl;
  file<<std::endl<<"LOOKUP_TABLE default"<<std::endl;
  for(int i = 0; i < colors.size(); i++)
  {
    int color = colors[i];
    const Alat::IntVector& sides = BI->getSidesOfColor(color);
    for(int j = 0; j < sides.size(); j++)
    {
      file<<color<<" ";
      file<<std::endl;
    }
    file<<std::endl;
  }

  file.close();
}

/*--------------------------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::localIndexOfNodeInCell(int iK, int in) const
{
  int ii = -1;
  for(int jj = 0; jj < getNNodesPerCell(iK); jj++)
  {
    if(getNodeIdOfCell(iK, jj) == in)
    {
      ii = jj;
      break;
    }
  }
  assert(ii != -1);
  return ii;
}

/*--------------------------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::localIndexOfSideInCell(int iK, int is) const
{
  int ii = -1;
  for(int jj = 0; jj < getNSidesPerCell(iK); jj++)
  {
    if(getSideIdOfCell(iK, jj) == is)
    {
      ii = jj;
      break;
    }
  }
  assert(ii != -1);
  return ii;
}

/*--------------------------------------------------------------------------*/

template<int DIM, int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
int FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::localIndexOfEdgeInCell(int iK, int ie) const
{
  int ii = -1;
  for(int jj = 0; jj < getNEdgesPerCell(iK); jj++)
  {
    if(getEdgeIdOfCell(iK, jj) == ie)
    {
      ii = jj;
      break;
    }
  }
  assert(ii != -1);
  return ii;
}

/*--------------------------------------------------------------------------*/
// line mesh
#define DIM          1
#define NODESPERCELL 2
#define SIDESPERCELL 2
#define NODESPERSIDE 1
template class FadaMesh::FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef DIM
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE

// triangle mesh
#define DIM          2
#define NODESPERCELL 3
#define SIDESPERCELL 3
#define NODESPERSIDE 2
template class FadaMesh::FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef DIM
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE

// quadrilateral mesh
#define DIM          2
#define NODESPERCELL 4
#define SIDESPERCELL 4
#define NODESPERSIDE 2
template class FadaMesh::FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef DIM
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE

// tetrahedral mesh
#define DIM          3
#define NODESPERCELL 4
#define SIDESPERCELL 4
#define NODESPERSIDE 3
template class FadaMesh::FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef DIM
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE

// hexahedral mesh
#define DIM          3
#define NODESPERCELL 8
#define SIDESPERCELL 6
#define NODESPERSIDE 4
template class FadaMesh::FadaMeshBase<DIM, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef DIM
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE
