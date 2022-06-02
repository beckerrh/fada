#ifndef __Fada_InitialConditionInterface_h
#define __Fada_InitialConditionInterface_h

#include  "data.h"
#include  "Alat/doublevector.h"

/*--------------------------------------------------------*/

namespace Fada
{
  class InitialConditionInterface : public Fada::Data
  {
protected:
    std::string getInterfaceName() const
    {
      return "InitialConditionInterface";
    }

public:
    ~InitialConditionInterface() {}
    InitialConditionInterface() : Fada::Data() {}
    InitialConditionInterface(const InitialConditionInterface& DI) : Fada::Data()
    {
      assert(0);
    }

    InitialConditionInterface& operator=(const InitialConditionInterface& DI)
    {
      assert(0);
      return *this;
    }

    virtual void getValue(Alat::DoubleVector& g, double x, double y, double z) const = 0;
  };
}


#endif
