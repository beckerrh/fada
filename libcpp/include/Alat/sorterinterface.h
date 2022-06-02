#ifndef __Alat_SorterInterface_h
#define __Alat_SorterInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
}
namespace Alat
{
  class VariableMatrixInterface;
  class VectorInterface;
}
namespace Alat
{
  class SorterInterface : public Alat::InterfaceBase
  {
public:
    ~SorterInterface();
    SorterInterface();
    SorterInterface( const SorterInterface& sorterinterface);
    SorterInterface& operator=( const SorterInterface& sorterinterface);
    std::string getInterfaceName() const;
    SorterInterface* clone() const;

    virtual void sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const;
    virtual void sort(Alat::IntVector& p) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
