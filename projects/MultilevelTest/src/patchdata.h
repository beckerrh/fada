#ifndef ___PatchData_h
#define ___PatchData_h

#include  "Alat/armadillo.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class SystemMatrixInterface;
  class SystemVectorInterface;
  class VariableVector;
}

class PatchData
{
public:
  arma::vec grad, grad2, diff, a0, a1;
  arma::ivec jN, i0, i1;
  PatchData(int n)
  {
    grad.zeros(n);
    grad2.zeros(n);
    diff.zeros(n);
    a0.zeros(n);
    a1.zeros(n);
    jN.zeros(n);
    i0.zeros(n);
    i1.zeros(n);
  }
};

/*--------------------------------------------------------------------------*/
#endif
