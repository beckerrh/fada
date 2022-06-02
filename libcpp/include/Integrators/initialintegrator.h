#ifndef __Integrators_InitialIntegrator_h
#define __Integrators_InitialIntegrator_h

#include  "Fada/integrator.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class InitialConditionInterface;
}

namespace Integrators
{
  class InitialIntegrator : public Fada::Integrator
  {
public:
    ~InitialIntegrator();
    InitialIntegrator();
    InitialIntegrator( const InitialIntegrator& initialIntegrator);
    InitialIntegrator& operator=( const InitialIntegrator& initialIntegrator);
    std::string getName() const;
    InitialIntegrator* clone() const;

    void initData();
    void setDomainsOfTerms();
  };
}

/*--------------------------------------------------------------------------*/

#endif
