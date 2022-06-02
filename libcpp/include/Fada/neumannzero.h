#ifndef __Fada_NeumannZero_h
#define __Fada_NeumannZero_h

#include  "neumanninterface.h"

/*--------------------------------------------------------*/

namespace Fada
{
  class NeumannZero : public Fada::NeumannInterface
  {
public:
    NeumannZero() : Fada::NeumannInterface(){}
    std::string getName() const
    {
      return "NeumannZero";
    }

    void basicInit() {}
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const {}
  };
}

#endif
