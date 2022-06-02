#ifndef __Integrators_BoundaryFunctionalIntegrator_h
#define __Integrators_BoundaryFunctionalIntegrator_h

#include  "Fada/integrator.h"
#include  "Alat/systemassemblevector.h"
#include  "Alat/doublevector.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class BoundaryFunctionalIntegrator : public Fada::Integrator
  {
protected:
    int _color, _ncomp;
    double _scale;
    mutable std::string _bdrycond, _varname;
    const Fada::IntegratorInterface* _integrator;
    mutable Alat::SystemAssembleVector _flocLother, _flocRother;

public:
    ~BoundaryFunctionalIntegrator();
    BoundaryFunctionalIntegrator(const Fada::IntegratorInterface* integrator, int color, int ncomp, double scale = 1.0, std::string bdrycond = "Dirichlet");
    BoundaryFunctionalIntegrator( const BoundaryFunctionalIntegrator& dragandliftintegrator);
    BoundaryFunctionalIntegrator& operator=( const BoundaryFunctionalIntegrator& dragandliftintegrator);
    std::string getName() const;
    BoundaryFunctionalIntegrator* clone() const;

    void initData();
    void setDomainsOfTerms();
    Fada::VariableInterface* constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const;
    void postProcessBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    void setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const;
    void setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
