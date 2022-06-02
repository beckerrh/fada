#ifndef __Integrators_TransportDgIntegrator_h
#define __Integrators_TransportDgIntegrator_h

#include  "transportintegrator.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class TransportDgIntegrator : public TransportIntegrator
  {
public:
    ~TransportDgIntegrator();
    TransportDgIntegrator();
    TransportDgIntegrator( const TransportDgIntegrator& transportdgintegrator);
    TransportDgIntegrator& operator=( const TransportDgIntegrator& transportdgintegrator);
    std::string getName() const;
    TransportDgIntegrator* clone() const;

    void setDomainsOfTerms();
  };
}

/*--------------------------------------------------------------------------*/

#endif
