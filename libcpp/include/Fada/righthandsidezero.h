#ifndef __Fada_RightHandSideZero_h
#define __Fada_RightHandSideZero_h

#include  "righthandsideinterface.h"

/*--------------------------------------------------------*/

namespace Fada
{
//--------------------------------------------------------------------------------
// Classe pour un second membre null
//--------------------------------------------------------------------------------
  class RightHandSideZero : public Fada::RightHandSideInterface
  {
public:
    RightHandSideZero() : Fada::RightHandSideInterface() {}
    std::string getName() const
    {
      return "RightHandSideZero";
    }

    void basicInit() {}
    void getValue(Alat::DoubleVector& g, double x, double y, double z, double t = 0.0) const
    {
      g.zeros();
    }
  };
}

#endif
