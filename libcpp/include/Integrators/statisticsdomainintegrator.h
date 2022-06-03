#ifndef __Integrators_StatisticsDomainIntegrator_h
#define __Integrators_StatisticsDomainIntegrator_h

#include  "Fada/integratorsinglevariable.h"
#include  "Alat/doublevector.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class StatisticsDomainIntegrator : public Fada::IntegratorSingleVariable
  {
protected:
    mutable double _omega, _Tmin, _Tmax;
    void postProcessCell(AlatEnums::residualstatus& status, double weight) const;
    std::string _varnamein, _varnameout;

public:
    ~StatisticsDomainIntegrator();
    StatisticsDomainIntegrator();
    StatisticsDomainIntegrator( const StatisticsDomainIntegrator& integrator);
    StatisticsDomainIntegrator& operator=( const StatisticsDomainIntegrator& integrator);
    std::string getName() const;
    StatisticsDomainIntegrator* clone() const;

    // void initData();
    void setDomainsOfTerms();
    Fada::VariableInterface* constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const;
    void afterIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, Alat::SystemVector* ouput) const;
    void beforeIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, const Fada::LocalGlobal& localglobal, Fada::LocalVectors& localvectors, Fada::LocalVectors& localvectorsR, const Fada::LocalGlobal::VectorMap& vectormap, const Alat::SystemVector* datavector) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
