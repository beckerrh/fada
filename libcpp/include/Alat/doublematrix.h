#ifndef __Alat_DoubleMatrix_h
#define __Alat_DoubleMatrix_h

#include  "armadillo.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class Node;
}
namespace Alat
{
  class DoubleVector;

  class DoubleMatrix : public arma::mat
  {
public:
    ~DoubleMatrix();
    DoubleMatrix();
    DoubleMatrix(const DoubleMatrix& A);
    DoubleMatrix(const arma::mat& A);
    DoubleMatrix(size_type n, size_type m);
    std::string getName() const;

    DoubleMatrix& operator=(const arma::mat& d);
    DoubleMatrix& operator=(const DoubleMatrix& d);

    DoubleMatrix& operator*=(double d);
    void productVector(DoubleVector& y, const DoubleVector& x, double s = 1.) const;
    void productVectorTransposed(DoubleVector& y, const DoubleVector& x, double s = 1.) const;
    void productVector(Alat::Node& y, const Alat::Node& x, double s = 1.) const;
    void productVectorTransposed(Alat::Node& y, const Alat::Node& x, double s = 1.) const;
    void identity();
    void productMatrix(DoubleMatrix& A, const DoubleMatrix& B, double d=1.0) const;
    void productTransposedMatrix(DoubleMatrix& A, const DoubleMatrix& B, double d=1.0) const;
    void productMatrixTransposed(DoubleMatrix& A, const DoubleMatrix& B, double d=1.0) const;

    void add(const DoubleMatrix& B);
    void add(double s, const DoubleMatrix& B);
    void add(const DoubleVector& u, const DoubleVector& v, double s = 1.0);
    double norm() const;

    void gaussJordan();
    void addVectorToRow(int i, const DoubleVector& v, double s = 1);

    std::ostream& writeBin(std::ostream& out) const;
    std::istream& readBin (std::istream& in);
    std::ostream& saveFada(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& loadFada(std::istream& in);
  };
}

/*---------------------------------------------------------*/

#endif
