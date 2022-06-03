#include  "Fada/applicationinterface.h"
#include  "FadaMesh/boundaryinfo.h"
#include  "Fada/boundarymanager.h"
#include  "Fada/modelinterface.h"
#include  "Alat/filescanner.h"
#include  "Alat/intvector.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <stdlib.h>
#include  <string>

using namespace Fada;
using namespace std;

/*--------------------------------------------------------------------------*/
std::string BoundaryManager::_none = "none";
Alat::StringSet BoundaryManager::_emptyset;
/*--------------------------------------------------------------------------*/

BoundaryManager::~BoundaryManager() {}
BoundaryManager::BoundaryManager() : Alat::InterfaceBase(), _init(0) {}
BoundaryManager::BoundaryManager(const BoundaryManager& boundarymanager) : Alat::InterfaceBase(boundarymanager), _init(0)
{
  assert(0);
}

BoundaryManager& BoundaryManager::operator=(const BoundaryManager& boundarymanager)
{
  assert(0);
  return *this;
}

std::string BoundaryManager::getName() const
{
  return "BoundaryManager";
}

std::string BoundaryManager::getInterfaceName() const
{
  return "BoundaryManager";
}

/*--------------------------------------------------------------------------*/
std::ostream& BoundaryManager::writeBoundaryConditions(std::ostream& os) const
{
  os << "|>~~~BoundaryManager:\n";
  os << "(color : conditions) ";
  for(Alat::Map<int, Alat::StringSet>::const_iterator p = _bdrycondofcolor.begin(); p != _bdrycondofcolor.end(); p++)
  {
    os << "(" << p->first << " : " << p->second <<")  ";
  }
  os << "\n";
  os << "(condition : vars) ";
  for(Alat::Map<std::string, Alat::StringSet>::const_iterator p = _varsofbdrycond.begin(); p != _varsofbdrycond.end(); p++)
  {
    os << "(" << p->first << " : " << p->second <<")  ";
  }
  os << "\n";
  return os;
}

/*--------------------------------------------------------------------------*/
void BoundaryManager::checkBoundaryConditions(const Fada::VariablesMap& unknowns, const FadaMesh::BoundaryInfo* boundaryinfo) const
{
  // test1 : qu'une variable par étiquette
  for(Alat::Map<int, Alat::StringSet>::const_iterator p = _bdrycondofcolor.begin(); p != _bdrycondofcolor.end(); p++)
  {
    const Alat::StringSet& bdryconsitions = p->second;
    Alat::StringSet variables;
    for(Alat::StringSet::const_iterator q = bdryconsitions.begin(); q != bdryconsitions.end(); q++)
    {
      assert( variables.find(*q) == variables.end() );
      variables.insert(*q);
    }
  }
  // test2 : il existe au moins une cl par bloc et etiqutte du maillage
  const Alat::IntVector& colors = boundaryinfo->getColors();
  for(int i = 0; i < colors.size(); i++)
  {
    int color = colors[i];
    if( _bdrycondofcolor.find(color) == _bdrycondofcolor.end() )
    {
      std::cerr << "_bdrycondofcolor "  << _bdrycondofcolor << "\n";
      _error_string("checkBoundaryConditions", "no boundary conditions for color", color);
    }
    if( _bdrycondofcolor[color].size() != _numberofboundaryconditions )
    {
      std::cerr << "_bdrycondofcolor[color].size()="<<_bdrycondofcolor[color].size()<<"\n";
      std::cerr << "_numberofboundaryconditions="<<_numberofboundaryconditions<<"\n";
      _error_string("checkBoundaryConditions", "wrong number of boundary conditions for color", color);
    }
  }
}

/*--------------------------------------------------------------------------*/
const Alat::StringSet& BoundaryManager::getBoundaryConditionsOfColor(int color) const
{
  return _bdrycondofcolor[color];
}

const Alat::StringSet& BoundaryManager::getVariablesOfBoundaryCondition(std::string bdrycond) const
{
  if( _varsofbdrycond.find(bdrycond) == _varsofbdrycond.end() )
  {
    return _emptyset;
  }
  return _varsofbdrycond[bdrycond];
}

bool BoundaryManager::boundaryConditionIs(std::string bdrycond, int color) const
{
  return _bdrycondofcolor[color].find(bdrycond) != _bdrycondofcolor[color].end();
}

/*--------------------------------------------------------------------------*/
void BoundaryManager::setData(const BoundaryConditionToVariables& boundaryconditiontovariables, const Fada::ApplicationInterface* application)
{
  for(BoundaryConditionToVariables::const_iterator p = boundaryconditiontovariables.begin(); p != boundaryconditiontovariables.end(); p++)
  {
    std::string boundarycondition = p->first;
    const Alat::StringSet& variables = p->second;
    for(Alat::StringSet::const_iterator r = variables.begin(); r != variables.end(); r++)
    {
      if(*r != "")
      {
        _varsofbdrycond[boundarycondition].insert( *r );
      }
    }
    assert(application);
    std::string colors = application->getBoundaryData(boundarycondition);
    Alat::StringVector colorsvector = Alat::Tokenize(colors, "|");
    for(int i = 0; i < colorsvector.size(); i++)
    {
#ifdef  CLANG
      _bdrycondofcolor[std::atoi( colorsvector[i].c_str() )].insert(boundarycondition);
#else
      _bdrycondofcolor[atoi( colorsvector[i].c_str() )].insert(boundarycondition);
#endif
    }
  }
  // std::cerr << "BoundaryManager:"<<writeBoundaryConditions(std::cerr)<<"\n";
  // assert(0);
}

/*--------------------------------------------------------------------------*/
void BoundaryManager::basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model)
{
  Alat::DataFormatHandler dataformathandler;
  std::string strongdirichlet, strongnormaldirichlet;
  dataformathandler.insert("strongdirichlet", &strongdirichlet, "");
  dataformathandler.insert("strongnormaldirichlet", &strongnormaldirichlet, "");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "BoundaryManager", 0);

  Alat::StringVector strongvector = Alat::Tokenize(strongdirichlet, "|");
  for(int i = 0; i < strongvector.size(); i++)
  {
    Alat::StringVector v2 = Alat::Tokenize(strongvector[i], ":");
    if(v2.size() != 2)
    {
      _error_string("basicInit", "do not understand", strongvector[i]);
    }
    Alat::StringVector vars = Alat::Tokenize(v2[1], ",");
    for(int ii = 0; ii < vars.size(); ii++)
    {
      _strongconds[v2[0]].insert(vars[ii]);
    }
  }
  Alat::StringVector strongnormalvector = Alat::Tokenize(strongnormaldirichlet, "|");
  for(int i = 0; i < strongnormalvector.size(); i++)
  {
    Alat::StringVector v2 = Alat::Tokenize(strongnormalvector[i], ":");
    assert(v2.size() == 2);
    Alat::StringVector vars = Alat::Tokenize(v2[1], ",");
    for(int ii = 0; ii < vars.size(); ii++)
    {
      _strongnormalconds[v2[0]].insert(vars[ii]);
    }
  }

  _numberofboundaryconditions = model->numberOfBoundaryConditionsPerColor();
  // std::cerr << "BoundaryManager::basicInit() model="<<model->getName()<<" _numberofboundaryconditions="<<_numberofboundaryconditions<<"\n";

  Alat::StringSet possibleboundaryconditions;
  Fada::ModelInterface::BoundaryConditionToVariables boundaryconditiontovariables;
  model->definePossibleBoundaryConditions( boundaryconditiontovariables );
  for(Fada::ModelInterface::BoundaryConditionToVariables::const_iterator p = boundaryconditiontovariables.begin(); p != boundaryconditiontovariables.end(); p++)
  {
    possibleboundaryconditions.insert(p->first);
  }

  const Fada::ApplicationInterface* application = model->getApplication();
  setData(boundaryconditiontovariables, application);
}

/*--------------------------------------------------------------------------*/
bool BoundaryManager::boundaryConditionIsStrong(std::string varname, int color) const
{
  const Alat::StringSet& bdryconds = _bdrycondofcolor[color];
  for(Alat::StringSet::const_iterator p = bdryconds.begin(); p != bdryconds.end(); p++)
  {
    if( _strongconds.find(*p) == _strongconds.end() )
    {
      return false;
    }
    const Alat::StringSet& vars = _strongconds[*p];
    if( vars.find(varname) != vars.end() )
    {
      return true;
    }
  }
  return false;
}

bool BoundaryManager::boundaryConditionIsStrongNormal(std::string varname, int color) const
{
  const Alat::StringSet& bdryconds = _bdrycondofcolor[color];
  for(Alat::StringSet::const_iterator p = bdryconds.begin(); p != bdryconds.end(); p++)
  {
    if( _strongnormalconds.find(*p) == _strongnormalconds.end() )
    {
      return false;
    }
    const Alat::StringSet& vars = _strongnormalconds[*p];
    if( vars.find(varname) != vars.end() )
    {
      return true;
    }
  }
  return false;
}

const Alat::StringSet& BoundaryManager::boundaryConditions(int color) const
{
  return _bdrycondofcolor[color];
}

const std::string& BoundaryManager::boundaryCondition(std::string varname, int color) const
{
  const Alat::StringSet& bdryconds = _bdrycondofcolor[color];
  for(Alat::StringSet::const_iterator p = bdryconds.begin(); p != bdryconds.end(); p++)
  {
    if( _varsofbdrycond.find(*p) == _varsofbdrycond.end() )
    {
      return _none;
    }
    const Alat::StringSet& vars = _varsofbdrycond[*p];
    if( vars.find(varname) != vars.end() )
    {
      return *p;
    }
  }
  return _none;
}
