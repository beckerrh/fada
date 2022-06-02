#ifndef __Alat_AssembleMatrix_h
#define __Alat_AssembleMatrix_h

#include  "doublematrix.h"
#include  "doublevector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class AssembleVector;
  class AssembleMatrix : public Alat::DoubleVector
  {
public:
    typedef Alat::DoubleMatrix::iterator iterator;
    typedef Alat::DoubleMatrix::const_iterator const_iterator;

private:
    int _ncomp, _mcomp, _n, _m, _nmc;

public:
    ~AssembleMatrix();
    AssembleMatrix();
    AssembleMatrix(const AssembleMatrix& A);
    // AssembleMatrix(int ncomp, int mcomp);
    AssembleMatrix& operator=(const AssembleMatrix& A);

    std::string getName() const;
    int ncomp() const;
    int mcomp() const;
    int n() const;
    int m() const;
    const double& operator()(int icomp, int jcomp, int i, int j) const;
    double& operator()(int icomp, int jcomp, int i, int j);
    void set_size(int ncomp, int mcomp, int n, int m);
    void set_size(int ncomp, int n);
    void set_size(int n);
    void reInitTranspose(const AssembleMatrix& A);
    void productVector(AssembleVector& out, const Alat::AssembleVector& in, double d=1.0) const;
    void productVectorTransposed(AssembleVector& out, const Alat::AssembleVector& in, double d=1.0) const;
  };
}

/*---------------------------------------------------------*/

#endif
