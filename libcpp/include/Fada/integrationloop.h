#ifndef __Fada_IntegrationLoop_h
#define __Fada_IntegrationLoop_h

#include  "Fada/integratorinterface.h"
#include  "Fada/localvectors.h"
#include  "Fada/termanddomain.h"
#include  "Alat/map.h"
#include  "Alat/stringset.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
}
namespace Fada
{
  class FemManagerInterface;
  class IntegrationFormulaInterface;
  class IntegratorManager;
}
namespace Fada
{
  class IntegrationLoop
  {
protected:
    const Alat::ParameterFile* _parameterfile;
    std::string _couplingtype;
    const Fada::IntegratorManager* _integratormanager;
    const Fada::FemManagerInterface* _femmanager, * _femmanagerR;
    mutable Fada::LocalVectors _localvectors, _localvectorsR, _localvectors_edge;
    const FadaMesh::MeshInterface* _mesh;

    Alat::Map<std::string, const Fada::IntegrationFormulaInterface*> _formulae;
    typedef Alat::Set<const Fada::IntegratorInterface*> IntegratorSet;
    typedef Alat::Map<Fada::TermAndDomain, Alat::Map<std::string, IntegratorSet> > IntegratorsForTermAndDomainAndFormula;
    IntegratorsForTermAndDomainAndFormula _integrators_for_term_and_domain_and_formula;
    Alat::Map<Fada::TermAndDomain, Alat::StringSet> _integrationformulaefortermanddomain;

    const Alat::StringSet& getIntegrationFormulaeForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const;
    const IntegratorSet& getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d, const std::string& formulaname ) const;

public:
    ~IntegrationLoop();
    IntegrationLoop();
    IntegrationLoop( const IntegrationLoop& integrationloop);
    IntegrationLoop& operator=( const IntegrationLoop& integrationloop);
    std::string getName() const;
    IntegrationLoop* clone() const;

    void basicInit(const FadaMesh::MeshInterface* mesh, const Fada::IntegratorManager* integratormanager, const Fada::FemManagerInterface* femmanager, const Fada::FemManagerInterface* femmanager_right, const Alat::ParameterFile* parameterfile, std::string couplingtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
