#include "Alat/iomanager.h"
#include  "FadaMesh/getmeshtype.h"
#include  "FadaMesh/singlemeshcomposition.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/trianglemesh.h"
// #include  "FadaMesh/linemesh.h"
#include  <cassert>
#include  <fstream>
#include  <sstream>
#include  <iomanip>
#include  <sstream>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

SingleMeshComposition::~SingleMeshComposition()
{
  if(_mesh)
  {
    delete _mesh;
    _mesh = NULL;
  }
}

SingleMeshComposition::SingleMeshComposition() : FadaMesh::MeshCompositionInterface()
{}
SingleMeshComposition::SingleMeshComposition( const SingleMeshComposition& singlemeshcomposition) : FadaMesh::MeshCompositionInterface(singlemeshcomposition)
{
  assert(0);
}

SingleMeshComposition& SingleMeshComposition::operator=( const SingleMeshComposition& singlemeshcomposition)
{
  FadaMesh::MeshCompositionInterface::operator=(singlemeshcomposition);
  assert(0);
  return *this;
}

std::string SingleMeshComposition::getName() const
{
  return "SingleMeshComposition";
}

SingleMeshComposition* SingleMeshComposition::clone() const
{
  return new SingleMeshComposition(*this);
}

/*--------------------------------------------------------------------------*/
int SingleMeshComposition::getDimension() const
{
  return _mesh->getDimension();
}

/*--------------------------------------------------------------------------*/

int SingleMeshComposition::getNCells() const
{
  return _mesh->getNCells();
}

/*--------------------------------------------------------------------------*/

int SingleMeshComposition::getNDomains() const
{
  return 1;
}

/*--------------------------------------------------------------------------*/

int SingleMeshComposition::getNCouplingMeshes() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/

const FadaMesh::MeshInterface* SingleMeshComposition::getMesh(int i) const
{
  return _mesh;
}

/*--------------------------------------------------------------------------*/

void SingleMeshComposition::constructFadaMesh(const std::string& meshname)
{
  Alat::StringPair p = getMeshType(meshname);
  std::string type = p.first;
  std::string datatype = p.second;
  assert(datatype == "ascii" || datatype == "binary");

  // if(type=="FadaMesh::LineMesh")
  // {
  //   _mesh = new FadaMesh::LineMesh;
  // }

  // else
  if(type == "FadaMesh::QuadrilateralMesh")
  {
    _mesh = new FadaMesh::QuadrilateralMesh;
  }
  else if(type=="FadaMesh::TriangleMesh")
  {
    _mesh = new FadaMesh::TriangleMesh;
  }
  else
  {
    _error_string("constructFadaMesh", "unknown meshtype: "+type);
  }
  // else if(type=="FadaMesh::TetrahedralMesh")
  // {
  //   _mesh = new FadaMesh::TetrahedralMesh;
  // }

  // ****
  // setGeometryObjectsConstructor(M);
  // delete _mesh->getGeometryObjectsConstructor();
  // _mesh->getGeometryObjectsConstructor()=new GeometryObjectsConstructor;
}

/*--------------------------------------------------------------------------*/

void SingleMeshComposition::read(const std::string& basefilename)
{
  _mesh->readFadaMesh(basefilename);
}

/*--------------------------------------------------------------------------*/

void SingleMeshComposition::write(const std::string& basefilename, std::string datatype) const
{
  _mesh->writeFadaMesh(basefilename, datatype);
}

/*--------------------------------------------------------------------------*/

std::string SingleMeshComposition::getInfo() const
{
  std::stringstream ss1, ss2, ss3;
  ss1 << getNDomains();
  ss2 << getNCouplingMeshes();
  ss3 << getNCells();
  return ss1.str()+"_"+ss2.str()+"_"+ss3.str();
}

/*--------------------------------------------------------------------------*/

void SingleMeshComposition::writeMeshInfo(std::string filename) const
{
  std::ofstream file( filename.c_str() );
  file <<_mesh->getDimension()<< "\n";
  int nlevels=1;
  file <<nlevels<< "\n";
  file << getNDomains() << "\n";
  file.close();
  int iblock = 0;
  std::string blockfilename = Alat::IoManager::getFileNameOnBlock( filename, iblock);
  _mesh->writeMeshInfo(blockfilename);
}

/*--------------------------------------------------------------------------*/

void SingleMeshComposition::writeH5(std::string filename) const
{
  int ndomains = getNDomains();
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    std::string blockfilename = filename;
    std::stringstream ss;
    ss<< std::setfill('0') << std::setw(4) << idomain;
    blockfilename += "_block_"+ss.str()+".h5";
    // const H5std_string h5filename(blockfilename);
    // H5::H5File file( h5filename, H5F_ACC_TRUNC );
    // _mesh->writeH5(file);
    // file.close();
    _mesh->writeH5(blockfilename);
  }
}
