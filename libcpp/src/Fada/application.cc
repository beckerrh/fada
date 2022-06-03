#include  "Fada/application.h"
#include  "Fada/boundarymanager.h"
#include  "Fada/modelinterface.h"
#include  "Alat/filescanner.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/modelinterface.h"
#include  "Fada/modelinterface.h"
#include  "Fada/problemdata.h"
#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/tokenize.h"
#include  "Alat/stringdoublevectormap.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
Application::~Application()
{}

Application::Application() : ApplicationInterface(), _parameterfile(NULL), _model(NULL)
{}

Application::Application( const Application& application ) : ApplicationInterface(application)
{
  *this = application;
}

Application& Application::operator=( const Application& application)
{
  ApplicationInterface::operator=(application);
  _parameterfile = application._parameterfile;
  _parameters = application._parameters;
  _bdcolors = application._bdcolors;
  _looptype = application._looptype;
  return *this;
}

std::string Application::getName() const
{
  return "Application";
}

/*--------------------------------------------------------*/
bool Application::hasExactSolutions() const
{
  return false;
}

/*--------------------------------------------------------*/
std::ostream& Application::printLoopInformation(std::ostream& os) const
{
  os << "|>===Application=" << getName() << "===<|\n";
  os << ">---- Boundary conditions:" <<_bdcolors;
  os << ">---- Parameters: " <<_parameters;
  os << ">---- Stringparameters:" <<_stringparameters<<"\n";
  return os;
}

/*--------------------------------------------------------*/
void Application::basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model, const Alat::StringSet& possibleboundaryconditions)
{
  _dimension = model->getMesh()->getDimension();
  _looptype = model->getLoopType();
  _parameterfile = parameterfile;
  _model = model;

  Alat::DataFormatHandler dataformathandler;
  for(Fada::ParameterMap<double>::iterator p = _parameters.begin(); p != _parameters.end(); p++)
  {
    dataformathandler.insert(p->first, &p->second, p->second);
  }
  for(Fada::ParameterMap<std::string>::iterator p = _stringparameters.begin(); p != _stringparameters.end(); p++)
  {
    dataformathandler.insert(p->first, &p->second, p->second);
  }
  for(Alat::StringSet::const_iterator p = possibleboundaryconditions.begin(); p != possibleboundaryconditions.end(); p++)
  {
    if( not _bdcolors.hasKey(*p) )
    {
      dataformathandler.insert(*p, &_bdcolors[*p], "none");
    }
    else
    {
      dataformathandler.insert(*p, &_bdcolors[*p], _bdcolors[*p]);
    }
  }
  for(Alat::Map<std::string, std::string>::const_iterator p = _bdcolors.begin(); p != _bdcolors.end(); p++)
  {
    if( possibleboundaryconditions.find(p->first) == possibleboundaryconditions.end() )
    {
      std::cerr << "Application::basicInit() bdrycndt " << p->first << " not found in possibilities " << possibleboundaryconditions << "\n";
      assert(0);
      exit(1);
    }
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Application_"+model->getName(), 0);
}

/*--------------------------------------------------------*/
void Application::setVariablesScales(Alat::StringDoubleVectorMap& varscales) const
{
  Alat::DataFormatHandler dataformathandler;
  std::string variablescales;
  dataformathandler.insert("variablescales", &variablescales, "none");
  Alat::FileScanner filescanner(dataformathandler, _parameterfile, "Application_"+_model->getName(), 0);
	if(variablescales=="none")
	{
		std::cerr << "please give 'variablescales' in Block \'" << "Application_"+_model->getName() << "\'\n";
		assert(0);
	}
  Alat::StringVector variablescalesvector = Alat::Tokenize(variablescales, "|");
  for(int i=0;i<variablescalesvector.size();i++)
  {
    Alat::StringVector scale = Alat::Tokenize(variablescalesvector[i], ":");
    assert(scale.size()==2);
    Alat::StringVector scales = Alat::Tokenize(scale[1], ",");
    int ncomp = scales.size();
    // varscales[scale[0]] = Alat::DoubleVector(ncomp);
    varscales[scale[0]].set_size(ncomp);
    for(int icomp=0;icomp<ncomp;icomp++)
    {
      varscales[scale[0]][icomp] = atof(scales[icomp].c_str());
    }
  }
  // std::cerr << "_variablesscales="<<_variablesscales<<"\n";
}

/*--------------------------------------------------------*/
const Alat::Map<std::string, std::string>& Application::getBoundaryConditions() const
{
  return _bdcolors;
}

void Application::clearBoundaryCondition()
{
  _bdcolors.clear();
}

/*--------------------------------------------------------*/
const Fada::ParameterMap<double>& Application::getParameters() const
{
  return _parameters;
}

Fada::ParameterMap<double>& Application::getParameters()
{
  return _parameters;
}

double Application::getParameter(const std::string& key) const
{
  return _parameters[key];
}

double& Application::getParameter(const std::string& key)
{
  return _parameters[key];
}

void Application::addParameter(std::string name, double d )
{
  _parameters[name] = d;
}

const std::string& Application::getStringParameter(const std::string& key) const
{
  return _stringparameters[key];
}

void Application::addStringParameter(std::string name, std::string s)
{
  _stringparameters[name] = s;
}

/*--------------------------------------------------------*/

int Application::getDimension() const
{
  return _dimension;
}

/*--------------------------------------------------------*/
const Alat::ParameterFile* Application::getParameterFile() const
{
  assert(_parameterfile);
  return _parameterfile;
}

/*--------------------------------------------------------*/
std::string& Application::getBoundaryData(const std::string& bdtype)
{
  return _bdcolors[bdtype];
}

std::string Application::getBoundaryData(const std::string& bdtype) const
{
  if( _bdcolors.hasKey(bdtype) )
  {
    return _bdcolors[bdtype];
  }
  else
  {
    return "none";
  }
}
