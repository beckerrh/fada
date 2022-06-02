#ifndef __Integrators_RightHandSideBoundaryIntegrator_h
#define __Integrators_RightHandSideBoundaryIntegrator_h

#include  "Fada/integrator.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class NeumannInterface;
}

namespace Integrators
{
  class RightHandSideBoundaryIntegrator : public Fada::Integrator
  {
protected:
    Alat::Map<std::string, const Fada::NeumannInterface*> _neumann;
    mutable Alat::Map<std::string, Alat::DoubleVector> _uneumann;
    void initData();
    Alat::Map<std::string, std::string> _bdrycondofvar;

public:
    ~RightHandSideBoundaryIntegrator();
    RightHandSideBoundaryIntegrator(const Alat::Map<std::string, std::string>& bdrycondofvar);
    RightHandSideBoundaryIntegrator( const RightHandSideBoundaryIntegrator& righthandsideintegrator);
    RightHandSideBoundaryIntegrator& operator=( const RightHandSideBoundaryIntegrator& righthandsideintegrator);
    RightHandSideBoundaryIntegrator* clone() const;

    std::string getName() const;

    void setDomainsOfTerms();
    void rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
