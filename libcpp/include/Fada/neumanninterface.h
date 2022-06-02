#ifndef __Fada__NeumannInterface_h
#define __Fada__NeumannInterface_h

#include  "data.h"

/*--------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
}

namespace Fada
{
  class NeumannInterface : public virtual Fada::Data
  {
protected:
    std::string getInterfaceName() const;

public:
    ~NeumannInterface();
    NeumannInterface();
    NeumannInterface(const NeumannInterface& neumanninterface);
    NeumannInterface& operator=(const NeumannInterface& neumanninterface);
    virtual void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const = 0;
  };
}


#endif
