#ifndef __Integrators_LaplaceDgIntegrator_h
#define __Integrators_LaplaceDgIntegrator_h

#include  "laplaceintegrator.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class LaplaceDgIntegrator : public LaplaceIntegrator
  {
private:
protected:
public:
    ~LaplaceDgIntegrator();
    LaplaceDgIntegrator();
    LaplaceDgIntegrator( const LaplaceDgIntegrator& laplacedgintegrator);
    LaplaceDgIntegrator& operator=( const LaplaceDgIntegrator& laplacedgintegrator);
    std::string getName() const;
    LaplaceDgIntegrator* clone() const;

    void setDomainsOfTerms();
  };
}

/*--------------------------------------------------------------------------*/

#endif
