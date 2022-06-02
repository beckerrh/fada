#ifndef __Integrators_RightHandSideDomainIntegrator_h
#define __Integrators_RightHandSideDomainIntegrator_h

#include  "Fada/integrator.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class RightHandSideInterface;
}

namespace Integrators
{
  class RightHandSideDomainIntegrator : public Fada::Integrator
  {
protected:
    Alat::Map<std::string, const Fada::RightHandSideInterface*> _righthandside;
    mutable Alat::Map<std::string, Alat::DoubleVector> _urhs;
    void initData();

public:
    ~RightHandSideDomainIntegrator();
    RightHandSideDomainIntegrator();
    RightHandSideDomainIntegrator( const RightHandSideDomainIntegrator& righthandsideintegrator);
    RightHandSideDomainIntegrator& operator=( const RightHandSideDomainIntegrator& righthandsideintegrator);
    RightHandSideDomainIntegrator* clone() const;

    std::string getName() const;

    void setDomainsOfTerms();
    void rhsCell(AlatEnums::residualstatus& status, double weight) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
