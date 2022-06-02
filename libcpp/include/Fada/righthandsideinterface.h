#ifndef __Fada_RightHandSideInterface_h
#define __Fada_RightHandSideInterface_h

#include  "data.h"

/*--------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
}

namespace Fada
{

  class RightHandSideInterface : public Fada::Data
  {
protected:
    std::string getInterfaceName() const
    {
      return "RightHandSideInterface";
    }

public:
    ~RightHandSideInterface() {}
    RightHandSideInterface() : Fada::Data() {}
    RightHandSideInterface(const RightHandSideInterface& DI) : Fada::Data()
    {
      assert(0);
    }

    RightHandSideInterface& operator=(const RightHandSideInterface& DI)
    {
      assert(0);
      return *this;
    }

    virtual void getValue(Alat::DoubleVector& f, double x, double y, double z, double t = 0.0) const = 0;
  };
}


#endif
