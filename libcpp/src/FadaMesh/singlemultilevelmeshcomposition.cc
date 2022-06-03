#include  "Alat/iomanager.h"
#include  "FadaMesh/getmeshtype.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/singlemultilevelmeshcomposition.h"
#include  <cassert>
#include  <fstream>
#include  <iomanip>
#include  <sstream>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

SingleMultiLevelMeshComposition::~SingleMultiLevelMeshComposition()
{
  if(_mesh)
  {
    delete _mesh;
    _mesh = NULL;
  }
}

SingleMultiLevelMeshComposition::SingleMultiLevelMeshComposition() : FadaMesh::MeshCompositionInterface()
{}
SingleMultiLevelMeshComposition::SingleMultiLevelMeshComposition( const SingleMultiLevelMeshComposition& singlemeshcomposition) : FadaMesh::MeshCompositionInterface(singlemeshcomposition)
{
  assert(0);
}

SingleMultiLevelMeshComposition& SingleMultiLevelMeshComposition::operator=( const SingleMultiLevelMeshComposition& singlemeshcomposition)
{
  FadaMesh::MeshCompositionInterface::operator=(singlemeshcomposition);
  assert(0);
  return *this;
}

std::string SingleMultiLevelMeshComposition::getName() const
{
  return "SingleMultiLevelMeshComposition";
}

SingleMultiLevelMeshComposition* SingleMultiLevelMeshComposition::clone() const
{
  return new SingleMultiLevelMeshComposition(*this);
}

/*--------------------------------------------------------------------------*/

std::string SingleMultiLevelMeshComposition::getInfo() const
{
  std::stringstream ss1, ss2, ss3;
  ss1 << getNDomains();
  ss2 << getNCouplingMeshes();
  ss3 << getNCells();
  return ss1.str()+"_"+ss2.str()+"_"+ss3.str();
}

/*--------------------------------------------------------------------------*/
int SingleMultiLevelMeshComposition::getDimension() const
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( _mesh );
  assert(mlmesh);
  return mlmesh->getMesh(0)->getDimension();
}

/*--------------------------------------------------------------------------*/
int SingleMultiLevelMeshComposition::getNCells() const
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( _mesh );
  assert(mlmesh);
  return mlmesh->getMesh(0)->getNCells();
}

/*--------------------------------------------------------------------------*/

int SingleMultiLevelMeshComposition::getNDomains() const
{
  return 1;
}

/*--------------------------------------------------------------------------*/

int SingleMultiLevelMeshComposition::getNCouplingMeshes() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/

const FadaMesh::MeshInterface* SingleMultiLevelMeshComposition::getMesh(int i) const
{
  return _mesh;
}

/*--------------------------------------------------------------------------*/

void SingleMultiLevelMeshComposition::constructFadaMesh(const std::string& meshname)
{
  Alat::StringPair p = getMeshType(meshname);
  std::string type = p.first;
  std::string datatype = p.second;
  assert(datatype == "ascii" || datatype == "binary");

  _mesh = new FadaMesh::MultiLevelMesh(type);
}

/*--------------------------------------------------------------------------*/

void SingleMultiLevelMeshComposition::read(const std::string& basefilename)
{
  _mesh->readFadaMesh(basefilename);
}

/*--------------------------------------------------------------------------*/

void SingleMultiLevelMeshComposition::write(const std::string& basefilename, std::string datatype) const
{
  _mesh->writeFadaMesh(basefilename, datatype);
}

/*--------------------------------------------------------------------------*/

void SingleMultiLevelMeshComposition::writeMeshInfo(std::string filename) const
{
  // std::cerr << "@@@ SingleMultiLevelMeshComposition::writeMeshInfo()\n";
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( _mesh );
  assert(mlmesh);
  int nlevels = _mesh->getNLevels();
  std::ofstream file( filename.c_str() );
  file <<_mesh->getDimension()<< "\n";
  file <<nlevels<< "\n";
  file << getNDomains()<< "\n";
  file.close();
  int iblock = 0;
  for(int level=0;level<nlevels;level++)
  {
    std::string blockfilename = Alat::IoManager::getFileNameOnBlock( filename, iblock, level);
    mlmesh->getMesh(level)->writeMeshInfo(blockfilename);
  }
}

/*--------------------------------------------------------------------------*/

void SingleMultiLevelMeshComposition::writeH5(std::string filename) const
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( _mesh );
  assert(mlmesh);
  int ndomains = getNDomains();
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    for(int level=0;level<mlmesh->getNLevels();level++)
    {
      std::string blockfilename = filename;
      std::stringstream ss;
      ss<< std::setfill('0') << std::setw(4) << idomain<< "_level_" << std::setfill('0') << std::setw(2) << level;
      blockfilename += "_block_"+ss.str()+".h5";
      // const H5std_string h5filename(blockfilename);
      // H5::H5File file( h5filename, H5F_ACC_TRUNC );
      // mlmesh->getMesh(level)->writeH5(file);
      mlmesh->getMesh(level)->writeH5(blockfilename);
      // file.close();
    }
  }
}
