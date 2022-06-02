#ifndef __Fada_DirichletZero_h
#define __Fada_DirichletZero_h

#include  "dirichletinterface.h"

/*--------------------------------------------------------*/

namespace Fada
{
  class DirichletZero : public Fada::DirichletInterface
  {
public:
    ~DirichletZero();
    DirichletZero();
    DirichletZero(const DirichletZero& dirichlet);
    DirichletZero& operator=(const DirichletZero& dirichlet);
    DataInterface* clone() const;
    std::string getName() const;

    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };
}

#endif
