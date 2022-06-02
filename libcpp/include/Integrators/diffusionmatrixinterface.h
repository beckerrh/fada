#ifndef __Integrators_DiffusionMatrixInterface_h
#define __Integrators_DiffusionMatrixInterface_h

#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"
#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class DiffusionMatrixInterface : public Alat::InterfaceBase
  {
protected:
  std::string getInterfaceName() const;

public:
    ~DiffusionMatrixInterface();
    DiffusionMatrixInterface();
    DiffusionMatrixInterface( const DiffusionMatrixInterface& diffusionmatrixinterface);
    DiffusionMatrixInterface& operator=( const DiffusionMatrixInterface& diffusionmatrixinterface);
    std::string getName() const;

    virtual void Diffusion(Alat::DoubleMatrix& K, const Alat::DoubleVector& u, double x, double y, double z, double t) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
