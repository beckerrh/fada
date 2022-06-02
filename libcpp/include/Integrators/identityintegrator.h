#ifndef __Integrators_IdentityIntegrator_h
#define __Integrators_IdentityIntegrator_h

#include  "Fada/integratorsinglevariable.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class IdentityIntegrator : public Fada::IntegratorSingleVariable
  {
private:
    double _coefficient;

public:
    ~IdentityIntegrator();
    IdentityIntegrator();
    IdentityIntegrator( const IdentityIntegrator& identityintegrator);
    IdentityIntegrator& operator=( const IdentityIntegrator& identityintegrator);
    std::string getName() const;
    IdentityIntegrator* clone() const;

    void initData();
    void setDomainsOfTerms();

    void formCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
