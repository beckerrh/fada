#include  "Alat/tokenize.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratormanager.h"
#include  "Fada/modelinterface.h"
#include  "Fada/variableinterface.h"
#include  "Fada/variablemanager.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
VariableManager::~VariableManager()
{}
VariableManager::VariableManager()
{}
VariableManager::VariableManager( const VariableManager& variablemanager)
{
  assert(0);
}

VariableManager& VariableManager::operator=( const VariableManager& variablemanager)
{
  assert(0);
  return *this;
}

std::string VariableManager::getName() const
{
  return "VariableManager";
}

std::ostream& VariableManager::printLoopInformation(std::ostream& os) const
{
  os << "|>~~~ Variables Unknowns =\n";
  const VariablesMap& unknowns = getUnknowns();
  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    const VariableInterface* var = p->second;
    os << var->getVarName()<<" "<<var->getNComponents()<<" "<<var->getFemName()<<" "<<var->getVisualizationType()<<"\n";
  }
  os << "|>~~~ Variables PostProcess =\n";
  const VariablesMap& postproc = getPostProcess();
  for(Fada::VariableManager::const_iterator p = postproc.begin(); p != postproc.end(); p++)
  {
    const VariableInterface* var = p->second;
    os << var->getVarName()<<" "<<var->getNComponents()<<" "<<var->getFemName()<<" "<<var->getVisualizationType()<<"\n";
  }
  os << "|>~~~ Variables Data =\n";
  const VariablesMap& data = getData();
  for(Fada::VariableManager::const_iterator p = data.begin(); p != data.end(); p++)
  {
    const VariableInterface* var = p->second;
    os << var->getVarName()<<" "<<var->getNComponents()<<" "<<var->getFemName()<<" "<<var->getVisualizationType();
  }
  if( data.size() )
  {
    os << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
int VariableManager::getNUnknownVariables() const
{
  return _unknownvariables.size();
}

int VariableManager::getNPostprocessVariables() const
{
  return _postprocessvariables.size();
}

int VariableManager::getNDataVariables() const
{
  return _datavariables.size();
}

Alat::StringSet VariableManager::getDataNames() const
{
  Alat::StringSet variables;
  for(VariablesMap::const_iterator p = _datavariables.begin(); p != _datavariables.end(); p++)
  {
    variables.insert(p->first);
  }
  return variables;
}

Alat::StringSet VariableManager::getPostProcessNames() const
{
  Alat::StringSet variables;
  for(VariablesMap::const_iterator p = _postprocessvariables.begin(); p != _postprocessvariables.end(); p++)
  {
    variables.insert(p->first);
  }
  return variables;
}

Alat::StringSet VariableManager::getUnknownsNames() const
{
  Alat::StringSet variables;
  for(VariablesMap::const_iterator p = _unknownvariables.begin(); p != _unknownvariables.end(); p++)
  {
    variables.insert(p->first);
  }
  return variables;
}

/*--------------------------------------------------------------------------*/
void VariableManager::addUnknownVariable(VariableInterface* variable)
{
  std::string name = variable->getVarName();
  // std::cerr << "### VariableManager::addUnknown() " << name << "\n";
  _unknownvariables[name] = variable;
}

void VariableManager::addPostProcessVariable(VariableInterface* variable)
{
  std::string name = variable->getVarName();
  // std::cerr << "### VariableManager::addPostProcess() " << name << "\n";
  _postprocessvariables[name] = variable;
}

void VariableManager::addAdditionalPostProcessVariable(VariableInterface* variable)
{
  assert(variable);
  std::string name = variable->getVarName();
  if( _postprocessvariables.find(name) != _postprocessvariables.end() )
  {
    return;
  }
  _postprocessvariables[name] = variable;
  int index = _index_to_postprocess.size();
  if(_postprocess_to_index.size() != index)
  {
    std::cerr << "_index_to_postprocess="<<_index_to_postprocess<<"\n";
    std::cerr << "_postprocess_to_index="<<_postprocess_to_index<<"\n";
    assert(0);
  }
  _index_to_postprocess.push_back(name);
  _postprocess_to_index[name] = index;
  // std::cerr << "_index_to_postprocess=" << _index_to_postprocess << "\n";
  // std::cerr << "_postprocess_to_index=" << _postprocess_to_index << "\n";
  // assert(0);
}

void VariableManager::addDataVariable(VariableInterface* variable)
{
  std::string name = variable->getVarName();
  std::cerr << "### VariableManager::addData() " << name << "\n";
  _datavariables[name] = variable;
}

/*--------------------------------------------------------------------------*/
double VariableManager::getScale(int ivar) const
{
  return _variablescale[ivar];
}

/*--------------------------------------------------------------------------*/
void VariableManager::_insertVariableNames(IntegratorOfName& integratorofundefineddata, IntegratorOfName& integratorofundefinedpostprocess, const Alat::StringVector& output, const Alat::StringVector& input, const Alat::StringVector& data, const IntegratorInterface* integrator) const
{
  for(int i = 0; i < output.size(); i++)
  {
    std::string name = output[i];
    if( ( _unknownvariables.find(name) == _unknownvariables.end() ) && ( _postprocessvariables.find(name) == _postprocessvariables.end() ) )
    {
      if(integratorofundefinedpostprocess.hasKey(name))
      {
        std::cerr << "VariableManager::_insertVariableNames : have already key \"" << name << "\"\n" << "\n I have:\n";
        integratorofundefinedpostprocess.write(std::cerr, "ascii");
        assert(0);
      }
      integratorofundefinedpostprocess[name] = integrator;
    }
  }
  for(int i = 0; i < data.size(); i++)
  {
    std::string name = data[i];
    if(integratorofundefineddata.hasKey(name))
    {
      std::cerr << "VariableManager::_insertVariableNames : have already key \"" << name << "\"\n" << "\n I have:\n";
      integratorofundefineddata.write(std::cerr, "ascii");
      assert(0);
    }
    if( _datavariables.find(name) == _datavariables.end() )
    {
      integratorofundefineddata[name] = integrator;
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableManager::defineDataAndPostProcessVariables(const IntegratorManager* integratormanager, const ModelInterface* model, const FadaMesh::MeshInterface* mesh, const DomainSolver* solver)
{
  //
  // Ceci suppose qu'une variable pp ou data non définie par le modèle n'est utilise que par un seul 'integrator' (sinon on s'arrete)
  //
  IntegratorOfName integratorofundefineddata, integratorofundefinedpostprocess;
  for(IntegratorManager::const_iterator_all p = integratormanager->begin_all(); p != integratormanager->end_all(); p++)
  {
    assert(p->second);
    const IntegratorInterface* domainintegrator = dynamic_cast<const IntegratorInterface*>( p->second );
    assert(domainintegrator);
    const VarInfoIntegrator* varinfo = domainintegrator->getVarInfo();
    const Alat::StringVector& output = varinfo->getVarNamesOutput();
    const Alat::StringVector& input = varinfo->getVarNamesInput();
    const Alat::StringVector& data = varinfo->getVarNamesData();
    _insertVariableNames(integratorofundefineddata, integratorofundefinedpostprocess, output, input, data, domainintegrator);
  }
  for(IntegratorOfName::const_iterator p = integratorofundefinedpostprocess.begin(); p != integratorofundefinedpostprocess.end(); p++)
  {
    std::string name = p->first;
    const IntegratorInterface* integrator = p->second;
    // std::cerr << "VariableManager::defineDataAndPostProcessVariables() name="<<name << " integrator="<<integrator->getName() << "\n";
    VariableInterface* var = integrator->constructPostProcessVariable(name, *this);
    if(not var)
    {
      std::cerr << "*** ERROR in VariableManager::defineDataAndPostProcessVariables(): neither Integrator nor Model constructs PostProcessVariable:  " << name << "\n";
      std::cerr << "*** ERROR in VariableManager::defineDataAndPostProcessVariables(): integrator:  " << integrator->getName() << "\n";
      assert(0);
      exit(1);
    }
    // std::cerr << "VariableManager::defineDataAndPostProcessVariables var="<<var->getVarName()<<" integrator="<<integrator->getName()<<"\n";
    addPostProcessVariable(var);
  }
  for(IntegratorOfName::const_iterator p = integratorofundefineddata.begin(); p != integratorofundefineddata.end(); p++)
  {
    std::string name = p->first;
    const IntegratorInterface* integrator = p->second;
    VariableInterface* var = integrator->constructDataVariable(name, *this, mesh, solver);
    if(not var)
    {
      std::cerr << "*** ERROR in VariableManager::defineDataAndPostProcessVariables(): neither Integrator nor Model constructs DataVariable:  " << name << "\n";
      std::cerr << "*** ERROR in VariableManager::defineDataAndPostProcessVariables(): integrator:  " << integrator->getName() << "\n";
      assert(0);
      exit(1);
    }
    addDataVariable(var);
  }

  // creer numerotation pour les unknowns
  _index_to_unknown.set_size( _unknownvariables.size() );
  // _varvect.set_size( _unknownvariables.size() );
  int count = 0;
  for(VariablesMap::const_iterator p = _unknownvariables.begin(); p != _unknownvariables.end(); p++)
  {
    _index_to_unknown[count] = p->first;
    // _varvect[count] = p->second;
    _unknown_to_index[p->first] = count;
    count++;
  }
  _index_to_postprocess.set_size( _postprocessvariables.size() );
  count = 0;
  for(VariablesMap::const_iterator p = _postprocessvariables.begin(); p != _postprocessvariables.end(); p++)
  {
    _index_to_postprocess[count] = p->first;
    _postprocess_to_index[p->first] = count;
    count++;
  }
  _index_to_data.set_size( _datavariables.size() );
  count = 0;
  for(VariablesMap::const_iterator p = _datavariables.begin(); p != _datavariables.end(); p++)
  {
    _index_to_data[count] = p->first;
    _data_to_index[p->first] = count;
    count++;
  }
}

/*--------------------------------------------------------------------------*/
const VariablesMap& VariableManager::getData() const
{
  return _datavariables;
}

const VariablesMap& VariableManager::getPostProcess() const
{
  return _postprocessvariables;
}

const VariablesMap& VariableManager::getUnknowns() const
{
  return _unknownvariables;
}

/*--------------------------------------------------------------------------*/
bool VariableManager::variableIsUnknown(std::string name) const
{
  return _unknownvariables.find(name) != _unknownvariables.end();
}

bool VariableManager::variableIsData(std::string name) const
{
  return _datavariables.find(name) != _datavariables.end();
}

bool VariableManager::variableIsPostProcess(std::string name) const
{
  return _postprocessvariables.find(name) != _postprocessvariables.end();
}

/*--------------------------------------------------------------------------*/
const VariableInterface* VariableManager::getData(std::string name) const
{
  return _datavariables[name];
}

const VariableInterface* VariableManager::getPostProcess(std::string name) const
{
  return _postprocessvariables[name];
}

const VariableInterface* VariableManager::getUnknown(std::string name) const
{
  return _unknownvariables[name];
}

//
// const VariableInterface* VariableManager::getUnknown(int ivar) const
// {
//   return _unknownvariables[_index_to_unknown[ivar]];
// }
//
// const VariableInterface* VariableManager::getData(int ivar) const
// {
//   return _datavariables[_index_to_data[ivar]];
// }
//
// const VariableInterface* VariableManager::getPostProcess(int ivar) const
// {
//   return _postprocessvariables[_index_to_postprocess[ivar]];
// }

/*--------------------------------------------------------------------------*/
const VariableInterface* VariableManager::getVariable(std::string name) const
{
  if( _unknownvariables.hasKey(name) )
  {
    return _unknownvariables[name];
  }
  if( _postprocessvariables.hasKey(name) )
  {
    return _postprocessvariables[name];
  }
  assert( _datavariables.hasKey(name) );
  return _datavariables[name];
}

const VariableInterface* VariableManager::getOutputVariable(std::string name) const
{
  if( _unknownvariables.hasKey(name) )
  {
    return _unknownvariables[name];
  }
  else
  {
    assert( _postprocessvariables.hasKey(name) );
    return _postprocessvariables[name];
  }
}

const VariableInterface* VariableManager::getInputVariable(std::string name) const
{
  if( _unknownvariables.hasKey(name) )
  {
    return _unknownvariables[name];
  }
  else
  {
    assert( _datavariables.hasKey(name) );
    return _datavariables[name];
  }
}
