#ifndef __Alat_AssembleVector_h
#define __Alat_AssembleVector_h

// #include  "doublevector.h"
#include  "doublematrix.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class AssembleVector : public Alat::DoubleMatrix
  {
public:
    ~AssembleVector(),
    AssembleVector();
    AssembleVector(const AssembleVector& A);
    AssembleVector& operator=(const AssembleVector& A);

    std::string getName() const;
    int ncomp() const;
    int nloc() const;
    void set_size(int ncomp, int n);
    void set_size(const AssembleVector& v);
    double operator*(const AssembleVector& v) const;
  };
  std::ostream& operator<<(std::ostream& os, const AssembleVector& g);
}

/*---------------------------------------------------------*/

#endif
