#include  "Alat/assemblevector.h"
#include  "Alat/iomanager.h"
#include  "Fada/modelinterface.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/femmanagerinterface.h"
#include  "Fada/integrator.h"
#include  "Fada/problemdata.h"
#include  "Fada/timedata.h"
#include  "Fada/varinfointegrator.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/variablevector.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
void Integrator::setProblemData(const ProblemData* problemdata)
{
  _problemdata = problemdata;
}

/*--------------------------------------------------------------------------*/
Integrator::~Integrator()
{
  if(_varinfo)
  {
    delete _varinfo;
    _varinfo = NULL;
  }
}

Integrator::Integrator() : IntegratorInterface(), _basicinitcalled(false), _time(0.0), _dt(0.0), _dtold(0.0), _masscoefimplicit(0.0), _timescheme("none"), _problemdata(NULL), _femmanager(NULL), _femmanagerR(NULL), _femfctmapL(NULL), _femfctmapR(NULL), _solver(NULL), _model(NULL)
{
  _varinfo = new VarInfoIntegrator;
}

Integrator::Integrator( const Integrator& integrator) :  _basicinitcalled(false), _problemdata(NULL), IntegratorInterface(integrator)
{
  assert(0);
}

Integrator& Integrator::operator=( const Integrator& integrator)
{
  IntegratorInterface::operator=(integrator);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/
const Fada::DomainSolverInterface* Integrator::getSolver() const
{
  assert(_solver);
  return _solver;
}

const Fada::ModelInterface* Integrator::getModel() const
{
  assert(_model);
  return _model;
}

/*--------------------------------------------------------------------------*/
void Integrator::defineVariables(const std::string& output, const std::string& input, const std::string& data)
{
  _varinfo->defineVariables(output, input, data);
}

/*--------------------------------------------------------------------------*/
void Integrator::basicInit(const Fada::DomainSolverInterface* solver)
{
  _basicInit(solver);

  _solver = dynamic_cast<const Fada::DomainSolverInterface*>( solver );
  assert(_solver);
  _varinfo->basicInit( _solver->getVariableManager(),  _solver->getFemManager() );
  _model = dynamic_cast<const Fada::ModelInterface*>( _solver->getModel() );
  assert(_model);
}

/*--------------------------------------------------------------------------*/

const VarInfoIntegrator* Integrator::getVarInfo() const
{
  return _varinfo;
}

/*--------------------------------------------------------------------------*/
void Integrator::_basicInit(const Fada::DomainSolverInterface* solver)
{
  if(_basicinitcalled)
  {
    std::cerr<<"*** ERROR Integrator::basicInit() called already for integrator "<<getName()<<"\n";
    assert(0);
    exit(1);
  }
  // _solver = solver;
  _basicinitcalled = true;
  _dimension = solver->getMesh()->getDimension();
  _looptype = solver->getType();
  const Fada::DomainSolverInterface* domainsolver = dynamic_cast<const Fada::DomainSolverInterface*>( solver );
  assert(domainsolver);
  _boundarymanager = domainsolver->getBoundaryManager();
  _femmanager = domainsolver->getFemManager();
  _femmanagerR = domainsolver->getFemManagerRight();
}

/*--------------------------------------------------------------------------*/
const FemInterface* Integrator::fem(std::string ivar) const
{
  return _femmanager->getFem(ivar);
}

const FemInterface* Integrator::femL(std::string ivar) const
{
  return _femmanager->getFem(ivar);
}

const FemInterface* Integrator::femR(std::string ivar) const
{
  return _femmanagerR->getFem(ivar);
}

void Integrator::setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const
{
  _femfctmapL = &femfctmap;
}

void Integrator::setFemFunctions(const Fada::FemFunctionsMap& femfctmapL, const Fada::FemFunctionsMap& femfctmapR) const
{
  _femfctmapL = &femfctmapL;
  _femfctmapR = &femfctmapR;
}

void Integrator::setAssembleData(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const
{
  _AlocLL = &AlocLL;
  _AlocLR = &AlocLR;
  _AlocRL = &AlocRL;
  _AlocRR = &AlocRR;
}

void Integrator::setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const
{
  _flocL = &flocL;
  _flocR = &flocR;
}

/*--------------------------------------------------------------------------*/
int Integrator::getOrderForIntegrationFormula(int orderin, int orderout) const
{
  return orderin+orderout;
}

const Integrator::DomainsOfTerm& Integrator::getDomainsOfTerm() const
{
  return _domainsofterm;
}

Integrator::DomainsOfTerm& Integrator::getDomainsOfTerm()
{
  return _domainsofterm;
}

/*--------------------------------------------------------------------------*/
void Integrator::addDomainsOfTerm(FadaEnums::term t, FadaEnums::domain d)
{
  _domainsofterm[t].insert(d);
}

/*--------------------------------------------------------------------------*/
void Integrator::setNoIntegration(FadaEnums::term t, FadaEnums::domain d)
{
  _nointegration.insert( Fada::TermAndDomain(t, d) );
}

bool Integrator::noIntegration(Fada::TermAndDomain term_and_domain) const
{
  return _nointegration.find( term_and_domain ) != _nointegration.end();
}

/*--------------------------------------------------------------------------*/
const Alat::Set<FadaEnums::domain>& Integrator::getDomainsOfTerm(FadaEnums::term t) const
{
  if( !( worksOn(t) ) )
  {
    _error_string( "getDomainsOfTerm", "this integrator does not work on term "+FadaEnums::termToString(t) );
  }
  return _domainsofterm.find(t)->second;
}

/*--------------------------------------------------------------------------*/
Alat::Set<FadaEnums::domain>& Integrator::getDomainsOfTerm(FadaEnums::term t)
{
  if( !( worksOn(t) ) )
  {
    _error_string( "getDomainsOfTerm", "this integrator does not work on term "+FadaEnums::termToString(t) );
  }
  return _domainsofterm.find(t)->second;
}

/*--------------------------------------------------------------------------*/
bool Integrator::worksOn(FadaEnums::term t) const
{
  if( _domainsofterm.find(t) != _domainsofterm.end() )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/*--------------------------------------------------------------------------*/
bool Integrator::worksOn(FadaEnums::domain d) const
{
  for(Integrator::DomainsOfTerm::const_iterator p = _domainsofterm.begin(); p != _domainsofterm.end(); p++)
  {
    if( p->second.find(d) != p->second.end() )
    {
      return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------*/
bool Integrator::worksOnDomainOfTerm(FadaEnums::term t, FadaEnums::domain d) const
{
  if( worksOn(t) )
  {
    if( _domainsofterm.find(t)->second.find(d) != _domainsofterm.find(t)->second.end() )
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    _error_string( "worksOnDomainOfTerm", "this integrator does not work on term "+FadaEnums::termToString(t) );
  }
}

/*--------------------------------------------------------------------------*/
void Integrator::setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit)
{
  _time = timedata.time;
  _dt = timedata.deltat;
  _dtold =  timedata.deltatold;
  _masscoefimplicit = masscoefimplicit;
}

void Integrator::setTimeScheme(std::string time_discretization)
{
  _timescheme = time_discretization;
}

/*--------------------------------------------------------------------------*/
int Integrator::getDimension() const
{
  return _dimension;
}

const Fada::BoundaryManager* Integrator::getBoundaryManager() const
{
  return _boundarymanager;
}

/*--------------------------------------------------------------------------*/
double Integrator::getTime() const
{
  return _time;
}

double Integrator::getDt() const
{
  return _dt;
}

double Integrator::getDtOld() const
{
  return _dtold;
}

double Integrator::getMassCoefImplicit() const
{
  return _masscoefimplicit;
}

/*--------------------------------------------------------------------------*/
double Integrator::getParameter(const std::string& parametername) const
{
  if( !_parameters.hasKey(parametername) )
  {
    std::cerr<<"*** ERROR in FadaModels::Common::BasicIntegrator::getParameter(): parameter not defined \""<<parametername<<"\" " << "in integrator \"" << getName() << "\"\n";
    std::cerr << "known parameters: " << _parameters << "\n";
    assert(0);
    exit(1);
  }
  return _parameters[parametername];
}

double Integrator::getParameter(const std::string& parametername, double d) const
{
  if( !_parameters.hasKey(parametername) )
  {
    return d;
  }
  return _parameters[parametername];
}

/*--------------------------------------------------------------------------*/
std::string Integrator::getStringParameter(const std::string& parametername) const
{
  if( !_strings.hasKey(parametername) )
  {
    std::cerr<<"*** ERROR in FadaModels::Common::BasicIntegrator::getStringParameter(): parameter not defined \""<<parametername<<"\" " << "in integrator \"" << getName() << "\"\n";
    std::cerr << "known stringd: " << _strings << "\n";
    assert(0);
    exit(1);
  }
  return _strings[parametername];
}

std::string Integrator::getStringParameter(const std::string& parametername, std::string d) const
{
  if( !_strings.hasKey(parametername) )
  {
    return d;
  }
  return _strings[parametername];
}

/*--------------------------------------------------------------------------*/

const Fada::ProblemData* Integrator::getProblemData() const
{
  assert(_problemdata);
  return _problemdata;
}

/*--------------------------------------------------------------------------*/

const Fada::DataInterface* Integrator::getProblemData(const std::string& varname, const std::string& dataname) const
{
  return getProblemData()->getData(varname, dataname);
}

/*--------------------------------------------------------------------------*/

std::string Integrator::getName() const
{
  return "Integrator";
}

/*--------------------------------------------------------------------------*/

void Integrator::setParameter(const std::string& parametername, double d)
{
  _parameters[parametername] = d;
}

/*--------------------------------------------------------------------------*/

void Integrator::setParameter(const std::string& parametername, std::string d)
{
  _strings[parametername] = d;
}

/*--------------------------------------------------------------------------*/

void Integrator::setData(const std::string& dataname, const DataInterface* data)
{
  _data[dataname] = data;
}

/*--------------------------------------------------------------------------*/

const Fada::DataInterface* Integrator::getData(const std::string& name) const
{
  return _data.getData(name);
}

const Fada::DataMap* Integrator::getData() const
{
  return &_data;
}

bool Integrator::hasKey(const std::string& name) const
{
  return _data.hasKey(name);
}

const Fada::DataInterface* Integrator::getDataIfDefined(const std::string& name) const
{
  if( not hasKey(name) )
  {
    return NULL;
  }
  return _data.getData(name);
}

/*--------------------------------------------------------------------------*/
void Integrator::setLavrentievParameter(double parameter) const
{
  _trparameter = parameter;
}
