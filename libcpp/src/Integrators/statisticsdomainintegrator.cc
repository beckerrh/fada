#include  "Fada/boundarymanager.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/variable.h"
#include  "Alat/variablevector.h"
#include  "Fada/varinfointegrator.h"
#include  "Alat/systemvector.h"
#include  "Integrators/statisticsdomainintegrator.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  <cassert>
#include  <limits>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
StatisticsDomainIntegrator::~StatisticsDomainIntegrator(){}
StatisticsDomainIntegrator::StatisticsDomainIntegrator() : Fada::IntegratorSingleVariable(){}
StatisticsDomainIntegrator::StatisticsDomainIntegrator( const StatisticsDomainIntegrator& dragandliftintegrator) : Fada::IntegratorSingleVariable(dragandliftintegrator){}
StatisticsDomainIntegrator& StatisticsDomainIntegrator::operator=( const StatisticsDomainIntegrator& dragandliftintegrator)
{
  Fada::IntegratorSingleVariable::operator=(dragandliftintegrator);
  assert(0);
  return *this;
}
std::string StatisticsDomainIntegrator::getName() const
{
  return "StatisticsDomainIntegrator";
}
StatisticsDomainIntegrator* StatisticsDomainIntegrator::clone() const
{
  return new StatisticsDomainIntegrator(*this);
}

/*--------------------------------------------------------------------------*/
void StatisticsDomainIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/
Fada::VariableInterface* StatisticsDomainIntegrator::constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const
{
  int ncomp = 3;
  Fada::Variable* var = new Fada::Variable(name, ncomp);
  var->setPostProcType(0, "sum");
  var->setPostProcType(1, "min");
  var->setPostProcType(2, "max");
  var->setVisualizationName(0, name+"_mean");
  var->setVisualizationName(1, name+"_min");
  var->setVisualizationName(2, name+"_max");
  return var;
}

/*--------------------------------------------------------------------------*/
void StatisticsDomainIntegrator::afterIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, Alat::SystemVector* ouput) const
{
  Alat::VariableVector* vector = dynamic_cast<Alat::VariableVector*>( ouput->getVector(_varnameout) );
  assert(vector);
  ( *vector )(0, 0) /= _omega;
  ( *vector )(1, 0) = _Tmin;
  ( *vector )(2, 0) = _Tmax;
  // std::cerr << "StatisticsDomainIntegrator::afterIntegrationLoop() vector="<<*vector<<"\n";
}

/*--------------------------------------------------------------------------*/
void StatisticsDomainIntegrator::beforeIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, const Fada::LocalGlobal& localglobal, Fada::LocalVectors& localvectors, Fada::LocalVectors& localvectorsR, const Fada::LocalGlobal::VectorMap& vectormap, const Alat::SystemVectorInterface* datavector) const
{
  _omega = 0.0;
  _Tmin = std::numeric_limits<double>::max();
  _Tmax = std::numeric_limits<double>::lowest();
}

/*--------------------------------------------------------------------------*/

void StatisticsDomainIntegrator::postProcessCell(AlatEnums::residualstatus& status, double weight) const
{
  const Fada::FemFunction& u = (*_UL);
  Alat::AssembleVector& floc = (*_fL);
  _omega += weight;
  for(int icomp = 0; icomp < getVarInfo()->getNCompIn(_varnamein); icomp++)
  {
    double ud = u(icomp);
    floc(0,0) += weight*ud;
    if(ud > _Tmax)
    {
      _Tmax = ud;
    }
    if(ud < _Tmin)
    {
      _Tmin = ud;
    }
    assert(ud != 0.0);
  }
  // std::cerr << "floc[indexout][0][0]="<<floc[indexout][0][0]<<"\n";
}
