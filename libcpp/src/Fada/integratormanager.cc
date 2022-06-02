#include  "Fada/integrationloopinterface.h"
#include  "Fada/integrator.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratormanager.h"
#include  "Fada/problemdata.h"
#include  "Alat/stringset.h"
#include  "Alat/vector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegratorManager::~IntegratorManager()
{}
IntegratorManager::IntegratorManager() : Alat::InterfaceBase()
{}

IntegratorManager::IntegratorManager( const IntegratorManager& integratormanager) : Alat::InterfaceBase(integratormanager)
{
  assert(0);
}

IntegratorManager& IntegratorManager::operator=( const IntegratorManager& integratormanager)
{
  assert(0);
  return *this;
}

std::string IntegratorManager::getInterfaceName() const
{
  return "IntegratorManager";
}

std::string IntegratorManager::getName() const
{
  return "IntegratorManager";
}

std::ostream& IntegratorManager::printLoopInformation(std::ostream& os) const
{
  for(int i = FadaEnums::term_begin(); i != FadaEnums::term_end(); i++)
  {
    FadaEnums::term term = static_cast<FadaEnums::term>( i );
    if( noIntegrator(term) )
    {
      continue;
    }
    os << "|>~~~ Integrators "<< FadaEnums::termToString(term)<<"=";
    for(IntegratorManager::const_iterator p = begin(term); p != end(term); p++)
    {
      const IntegratorInterface* integrator = *p;
      os << "  "<<integrator->getName();
    }
    os << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
void IntegratorManager::add(std::string integrator_name, IntegratorInterface* integrator, std::string output, std::string input, std::string data)
{
  assert(integrator);
  // integrator->setDomainsOfTerms();
  _integratormap[integrator_name] = integrator;
  integrator->defineVariables(output, input, data);
}

/*--------------------------------------------------------------------------*/
bool IntegratorManager::noIntegrator(FadaEnums::domain d) const
{
  for(const_iterator_all p = begin_all(); p != end_all(); p++)
  {
    const IntegratorInterface* integrator = p->second;
    // std::cerr << "integratorname: " << integrator->getName() << "\n";
    const IntegratorInterface::DomainsOfTerm& domainsofterm = integrator->getDomainsOfTerm();
    for(IntegratorInterface::DomainsOfTerm::const_iterator q = domainsofterm.begin(); q != domainsofterm.end(); q++)
    {
      for(Alat::Set<FadaEnums::domain>::const_iterator itdomain = ( q->second ).begin(); itdomain != ( q->second ).end(); itdomain++)
      {
        if(*itdomain == d)
        {
          return false;
        }
      }
    }
  }
  return true;
}

/*--------------------------------------------------------------------------*/
bool IntegratorManager::hasIntegrator(FadaEnums::domain d) const
{
  for(const_iterator_all p = begin_all(); p != end_all(); p++)
  {
    const IntegratorInterface* integrator = p->second;
    // std::cerr << "integratorname: " << integrator->getName() << "\n";
    const IntegratorInterface::DomainsOfTerm& domainsofterm = integrator->getDomainsOfTerm();
    for(IntegratorInterface::DomainsOfTerm::const_iterator q = domainsofterm.begin(); q != domainsofterm.end(); q++)
    {
      for(Alat::Set<FadaEnums::domain>::const_iterator itdomain = ( q->second ).begin(); itdomain != ( q->second ).end(); itdomain++)
      {
        if(*itdomain == d)
        {
          return true;
        }
      }
    }
  }
  return false;
}

/*--------------------------------------------------------------------------*/
bool IntegratorManager::noIntegrator(FadaEnums::term term) const
{
  return not _integrators_of_term.hasKey(term);
}

bool IntegratorManager::hasIntegrator(FadaEnums::term term) const
{
  return _integrators_of_term.hasKey(term);
}

bool IntegratorManager::noIntegrator(FadaEnums::term t, FadaEnums::domain d) const
{
  TermAndDomain term_and_domain;
  term_and_domain.first = t;
  term_and_domain.second = d;
  return not _integrators_for_term_and_domain.hasKey(term_and_domain);
}

bool IntegratorManager::hasIntegrator(FadaEnums::term t, FadaEnums::domain d) const
{
  TermAndDomain term_and_domain;
  term_and_domain.first = t;
  term_and_domain.second = d;
  return _integrators_for_term_and_domain.hasKey(term_and_domain);
}

/*--------------------------------------------------------------------------*/
IntegratorManager::const_iterator IntegratorManager::begin(FadaEnums::term term) const
{
  if( not _integrators_of_term.hasKey(term) )
  {
    _error_string( "begin", "no integrators of type " + FadaEnums::termToString(term) );
  }
  return _integrators_of_term[term].begin();
}

IntegratorManager::const_iterator IntegratorManager::end(FadaEnums::term term) const
{
  return _integrators_of_term[term].end();
}

IntegratorManager::const_iterator_all IntegratorManager::begin_all() const
{
  return _integratormap.begin();
}

IntegratorManager::const_iterator_all IntegratorManager::end_all() const
{
  return _integratormap.end();
}

IntegratorManager::iterator_all IntegratorManager::begin_all()
{
  return _integratormap.begin();
}

IntegratorManager::iterator_all IntegratorManager::end_all()
{
  return _integratormap.end();
}

/*---------------------------------------------------------*/
void IntegratorManager::setDataIfDefined(const std::string& integratorname, const std::string& dataname, const DataInterface* data)
{
  if( not _integratormap.hasKey(integratorname) )
  {
    return;
  }
  _integratormap[integratorname]->setData(dataname, data);
}

void IntegratorManager::setData(const std::string& integratorname, const std::string& dataname, const DataInterface* data)
{
  if( not _integratormap.hasKey(integratorname) )
  {
    std::cerr<<"*** ERROR Fada::IntegratorsManager::setData(): integrator does not exist  \""<<integratorname<<"\"\n";
    assert(0);
    exit(1);
  }
  _integratormap[integratorname]->setData(dataname, data);
}

void IntegratorManager::setParameterIfDefined(const std::string& integratorname, const std::string& parametername, double d)
{
  if( not _integratormap.hasKey(integratorname) )
  {
    return;
  }
  _integratormap[integratorname]->setParameter(parametername, d);
}

void IntegratorManager::setParameterIfDefined(const std::string& integratorname, const std::string& parametername, std::string s)
{
  if( not _integratormap.hasKey(integratorname) )
  {
    return;
  }
  _integratormap[integratorname]->setParameter(parametername, s);
}

/*---------------------------------------------------------*/
IntegratorInterface* IntegratorManager::getIntegrator(const std::string& integratorname)
{
  if( not _integratormap.hasKey(integratorname) )
  {
    std::cerr<<"*** ERROR Fada::IntegratorsManager::getIntegrator(): integrator does not exist  \""<<integratorname<<"\"\n";
    assert(0);
    exit(1);
  }
  return _integratormap[integratorname];
}

const IntegratorInterface* IntegratorManager::getIntegrator(const std::string& integratorname) const
{
  if( not _integratormap.hasKey(integratorname) )
  {
    return NULL;

    std::cerr<<"*** ERROR Fada::IntegratorsManager::getIntegrator(): integrator does not exist  \""<<integratorname<<"\"\n";
    assert(0);
    exit(1);
  }
  return _integratormap[integratorname];
}

/*---------------------------------------------------------*/
void IntegratorManager::initIntegrators(const Fada::SolverInterface* solver)
{
  // std::cerr << "IntegratorManager::initIntegrators() DEBUT="<<"\n";
  for(IntegratorManager::iterator_all p = begin_all(); p != end_all(); p++)
  {
    IntegratorInterface* integrator = p->second;
    // std::cerr << "1IntegratorManager::initIntegrators() integrator="<<integrator->getName()<<"\n";
    integrator->basicInit(solver);
    // std::cerr << "2IntegratorManager::initIntegrators() integrator="<<integrator->getName()<<"\n";
    integrator->initData();
    // std::cerr << "3IntegratorManager::initIntegrators() integrator="<<integrator->getName()<<"\n";
    integrator->setDomainsOfTerms();
  }
  // std::cerr << "IntegratorManager::initIntegrators() FIN="<<"\n";
}

/*---------------------------------------------------------*/
void IntegratorManager::setProblemData(const ProblemData* problemdata)
{
  for(IntegratorManager::iterator_all p = begin_all(); p != end_all(); p++)
  {
    IntegratorInterface* integrator = p->second;
    integrator->setProblemData(problemdata);
  }
}

/*---------------------------------------------------------*/
void IntegratorManager::setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit)
{
  for(IntegratorManager::iterator_all p = begin_all(); p != end_all(); p++)
  {
    p->second->setTimeInfo(timedata, masscoefimplicit);
  }
}

void IntegratorManager::setTimeScheme(std::string time_discretization)
{
  for(IntegratorManager::iterator_all p = begin_all(); p != end_all(); p++)
  {
    IntegratorInterface* integrator = p->second;
    integrator->setTimeScheme(time_discretization);
  }
}

void IntegratorManager::setLavrentievParameter(double parameter) const
{
  for(IntegratorManager::const_iterator p = begin(FadaEnums::ComputeNormSquared); p != end(FadaEnums::ComputeNormSquared); p++)
  {
    const IntegratorInterface* integrator = *p;
    integrator->setLavrentievParameter(parameter);
  }
}

/*---------------------------------------------------------*/
void IntegratorManager::defineIntegratorMaps()
{
  for(Alat::Map<std::string, IntegratorInterface*>::const_iterator p = _integratormap.begin(); p != _integratormap.end(); p++)
  {
    const IntegratorInterface* integrator = p->second;
    // std::cerr << "integratorname: " << integrator->getName() << "\n";
    const IntegratorInterface::DomainsOfTerm& domainsofterm = integrator->getDomainsOfTerm();
    for(IntegratorInterface::DomainsOfTerm::const_iterator q = domainsofterm.begin(); q != domainsofterm.end(); q++)
    {
      // std::cerr << "term: " << FadaEnums::termToString(q->first) << "\n";
      _integrators_of_term[q->first].insert(integrator);
      for(Alat::Set<FadaEnums::domain>::const_iterator itdomain = ( q->second ).begin(); itdomain != ( q->second ).end(); itdomain++)
      {
        TermAndDomain term_and_domain;
        term_and_domain.first = q->first;
        term_and_domain.second = *itdomain;
        // std::cerr<<term_and_domain<<'\n';
        _integrators_for_term_and_domain[term_and_domain].insert(integrator);
      }
    }
  }
}

/*---------------------------------------------------------*/

const IntegratorManager::IntegratorSet& IntegratorManager::getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const
{
  TermAndDomain term_and_domain;
  term_and_domain.first = t;
  term_and_domain.second = d;
  return _integrators_for_term_and_domain[term_and_domain];
}
