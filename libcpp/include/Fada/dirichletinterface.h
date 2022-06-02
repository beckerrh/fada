#ifndef __Fada_DirichletInterface_h
#define __Fada_DirichletInterface_h

#include  "data.h"

/*--------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
  template <class T>
  class Vector;  
}

namespace Fada
{
  class DirichletInterface : public virtual Fada::Data
  {
public:
    ~DirichletInterface();
    DirichletInterface();
    DirichletInterface(const DirichletInterface& dirichlet);
    DirichletInterface& operator=(const DirichletInterface& dirichlet);

    virtual void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const = 0;
    virtual void getGradient(Alat::Vector<Alat::Node>& grad, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };
}


#endif
