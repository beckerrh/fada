#include  "Alat/filescanner.h"
#include  "Fada/feminterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/femmanagerinterface.h"
#include  "Fada/integrationformulainterface.h"
#include  "Fada/integrationformulaselectorquadrilateralmesh.h"
#include  "Fada/integrationformulaselectortrianglemesh.h"
#include  "Fada/integrationloop.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratormanager.h"
#include  "Fada/varinfointegrator.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/systemvector.h"
#include  "Alat/variablevectorinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegrationLoop::~IntegrationLoop()
{
  for(Alat::Map<std::string, const Fada::IntegrationFormulaInterface*>::iterator p = _formulae.begin(); p != _formulae.end(); p++)
  {
    if(p->second)
    {
      assert(0);
      delete p->second;
      p->second = NULL;
    }
  }
}

IntegrationLoop::IntegrationLoop()
{}
IntegrationLoop::IntegrationLoop( const IntegrationLoop& integrationloop)
{
  assert(0);
}

IntegrationLoop& IntegrationLoop::operator=( const IntegrationLoop& integrationloop)
{
  assert(0);
  return *this;
}

std::string IntegrationLoop::getName() const
{
  return "IntegrationLoop";
}

IntegrationLoop* IntegrationLoop::clone() const
{
  assert(0);
//return new IntegrationLoop(*this);
}

// /*--------------------------------------------------------------------------*/
// void IntegrationLoop::initFemFunctions(Fada::FemFunctionsMap& femfctmap, const Fada::LocalGlobal::VectorMap& vectormap) const
// {
//   femfctmap.clear();
//   for(Fada::LocalGlobal::VectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
//   {
//     Alat::StringIntMap ncomps;
//     Alat::StringSet varnames = p->second->getVariables();
//     for(Alat::StringSet::const_iterator q=varnames.begin(); q!=varnames.end();q++)
//     {
//       ncomps[*q] = p->second->getVector(*q)->getNComponents();
//     }
//     femfctmap[p->first].basicInit(ncomps);
//   }
// }

/*--------------------------------------------------------------------------*/
void IntegrationLoop::basicInit(const FadaMesh::MeshInterface* mesh, const Fada::IntegratorManager* integratormanager, const Fada::FemManagerInterface* femmanager, const Fada::FemManagerInterface* femmanager_right, const Alat::ParameterFile* parameterfile, std::string couplingtype)
{
  // assert(femmanager_right);
  _mesh = mesh;
  _integratormanager = integratormanager;
  _femmanager = femmanager;
  _femmanagerR = femmanager_right;
  _parameterfile = parameterfile;
  _couplingtype = couplingtype;


  // Construction des formules d'integration
  std::string celltype = _mesh->getCellType();
  Fada::IntegrationFormulaSelectorInterface* integrationformulaselector = NULL;
  if( celltype == "Quadrilateral"  )
  {
    integrationformulaselector = new IntegrationFormulaSelectorQuadrilateralMesh;
  }
  else if( celltype == "Triangle")
  {
    integrationformulaselector = new IntegrationFormulaSelectorTriangleMesh;
  }

  Alat::Set<FadaEnums::domain> domains;
  domains.insert(FadaEnums::Cells);
  domains.insert(FadaEnums::BoundarySides);
  domains.insert(FadaEnums::InteriorSides);
  domains.insert(FadaEnums::CouplingSides);
  Alat::Set<FadaEnums::term> terms;
  terms.insert(FadaEnums::Initialize);
  terms.insert(FadaEnums::Matrix);
  terms.insert(FadaEnums::Form);
  terms.insert(FadaEnums::RHS);
  terms.insert(FadaEnums::TimeRHS);
  terms.insert(FadaEnums::PostProcess);
  terms.insert(FadaEnums::ComputeNormSquared);
  terms.insert(FadaEnums::SortCriterion);


  // trouver l'"odre" par term et domaine
  Alat::Map<Fada::TermAndDomain, int> orderoftermanddomain;
  for(Alat::Set<FadaEnums::domain>::const_iterator pd = domains.begin(); pd != domains.end(); pd++)
  {
    for(Alat::Set<FadaEnums::term>::const_iterator pt = terms.begin(); pt != terms.end(); pt++)
    {
      if( _integratormanager->noIntegrator(*pt, *pd) )
      {
        continue;
      }
      Fada::TermAndDomain term_and_domain(*pt, *pd);
      const Fada::IntegratorManager::IntegratorSet& integratorset = _integratormanager->getIntegratorsForTermAndDomain(*pt, *pd);
      for(Fada::IntegratorManager::IntegratorSet::const_iterator p = integratorset.begin(); p != integratorset.end(); p++)
      {
        if( ( *p )->noIntegration(term_and_domain) )
        {
          continue;
        }
        int orderout = 0;
        int orderin = 0;
        const Fada::IntegratorInterface* integratordomain = dynamic_cast<const Fada::IntegratorInterface*>( *p );
        assert(integratordomain);
        const Alat::StringSet& varnamesout = integratordomain->getVarInfo()->getVarNamesOutput();
        for(Alat::StringSet::const_iterator p = varnamesout.begin(); p != varnamesout.end(); p++)
        {
          std::string varnameout = *p;
          const Fada::FemInterface* fem = femmanager->getFem(varnameout);
          if(fem)
          {
            orderout = std::max( orderout, fem->getOrder() );
          }
        }
        const Alat::StringSet& varnamesin = integratordomain->getVarInfo()->getVarNamesInput();
        for(Alat::StringSet::const_iterator p = varnamesin.begin(); p != varnamesin.end(); p++)
        {
          std::string varnamein = *p;
          const Fada::FemInterface* fem = femmanager->getFem(varnamein);
          if(fem)
          {
            orderin = std::max( orderin, fem->getOrder() );
          }
        }
      orderoftermanddomain[term_and_domain] = ( *p )->getOrderForIntegrationFormula(orderin, orderout);
      }
    }
  }
  // std::cerr << "@@@@@   orderoftermanddomain !!\n";
  // for(Alat::Map<Fada::TermAndDomain, int>::const_iterator p = orderoftermanddomain.begin(); p != orderoftermanddomain.end(); p++)
  // {
  //   std::cerr << p->first.toString() << " orderoftermanddomain=" << p->second<<"\n";
  // }

  //lire evtl dans le fichier parametre
  Alat::Map<Fada::TermAndDomain, std::string> nameoftermanddomainfromparameterfile;
  Alat::DataFormatHandler dataformathandler;
  for(Alat::Set<FadaEnums::domain>::const_iterator pd = domains.begin(); pd != domains.end(); pd++)
  {
    for(Alat::Set<FadaEnums::term>::const_iterator pt = terms.begin(); pt != terms.end(); pt++)
    {
      if( _integratormanager->noIntegrator(*pt, *pd) )
      {
        continue;
      }
      Fada::TermAndDomain term_and_domain(*pt, *pd);
      dataformathandler.insert(term_and_domain.toString(), &nameoftermanddomainfromparameterfile[term_and_domain], "none");
    }
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "IntegrationFormulae", 0);
  // std::cerr << "IntegrationLoop::basicInit() nameoftermanddomainfromparameterfile="<<nameoftermanddomainfromparameterfile<<"\n";


  Alat::Map<Alat::Pair<int, FadaEnums::domain>, std::string > formulaeconstructed;
  Alat::Map<Alat::Pair<std::string, FadaEnums::domain>, std::string > formulaeconstructedread;
  // Alat::Set<std::string > formulaeconstructed;
  for(Alat::Set<FadaEnums::domain>::const_iterator pd = domains.begin(); pd != domains.end(); pd++)
  {
    for(Alat::Set<FadaEnums::term>::const_iterator pt = terms.begin(); pt != terms.end(); pt++)
    {
      if( _integratormanager->noIntegrator(*pt, *pd) )
      {
        continue;
      }
      Fada::TermAndDomain term_and_domain(*pt, *pd);
      int order = orderoftermanddomain[term_and_domain];
      std::string formularead = nameoftermanddomainfromparameterfile[term_and_domain];
      std::string formulaname;
      if(formularead != "none")
      {
        Alat::Pair<std::string, FadaEnums::domain> nomanddomain(formularead, *pd);
        if( formulaeconstructedread.find(nomanddomain) == formulaeconstructedread.end() )
        {
          Fada::IntegrationFormulaInterface* formula = integrationformulaselector->newIntegrationFormula( formularead, *pd);
          formulaname = formula->getName();
          _formulae[formulaname] = formula;
          formulaeconstructedread[nomanddomain] = formulaname;
        }
        else
        {
          formulaname = formulaeconstructedread[nomanddomain];
        }
        _integrationformulaefortermanddomain[term_and_domain].insert(formulaname);
      }
      else
      {
        Alat::Pair<int, FadaEnums::domain> orderdomain(order, *pd);
        if( formulaeconstructed.find(orderdomain) == formulaeconstructed.end() )
        {
          Fada::IntegrationFormulaInterface* formula;
          formula = integrationformulaselector->newIntegrationFormula( order, *pd);
          formulaname = formula->getName();
          if( _formulae.hasKey(formulaname) )
          {
            delete formula;
          }
          else
          {
            _formulae[formulaname] = formula;
          }
          formulaeconstructed[orderdomain] = formulaname;
        }
        else
        {
          formulaname = formulaeconstructed[orderdomain];
        }
        _integrationformulaefortermanddomain[term_and_domain].insert(formulaname);
      }
      _integrators_for_term_and_domain_and_formula[term_and_domain][formulaname].clear();
      const Fada::IntegratorManager::IntegratorSet& integratorset = _integratormanager->getIntegratorsForTermAndDomain(*pt, *pd);
      for(Fada::IntegratorManager::IntegratorSet::const_iterator p = integratorset.begin(); p != integratorset.end(); p++)
      {
        const Fada::IntegratorInterface* integrator = *p;
        if( integrator->noIntegration(term_and_domain) )
        {
          continue;
        }
        _integrators_for_term_and_domain_and_formula[term_and_domain][formulaname].insert(integrator);
      }
    }
  }
  bool output = false;
  if(output)
  {
    std::cerr << "@@@@@   _integrators_for_term_and_domain_and_formula !!\n";
    for(IntegratorsForTermAndDomainAndFormula::const_iterator p = _integrators_for_term_and_domain_and_formula.begin(); p != _integrators_for_term_and_domain_and_formula.end(); p++)
    {
      std::cerr << p->first.toString() << "\n";
      for(Alat::Map<std::string, IntegratorSet>::const_iterator q = p->second.begin(); q != p->second.end(); q++)
      {
        std::cerr << "\t name="<<q->first << "integrators=";
        for(IntegratorSet::const_iterator r = q->second.begin(); r != q->second.end(); r++)
        {
          std::cerr <<( *r )->getName()<<" ";
        }
        std::cerr << "\n";
      }
    }
    assert(0);
  }
  if(integrationformulaselector)
  {
    delete integrationformulaselector;
  }
}

/*---------------------------------------------------------*/

const Alat::StringSet& IntegrationLoop::getIntegrationFormulaeForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const
{
  Fada::TermAndDomain term_and_domain;
  term_and_domain.first = t;
  term_and_domain.second = d;
  return _integrationformulaefortermanddomain[term_and_domain];
}

/*---------------------------------------------------------*/

const IntegrationLoop::IntegratorSet& IntegrationLoop::getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d, const std::string& formulaname ) const
{
  Fada::TermAndDomain term_and_domain;
  term_and_domain.first = t;
  term_and_domain.second = d;
  return _integrators_for_term_and_domain_and_formula[term_and_domain][formulaname];
}
