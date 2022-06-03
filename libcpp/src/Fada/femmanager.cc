#include  "Fada/femcgq12d.h"
#include  "Fada/femcgq22d.h"
#include  "Fada/femdgq02d.h"
#include  "Fada/femdgp02d.h"
#include  "Fada/femcgp12d.h"
#include  "Fada/femvrtm2d.h"
#include  "Fada/q12dtransformation.h"
#include  "Fada/q22dtransformation.h"
#include  "Fada/p12dtransformation.h"
#include  "Alat/assemblevector.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Fada/problemdata.h"
#include  "Alat/systemassemblevector.h"
#include  "Fada/variableinterface.h"
#include  "Fada/variablemanager.h"
#include  "Alat/systemassemblevector.h"
#include  "Fada/femfunctionsmap.h"
#include  "Alat/intvector.h"
#include  "Alat/stringset.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
FemManager::~FemManager()
{
  for(int i = 0; i < _fems.size(); i++)
  {
    delete _fems[i];
    _fems[i] = 0;
  }
}
FemManager::FemManager()
{
  _ivarfornormal = 0;
}

FemManager::FemManager( const FemManager& femmanager)
{
  _ivarfornormal = femmanager._ivarfornormal;
  int nfems = femmanager._fems.size();
  _fems.set_size(nfems);
  for(int i = 0; i < nfems; i++)
  {
    if(femmanager._fems[i])
    {
      _fems[i] = femmanager._fems[i]->clone();
    }
    else
    {
      _fems[i] = NULL;
    }
  }
  _variablemanager = femmanager._variablemanager;
  _varnametofemindex = femmanager._varnametofemindex;
}
FemManager* FemManager::clone() const
{
  return new FemManager(*this);
}
FemManager& FemManager::operator=( const FemManager& femmanager)
{
  assert(0);
}
std::string FemManager::getName() const
{
  return "FemManager";
}

/*--------------------------------------------------------------------------*/
void FemManager::basicInit(const ProblemData* problemdata, const VariableManager* variablemanager, const FadaMesh::MeshInterface* mesh)
{
  _variablemanager = variablemanager;

  const VariablesMap& datavars = variablemanager->getData();
  const VariablesMap& ppvars = variablemanager->getPostProcess();
  const VariablesMap& uvars = variablemanager->getUnknowns();

  Alat::StringSet femnames;
  for(VariablesMap::const_iterator p = datavars.begin(); p != datavars.end(); p++)
  {
    femnames.insert( p->second->getFemName() );
  }
  for(VariablesMap::const_iterator p = ppvars.begin(); p != ppvars.end(); p++)
  {
    femnames.insert( p->second->getFemName() );
  }
  for(VariablesMap::const_iterator p = uvars.begin(); p != uvars.end(); p++)
  {
    femnames.insert( p->second->getFemName() );
  }
  // std::cerr << "FemManager::basicInit() femnames " << femnames << "\n";


  Alat::StringVector femnames2( femnames.size() );
  std::copy( femnames.begin(), femnames.end(), femnames2.begin() );
  _fems.set_size( femnames.size() );
  Alat::StringIntMap femnametoindex;
  for(int i = 0; i < _fems.size(); i++)
  {
    femnametoindex[femnames2[i]] = i;
    _fems[i] = newFem(femnames2[i]);
    if(_fems[i])
    {
      _fems[i]->basicInit(mesh);
    }
  }
  for(VariablesMap::const_iterator p = datavars.begin(); p != datavars.end(); p++)
  {
    _varnametofemindex[p->first] = femnametoindex[p->second->getFemName()];
  }
  for(VariablesMap::const_iterator p = ppvars.begin(); p != ppvars.end(); p++)
  {
    _varnametofemindex[p->first] = femnametoindex[p->second->getFemName()];
  }
  for(VariablesMap::const_iterator p = uvars.begin(); p != uvars.end(); p++)
  {
    _varnametofemindex[p->first] = femnametoindex[p->second->getFemName()];
  }
}

/*--------------------------------------------------------------------------*/
Alat::Vector<FemInterface*>& FemManager::getFems() const
{
  return _fems;
}
// FemInterface* FemManager::getFem(std::string varname)
// {
//   // if (_varnametofemindex.find(varname)==_varnametofemindex.end())
//   // {
//   //   return NULL;
//   // }
//   return _fems[_varnametofemindex[varname]];
// }
// const FemInterface* FemManager::getFem(std::string varname) const
// {
//   return _fems[_varnametofemindex[varname]];
// }
FemInterface* FemManager::getFem(std::string varname) const
{
  return _fems[_varnametofemindex[varname]];
}
const Alat::StringIntMap& FemManager::getVarname2Index() const
{
  return _varnametofemindex;
}

/*--------------------------------------------------------------------------*/
void FemManager::initCell(const Alat::Vector<Alat::Node>& F, bool curved) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitCell(F);
    }
  }
}

/*--------------------------------------------------------------------------*/
void FemManager::initCellAndSide(const Alat::Vector<Alat::Node>& F, int il, bool curved) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitCellAndSide(F, il);
    }
  }
}

/*--------------------------------------------------------------------------*/
void FemManager::_pointFemFunctions(Fada::FemFunctionsMap& femfctmap, const Fada::LocalVectors& localvectors) const
{
  for(Fada::LocalVectors::LocalVectorMap::const_iterator p = localvectors.getVectorMap().begin(); p != localvectors.getVectorMap().end(); p++)
  {
    for(Alat::SystemAssembleVector::const_iterator q=p->second.begin(); q!=p->second.end(); q++)
    {
      femfctmap[p->first].computeFunctionAndGradient(q->first, getFem(q->first), q->second);
    }
  }
}

/*--------------------------------------------------------------------------*/
const Alat::Node& FemManager::x() const
{
  return _fems[_ivarfornormal]->x();
}

/*--------------------------------------------------------------------------*/
double FemManager::reInitReferencePoint(const Alat::Node& xhat) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitReferencePoint(xhat);
    }
  }
  return _fems[_ivarfornormal]->J();
}

/*--------------------------------------------------------------------------*/
void FemManager::reInitPhysicalPoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xphys, const Fada::LocalVectors& localvectors) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitPhysicalPoint(xphys);
    }
  }
  _pointFemFunctions(femfctmap, localvectors);
}

/*--------------------------------------------------------------------------*/
double FemManager::reInitReferencePoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitReferencePoint(xhat);
    }
  }
  _pointFemFunctions(femfctmap, localvectors);
  return _fems[_ivarfornormal]->J();
}

/*--------------------------------------------------------------------------*/
double FemManager::reInitCenterPoint(Fada::FemFunctionsMap& femfctmap, const Fada::LocalVectors& localvectors) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitReferenceCenterPoint();
    }
  }
  _pointFemFunctions(femfctmap, localvectors);
  return _fems[_ivarfornormal]->J();
}

/*--------------------------------------------------------------------------*/
double FemManager::reInitReferencePointBoundary(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const
{
  for(int i = 0; i < _fems.size(); i++)
  {
    if(_fems[i])
    {
      _fems[i]->reInitReferencePointBoundary(xhat);
    }
  }
  assert( _fems.size() );
  _pointFemFunctions(femfctmap, localvectors);
  return _fems[_ivarfornormal]->G();
}

/*--------------------------------------------------------------------------*/
Fada::TransformationInterface* FemManager::newTransformation(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)
{
  // return fem->newTransformation();
  FadaMeshEnums::meshtype meshtype = mesh->getType();
  if(meshtype == FadaMeshEnums::TriangleMesh)
  {
    return new P12DTransformation;
  }
  else if(meshtype == FadaMeshEnums::QuadrilateralMesh)
  {
    return new Q12DTransformation;
  }
  else
  {
    _error_string( "newTransformation", "unknown mesh", FadaMeshEnums::meshTypeToString(meshtype) );
  }
}

/*--------------------------------------------------------------------------*/
Fada::TransformationInterface* FemManager::newTransformationCurved(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)
{
  // return fem->newTransformation();
  FadaMeshEnums::meshtype meshtype = mesh->getType();
  if(meshtype == FadaMeshEnums::TriangleMesh)
  {
    return new P12DTransformation;
  }
  else if(meshtype == FadaMeshEnums::QuadrilateralMesh)
  {
    return new Q22DTransformation;
  }
  else
  {
    _error_string( "newTransformationCurved", "unknown mesh", FadaMeshEnums::meshTypeToString(meshtype) );
  }
}

/*--------------------------------------------------------------------------*/
Fada::FemInterface* FemManager::newFem(std::string femname)
{
  // std::cerr << "FemManager::newFem() femname=" << femname<<"\n";
  if(femname == "cgq12d")
  {
    return new FemCgQ12d;
  }
  else if(femname == "cgp12d")
  {
    return new FemCgP12d;
  }
  else if(femname == "cgq22d")
  {
    return new FemCgQ22d;
  }
  else if(femname == "dgp02d")
  {
    return new FemDgP02d;
  }
  else if(femname == "dgq02d")
  {
    return new FemDgQ02d;
  }
  else if(femname == "nc12d")
  {
    return new FemVRTM2d;
  }
  else if(femname == "none")
  {
    return NULL;
  }
  else
  {
    std::cerr << "*** ERROR in FemManager::newFem(): unknown femname " << femname << "\n";
    assert(0);
    exit(1);
  }
}
