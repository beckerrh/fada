#include  "Fada/domainmodelinterface.h"
#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "FadaMesh/meshinterface.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  "Fada/modelmanager.h"
#include  "Alat/map.h"
#include  "Alat/tokenize.h"
#include  "Alat/vector.h"
#include  <cassert>
#include  <fstream>
#include  <iomanip>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
ModelManager::~ModelManager()
{}
ModelManager::ModelManager() : ModelManagerInterface()
{}
ModelManager::ModelManager( const ModelManager& modelmanager) : ModelManagerInterface(modelmanager)
{
  assert(0);
}

ModelManager& ModelManager::operator=( const ModelManager& modelmanager)
{
  ModelManager::operator=(modelmanager);
  assert(0);
  return *this;
}

std::string ModelManager::getName() const
{
  return "ModelManager";
}

/*---------------------------------------------------------*/
std::ostream& ModelManager::printLoopInformation(std::ostream& os) const
{
  os << "\n>=====================DomainModels:=====================\n";
  for(IndicesOfModelMap::const_iterator p = _indicesofmodelmapdomain.begin(); p != _indicesofmodelmapdomain.end(); p++)
  {
    os << p->first << ": " << p->second << "\n";
    getDomainModel( *p->second.begin() )->printLoopInformation(os);
  }
  os << "\n>=====================CouplingModels:=====================\n";
  for(IndicesOfModelMap::const_iterator p = _indicesofmodelmapcoupling.begin(); p != _indicesofmodelmapcoupling.end(); p++)
  {
    os << p->first << ": " << p->second << "\n";
    getCouplingModel( *p->second.begin() )->printLoopInformation(os);
  }
  return os;
}

/*---------------------------------------------------------*/
void ModelManager::basicInit(const Alat::ParameterFile* parameterfile, FadaMesh::MeshCompositionInterface* meshcomposition, FadaEnums::looptype looptype)
{
  // _singlemodel = NULL;
  // _issingle = 0;
  Alat::DataFormatHandler dataformathandler;
  int numberofmodels, numberofspecialcouplings;
  std::string coupling, model;
  dataformathandler.insert("numberofmodels", &numberofmodels, 1);
  dataformathandler.insert("numberofspecialcouplings", &numberofspecialcouplings, 0);
  dataformathandler.insert("coupling", &coupling, "none");
  dataformathandler.insert("model", &model, "none");
  Alat::FileScanner FS1(dataformathandler, parameterfile, "ModelManager", 0);

  Alat::Vector<std::string> modeltype_desc(numberofmodels), specialcouplingtype_desc(numberofspecialcouplings);
  assert(numberofmodels == 1);
  assert(numberofspecialcouplings==0);
  for(int i = 0; i < numberofmodels; i++)
  {
    std::stringstream ss;
    ss<<"model_"<< std::setw(2) << std::setfill('0') << i;
    std::string nummodel = ss.str();
    dataformathandler.insert(nummodel, &modeltype_desc[i], model+":all");
  }
  for(int i = 0; i < numberofspecialcouplings; i++)
  {
    std::stringstream ss;
    ss<<"specialcoupling_"<< std::setw(2) << std::setfill('0') << i;
    std::string numcoupling = ss.str();
    dataformathandler.insert(numcoupling, &specialcouplingtype_desc[i]);
  }
  Alat::FileScanner FS2(dataformathandler, parameterfile, "ModelManager", 0);
  _constructModelMap(modeltype_desc, meshcomposition);
  // _constructSpecialCouplingMap(specialcouplingtype_desc);
  _constructModels(parameterfile, meshcomposition, looptype);
  _indicesofmodelmapdomain.clear();
  _indicesofmodelmapcoupling.clear();
  for(int idomain = 0; idomain < getNDomainModels(); idomain++)
  {
    _indicesofmodelmapdomain[getDomainModel(idomain)->getName()].insert(idomain);
  }
  for(int icoupling = 0; icoupling < getNCouplingModels(); icoupling++)
  {
    _indicesofmodelmapcoupling[getCouplingModel(icoupling)->getName()].insert(icoupling);
  }
}

/*---------------------------------------------------------*/
void ModelManager::_constructModelMap(const Alat::Vector<std::string>& modeltype_desc, const FadaMesh::MeshCompositionInterface* meshcomposition)
{
  int ndesc = modeltype_desc.size();
  Alat::Vector<std::string> tokendesc;
  for(int idesc = 0; idesc < ndesc; idesc++)
  {
    tokendesc = Alat::Tokenize(modeltype_desc[idesc], ":");
    if( tokendesc.size() <= 1 )
    {
      std::cerr << "modeltype_desc= "<<modeltype_desc<<"\n";
      std::cerr << "modeltype_desc[idesc]= "<<modeltype_desc[idesc]<<"\n";
      assert(0);
    }
    std::string modelname = tokendesc[0];
    if(tokendesc.size() == 2 and tokendesc[1] == "all")
    {
      int ndomains = meshcomposition->getNDomains();
      for(int iblock = 0; iblock < ndomains; iblock++)
      {
        _modelmap[iblock] = modelname;
      }
    }
    else
    {
      for(int i = 1; i < tokendesc.size(); i++)
      {
        Alat::Vector<std::string> blockindofmodel = Alat::Tokenize(tokendesc[i], "-");
        for(int iblock = atoi( blockindofmodel[0].c_str() ); iblock < atoi( blockindofmodel[blockindofmodel.size()-1].c_str() )+1; iblock++)
        {
          if( _modelmap.hasKey(iblock) )
          {
            std::cerr<<"*****Error::ModelManager multiple definition of block "<<iblock<<" in model data"<<'\n';
            assert(0);
            exit(1);
          }
          _modelmap[iblock] = modelname;
        }
      }
    }
  }
}

/*---------------------------------------------------------*/
void ModelManager::_constructSpecialCouplingMap(const Alat::Vector<std::string>& specialcouplingtype_desc)
{
  assert(0);
}

/*---------------------------------------------------------*/
void ModelManager::_constructModels(const Alat::ParameterFile* parameterfile, FadaMesh::MeshCompositionInterface* meshcomposition, FadaEnums::looptype looptype)
{
  int ndomains = meshcomposition->getNDomains();
  reInitModelVector(ndomains);
  int numberofmodels = _modelmap.size();
  if(numberofmodels != ndomains)
  {
    std::cerr<<"*****Error in ModelManager::constructModels "<<'\n';
    std::cerr<<"MultiModels case: number of models different to number of domains"<<'\n';
    std::cerr<<"ndomains= "<<ndomains<<"   numberofmodels="<<numberofmodels<<'\n';
    std::cerr<<"ModelMap:\n"<<_modelmap<<'\n';
    assert(0);
    exit(1);
  }
  for(int idomain = 0; idomain < numberofmodels; idomain++)
  {
    getDomainModelPointer(idomain) = constructModelByName(_modelmap[idomain]);
  }
  int ncouplings = meshcomposition->getNCouplingMeshes();
  reInitCouplingModelVectors(ncouplings);
  for(int icoupling = 0; icoupling < ncouplings; icoupling++)
  {
    int iblockl = meshcomposition->getMacroMesh()->getCellIdOfSide(icoupling, 0);
    int iblockr = meshcomposition->getMacroMesh()->getCellIdOfSide(icoupling, 1);
    _constructCouplingModel(icoupling, iblockl, iblockr);
  }


  if(ncouplings > 0)
  {
    assert(0);
    // meshcomposition->initCouplingGrids(this);
  }

  for(int idomain = 0; idomain < numberofmodels; idomain++)
  {
    getDomainModel(idomain)->basicInit(parameterfile, meshcomposition->getMesh(idomain), looptype);
  }
  // for(int icoupling = 0; icoupling < ncouplings; icoupling++)
  // {
  //   getCouplingModel(icoupling)->basicInit(parameterfile, meshcomposition->getCouplingMesh(icoupling), looptype);
  // }
  // std::cerr<<"ModelManager::constructModels() MODELS\n";
  // for(int i = 0; i < _domainmodels.size(); i++)
  // {
  //   std::cerr << _domainmodels[i]->getName()<<"\n";
  // }
  // std::cerr<<"ModelManager::constructModels() COUPLINGMODELS\n";
  // for(int i = 0; i < _couplingmodels.size(); i++)
  // {
  //   std::cerr << _couplingmodels[i]->getName()<<"\n";
  // }
}

/*---------------------------------------------------------*/
void ModelManager::_constructCouplingModel(int icoupling, int iblockl, int iblockr)
{
  assert(0);
}

const Fada::ModelManager::IndicesOfModelMap& ModelManager::getIndicesOfDomainModelMap() const
{
  return _indicesofmodelmapdomain;
}

const Fada::ModelManager::IndicesOfModelMap& ModelManager::getIndicesOfCouplingModelMap() const
{
  return _indicesofmodelmapcoupling;
}

/*---------------------------------------------------------*/
int ModelManager::getNCouplingModels() const
{
  return _couplingmodels.size();
}

ModelInterface*& ModelManager::getCouplingModelPointer(int i)
{
  assert( i < _couplingmodels.size() );
  return _couplingmodels[i];
}

const ModelInterface* ModelManager::getCouplingModel(int i) const
{
  assert( i < _couplingmodels.size() );
  return _couplingmodels[i];
}

ModelInterface* ModelManager::getCouplingModel(int i)
{
  assert( i < _couplingmodels.size() );
  return _couplingmodels[i];
}

/*---------------------------------------------------------*/
int ModelManager::getNDomainModels() const
{
  return _domainmodels.size();
}

DomainModelInterface*& ModelManager::getDomainModelPointer(int i)
{
  assert( i < _domainmodels.size() );
  return _domainmodels[i];
}

const DomainModelInterface* ModelManager::getDomainModel(int i) const
{
  assert( i < _domainmodels.size() );
  return _domainmodels[i];
}

DomainModelInterface* ModelManager::getDomainModel(int i)
{
  assert( i < _domainmodels.size() );
  return _domainmodels[i];
}

/*---------------------------------------------------------*/

void ModelManager::reInitCouplingModelVectors(int n)
{
  _couplingmodels.set_size(n);
  _couplingorientation.set_size(n);
}

/*---------------------------------------------------------*/

void ModelManager::reInitModelVector(int n)
{
  _domainmodels.set_size(n);
}

/*--------------------------------------------------------------------------*/
int ModelManager::getLeftDomain(int icoupling) const
{
  return _couplingorientation[icoupling].first;
}

/*--------------------------------------------------------------------------*/
int ModelManager::getRightDomain(int icoupling) const
{
  return _couplingorientation[icoupling].second;
}
