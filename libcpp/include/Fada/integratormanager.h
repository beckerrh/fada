#ifndef __Fada_IntegratorManager_h
#define __Fada_IntegratorManager_h

#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/vector.h"
#include  "integratorinterface.h"
#include  "Alat/interfacebase.h"
#include  "termanddomain.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DomainSolverInterface;
  class IntegratorInterface;
  class SolverInterface;

  class IntegratorManager : public Alat::InterfaceBase
  {
public:
    typedef Alat::Set<const Fada::IntegratorInterface*> IntegratorSet;
    typedef Alat::Map<std::string, IntegratorInterface*>::const_iterator const_iterator_all;
    typedef Alat::Map<std::string, IntegratorInterface*>::iterator iterator_all;
    typedef IntegratorSet::const_iterator const_iterator;
    typedef Alat::Map<FadaEnums::term, IntegratorSet> IntegratorsForTerm;
    typedef Alat::Map<TermAndDomain, IntegratorSet> IntegratorsForTermAndDomain;

protected:
    Alat::Map<std::string, IntegratorInterface*> _integratormap;
    IntegratorsForTerm  _integrators_of_term;
    IntegratorsForTermAndDomain _integrators_for_term_and_domain;
    std::string getInterfaceName() const;

public:
    ~IntegratorManager();
    IntegratorManager();
    IntegratorManager( const IntegratorManager& integratormanager);
    IntegratorManager& operator=( const IntegratorManager& integratormanager);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;

    bool noIntegrator(FadaEnums::domain domain) const;
    bool noIntegrator(FadaEnums::term term) const;
    bool noIntegrator(FadaEnums::term t, FadaEnums::domain d) const;
    bool hasIntegrator(FadaEnums::domain domain) const;
    bool hasIntegrator(FadaEnums::term term) const;
    bool hasIntegrator(FadaEnums::term t, FadaEnums::domain d) const;
    const_iterator begin(FadaEnums::term term) const;
    const_iterator end(FadaEnums::term term) const;
    const_iterator_all begin_all() const;
    const_iterator_all end_all() const;
    iterator_all begin_all();
    iterator_all end_all();
    void initIntegrators(const Fada::SolverInterface* solver);
    void setData(const std::string& integratorname, const std::string& dataname, const DataInterface* data);
    void setDataIfDefined(const std::string& integratorname, const std::string& dataname, const DataInterface* data);
    // void setParameter(const std::string& integratorname, const std::string& parametername, double d);
    void setParameterIfDefined(const std::string& integratorname, const std::string& parametername, double d);
    void setParameterIfDefined(const std::string& integratorname, const std::string& parametername, std::string s);
    void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit);
    void setTimeScheme(std::string time_discretization);
    void setProblemData(const ProblemData* problemdata);

    IntegratorInterface* getIntegrator(const std::string& integratorname);
    const IntegratorInterface* getIntegrator(const std::string& integratorname) const;
    const IntegratorSet& getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const;
    void add(std::string integrator_name, IntegratorInterface* integrator, std::string output, std::string input, std::string data = "{}");
    void defineIntegratorMaps();

    void setLavrentievParameter(double parameter) const;

    // const Alat::StringSet& getIntegrationFormulaeForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const;
    // const IntegratorSet& getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d, const std::string& formulaname ) const;
    // void setRightFemManager(const FemManagerInterface* femmanager, FadaEnums::domain domain);
  };
}

/*--------------------------------------------------------------------------*/

#endif
