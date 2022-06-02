#include  "Alat/filescanner.h"
#include  "Fada/integrator.h"
#include  "Fada/integratormanager.h"
#include  "Fada/model.h"
#include  "Fada/variablemanager.h"
#include  "Alat/tokenize.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
Model::~Model()
{}

Model::Model() : ModelInterface(), _mesh(NULL), _stringparameters("Model"), _stringparametersofloop("Loop"), _physicalparameters("PhysicalParameters"), _numericalparameters("NumericalParameters"), _basicinitcalled(false), _modelid(-1), _application(NULL) {}
Model::Model(const Model& model) : ModelInterface(model), _application(NULL)
{
  *this = model;
}

Model& Model::operator=(const Model& model)
{
  ModelInterface::operator=(model);
  _mesh = model._mesh;
  _parameterfile = model._parameterfile;
  _stringparameters = model._stringparameters;
  _stringparametersofloop = model._stringparametersofloop;
  _physicalparameters = model._physicalparameters;
  _numericalparameters = model._numericalparameters;
  _problemdata = model._problemdata;
  _basicinitcalled = false;//model._basicinitcalled;
  _modelid = model._modelid;
  _looptype = model._looptype;
  _application = NULL;
  return *this;
}

ModelInterface* Model::clone() const
{
  assert(0);
}

std::string Model::getName() const
{
  return "Model";
}

/*--------------------------------------------------------------------------*/
std::ostream& Model::printLoopInformation(std::ostream& os) const
{
  os << "|>***Model="<<getName()<<"***<|\n";
  os << "|>~~~ProblemData:";
  for(Fada::ProblemData::MapDataMap::const_iterator p = getProblemData()->begin(); p != getProblemData()->end(); p++)
  {
    os << "  \""<<p->first<<"\": ";
    for(Fada::ProblemData::DataMap::const_iterator q = p->second.begin(); q != p->second.end(); q++)
    {
      os << q->first << " --> " << q->second->getName() << ",  ";
    }
  }
  os << "\n";
  os << "|>~~~Model Parameters:";
  for(Fada::ParameterMap<std::string>::const_iterator p = _stringparameters.begin(); p != _stringparameters.end(); p++)
  {
    os << "  \""<<p->first<<"\": "<<p->second<<", ";
  }
  os << "\n";
  os << "|>~~~Physical Parameters:";
  for(Fada::ParameterMap<double>::const_iterator p = getPhysicalParameters().begin(); p != getPhysicalParameters().end(); p++)
  {
    os << "  \""<<p->first<<"\": "<<p->second<<", ";
  }
  os << "\n";
  os << "|>~~~Numerical Parameters:";
  for(Fada::ParameterMap<double>::const_iterator p = getNumericalParameters().begin(); p != getNumericalParameters().end(); p++)
  {
    os << "  \""<<p->first<<"\": "<<p->second<<", ";
  }
  os << "\n";
  os << "|>~~~Other Parameters:";
  for(Alat::Map<std::string, const Fada::DataInterface*>::const_iterator p = _data.begin(); p != _data.end(); p++)
  {
    os << "  \""<<p->first<<"\": "<<p->second->getName()<<", ";
  }
  os << "\n";
  if(_application)
  {
    getApplication()->printLoopInformation(os);
  }
  // os << "|>~~~\tApplication: \"" << getApplication()->getName() << "\":\n"<<
  // os << "|>~~~\tFunctionals: \n";
  // for(FunctionalMap::const_iterator p = getFunctionals()->begin(); p != getFunctionals()->end(); p++)
  // {
  //   os << "|>~~~\t\t\""<<p->first<<"\"\t"<<p->second->getName()<<"\n";
  // }
  return os;
}

/*--------------------------------------------------------------------------*/
void Model::basicInit(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh, FadaEnums::looptype looptype)
{
  if(_basicinitcalled == true)
  {
    std::cerr << "je suis "<<getName() << "\n";
    assert(0);
  }
  // if(_basicinitcalled == true)
  // {
  //   return;
  // }
  _basicinitcalled = true;
  _parameterfile = parameterfile;
  _mesh = mesh;
  assert(_mesh);
  // _time = 0.;
  _looptype = looptype;
  addStringsAndParameters(parameterfile, mesh);

  getStringParamaters().addModelNameAndId(getName(), _modelid);
  getStringParamaters().basicInit( getParameterFile() );
  getPhysicalParameters().addModelNameAndId(getName(), _modelid);
  getPhysicalParameters().basicInit( getParameterFile() );
  getNumericalParameters().addModelNameAndId(getName(), _modelid);
  getNumericalParameters().basicInit( getParameterFile() );
  getStringParamatersOfLoop().basicInit( getParameterFile() );

  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("applicationname", &_applicationname, "none");
  dataformathandler.insert("scaling", &_scaling, "application");
  Alat::FileScanner filescanner(dataformathandler, getParameterFile(), "Model_"+getName(), 0);
  if(_applicationname=="none")
	{
		std::cerr << "please give 'applicationname' in Block \'" << "Model_"+getName() << "\'\n";
		assert(0);
	}
  init();
  constructApplication();
  if(_scaling=="application")
  {
    _application->setVariablesScales(_variablesscales);
  }
  else if(_scaling=="none")
  {
    _error_string("basicInit", "_scaling is", _scaling);
  }
}

/*--------------------------------------------------------------------------*/
void Model::constructApplication()
{
  // std::cerr << getName() << " _applicationname="<<_applicationname<<"\n";
  Fada::ModelInterface::BoundaryConditionToVariables boundaryconditiontovariables;
  definePossibleBoundaryConditions( boundaryconditiontovariables );
  Alat::StringSet possibleboundaryconditions;
  for(Fada::ModelInterface::BoundaryConditionToVariables::const_iterator p = boundaryconditiontovariables.begin(); p != boundaryconditiontovariables.end(); p++)
  {
    possibleboundaryconditions.insert(p->first);
  }
  // std::cerr << "model="<<getName() <<" _applicationname="<<_applicationname<<"\n";
  if(_applicationname != "none")
  {
    // std::cerr << "newApplication !!! \n";
    _application = newApplication(_applicationname);
    _application->basicInit(getParameterFile(), this, possibleboundaryconditions);
    defineApplicationData();
  }
  else
  {
    assert(_application==NULL);
  }
}
/*--------------------------------------------------------------------------*/
const Alat::StringDoubleVectorMap& Model::getVariablesScales() const
{
  return _variablesscales;
}
Alat::StringDoubleVectorMap& Model::getVariablesScales()
{
  return _variablesscales;
}
/*--------------------------------------------------------------------------*/
void Model::setVariablesScales(const Alat::StringDoubleVectorMap& maxvalues)
{
  assert(_variablesscales.size()==maxvalues.size());
  Alat::StringVector scalingtok = Alat::Tokenize(_scaling, "|");
  assert(scalingtok.size()==maxvalues.size());
  Alat::StringMap vartotok;
  for(int i=0;i<scalingtok.size();i++)
  {
    Alat::StringVector scalingtoktok = Alat::Tokenize(scalingtok[i], ":");
    if(scalingtoktok.size()!=2)
    {
      _error_string("setVariablesScales", "_scaling is", _scaling);
    }
    vartotok[scalingtoktok[0]]=scalingtoktok[1];
  }
  for(Alat::StringDoubleVectorMap::const_iterator p=maxvalues.begin(); p!=maxvalues.end(); p++)
  {
    const Alat::DoubleVector& mv = p->second;
    int ncomp = mv.size();
    assert(_variablesscales[p->first].size() == ncomp);
    for(int icomp=0;icomp<ncomp;icomp++)
    {
      _variablesscales[p->first][icomp] = mv[icomp];
    }
    Alat::StringVector eqvec = Alat::Tokenize(vartotok[p->first], "eq");
    Alat::IntSet eqset;
    for(int i=0;i<eqvec.size();i++)
    {
      eqset.insert( atoi( eqvec[i].c_str() ));
    }
    double max = 0.0;
    for(Alat::IntSet::const_iterator q=eqset.begin(); q!=eqset.end();q++)
    {
      max = fmax(max, mv[*q]);
    }
    assert(max>0.0);
    for(Alat::IntSet::const_iterator q=eqset.begin(); q!=eqset.end();q++)
    {
      _variablesscales[p->first][*q] = max;
    }
  }
  // std::cerr << "_variablesscales = "<<_variablesscales<<"\n";
}

/*--------------------------------------------------------------------------*/
void Model::addString(std::string key, std::string value)
{
  _stringparameters[key] = value;
}

void Model::addStringOfLoop(std::string key, std::string value)
{
  _stringparametersofloop[key] = value;
}

const ApplicationInterface* Model::getApplication() const
{
  return _application;
}

void Model::setMesh(const FadaMesh::MeshInterface* mesh) const
{
  _mesh = mesh;
}

const FadaMesh::MeshInterface* Model::getMesh() const
{
  return _mesh;
}

const Alat::ParameterFile* Model::getParameterFile() const
{
  return _parameterfile;
}

void Model::setRunDir(std::string rundir)
{
  _rundir = rundir;
}

std::string Model::getRunDir() const
{
  return _rundir;
}

const Fada::ParameterMap<std::string>& Model::getStringParamatersOfLoop() const
{
  return _stringparametersofloop;
}

Fada::ParameterMap<std::string>& Model::getStringParamatersOfLoop()
{
  return _stringparametersofloop;
}

const Fada::ParameterMap<std::string>& Model::getStringParamaters() const
{
  return _stringparameters;
}

Fada::ParameterMap<std::string>& Model::getStringParamaters()
{
  return _stringparameters;
}

const Fada::ParameterMap<double>& Model::getPhysicalParameters() const
{
  return _physicalparameters;
}

Fada::ParameterMap<double>& Model::getPhysicalParameters()
{
  return _physicalparameters;
}

const Fada::ParameterMap<double>& Model::getNumericalParameters() const
{
  return _numericalparameters;
}

Fada::ParameterMap<double>& Model::getNumericalParameters()
{
  return _numericalparameters;
}

const std::string& Model::getStringParameter(const std::string& key) const
{
  return _stringparameters[key];
}

const std::string& Model::getStringParameterOfLoop(const std::string& key) const
{
  return _stringparametersofloop[key];
}

void Model::addPhysicalParameter(std::string name, double d)
{
  _physicalparameters[name] = d;
}

void Model::addNumericalParameter(std::string name, double d)
{
  _numericalparameters[name] = d;
}

bool Model::physicalParameterhasKey(std::string name) const
{
  return _physicalparameters.hasKey(name);
}

bool Model::numericalParameterhasKey(std::string name) const
{
  return _numericalparameters.hasKey(name);
}

const Fada::ProblemData* Model::getProblemData() const
{
  return &_problemdata;
}

Fada::ProblemData* Model::getProblemData()
{
  return &_problemdata;
}

Fada::DataInterface* Model::getProblemData(const std::string& varname, const std::string& dataname)
{
  return _problemdata.getData(varname, dataname);
}

const Fada::DataInterface* Model::getProblemData(const std::string& varname, const std::string& dataname) const
{
  return _problemdata.getData(varname, dataname);
}

bool Model::isDataDefined(const std::string& name) const
{
  return _data.hasKey(name);
}

FadaEnums::looptype Model::getLoopType() const
{
  return _looptype;
}

Fada::ParameterTranslator&  Model::getDictionary() const
{
  return _dico;
}

FunctionalMap* Model::getFunctionals()
{
  return &_functional_map;
}

const FunctionalMap* Model::getFunctionals() const
{
  return &_functional_map;
}

/*--------------------------------------------------------*/
double Model::getPhysicalParameter(const std::string& parametername) const
{
  if( not _physicalparameters.hasKey(parametername) )
  {
    std::cerr<<"*** ERROR Fada::Model::getPhysicalParameter(): parameter not found \""<<parametername<<"\"\n";
    std::cerr<<"*** known parameters \""<<_physicalparameters<<"\"\n";
    assert(0);
    exit(1);
  }
  return _physicalparameters[parametername];
}

double Model::getNumericalParameter(const std::string& parametername) const
{
  if( not _numericalparameters.hasKey(parametername) )
  {
    std::cerr<<"*** ERROR Fada::Model::getNumericalParameter(): parameter not found \""<<parametername<<"\"\n";
    std::cerr<<"*** known parameters \""<<_numericalparameters<<"\"\n";
    assert(0);
    exit(1);
  }
  return _numericalparameters[parametername];
}

/*--------------------------------------------------------*/
void Model::setParameters(){}

/*--------------------------------------------------------*/
void Model::deleteData(const std::string& name)
{
  if( not _data.hasKey(name) )
  {
    _warning_string( "deleteData", "parameter does not exist: " + name );
    return;
  }
  if(_data[name])
  {
    delete _data[name];
    _data[name] = NULL;
  }
  _data.erase(name);
  // std::cerr << "_data " << _data << "\n";
}

/*--------------------------------------------------------*/
const Fada::DataInterface*& Model::setData(const std::string& name)
{
  if( _data.hasKey(name) )
  {
    _error_string( "setData", "parameter exists already " + name );
  }
  return _data[name];
}

/*--------------------------------------------------------*/
void Model::setDataForProblemData(const std::string& varname, const std::string& problemdataname, const std::string& dataname, const Fada::DataInterface* data)
{
  // std::cerr<<"Model::setDataForProblemData "<<this->getName()<<'\n';
  if( not getProblemData(varname, problemdataname) )
  {
    _warning_string( "setDataForProblemData", "problemdata \""+problemdataname+ "\" for variable \"" + varname + "\" not defined" );
    return;
    // assert(0);
    // exit(1);
  }
  getProblemData(varname, problemdataname)->setData( dataname, data );
}

/*--------------------------------------------------------*/
void Model::setParameterForProblemData(const std::string& varname, const std::string& problemdataname, const std::string& dataname, double d)
{
  // std::cerr<<"Model::setDataForProblemData "<<this->getName()<<"  "<<dataname<<'\n';
  // for (Fada::ProblemData::MapDataMap::const_iterator it=getProblemData()->begin();it!=getProblemData()->end();it++)
  // {
  //     std::cerr<< " Model::setParameterForProblemData() " << (*it).first<<'\n';
  // }
  if( not getProblemData(varname, problemdataname) )
  {
    _warning_string( "setParameterForProblemData", "problemdata \""+problemdataname+ "\" for variable \"" + varname +  "\" for parameter \"" + dataname + "\" not defined\n" );
    assert(0);
    exit(1);
    return;
  }
  getProblemData(varname, problemdataname)->setParameter( dataname, d );
}

/*--------------------------------------------------------*/
void Model::setParameterForProblemDataIfDefined(const std::string& varname, const std::string& problemdataname, const std::string& dataname, double d)
{
  if( not getProblemData(varname, problemdataname) )
  {
    return;
  }
  getProblemData(varname, problemdataname)->setParameter( dataname, d );
}

/*--------------------------------------------------------*/
const Fada::DataInterface* Model::getData(const std::string& name) const
{
  if( not _data.hasKey(name) )
  {
    _error_string( "getData", "parameter not found " + name );
  }
  return _data[name];
}

/*--------------------------------------------------------*/
Fada::FunctionalInterface* Model::getFunctional(const std::string& name)
{
  if( not _functional_map.hasKey(name) )
  {
    _error_string( "getFunctional", "unknwown functional " + name + "\nknown functionals ", _functional_map.keys() );
  }
  return _functional_map[name];
}

/*--------------------------------------------------------*/
const Fada::FunctionalInterface* Model::getFunctional(const std::string& name) const
{
  if( not _functional_map.hasKey(name) )
  {
    _error_string( "getFunctional", "unknwown functional " + name );
  }
  return _functional_map[name];
}

/*--------------------------------------------------------*/
std::string Model::determineFemType(std::string varname) const
{
  assert( getParameterFile() );
  std::string fem, femtype;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert(varname, &fem);
  Alat::FileScanner filescanner(dataformathandler, getParameterFile(), "Variables", 0);

  femtype = "none";
  if( ( fem == "dg0" ) || ( fem == "dg1" ) || ( fem == "dg2" ) || ( fem == "dg3" ) )
  {
    femtype = "dg";
  }
  else if( ( fem == "nc1" ) || ( fem == "nc2" ) || ( fem == "nc3" ) || ( fem == "morley" ) || ( fem == "hermite" ) )
  {
    femtype = "nc";
  }
  else if( ( fem == "cg1" ) || ( fem == "cg2" ) || ( fem == "cg3" ) )
  {
    femtype = "cg";
  }
  else
  {
    _error_string( "determineFemType", "unknwown fem " + fem + "\" for variable \""+varname);
    assert(0);
    exit(1);
  }
  return femtype;
}
