#ifndef __Alat_DoubleVector_h
#define __Alat_DoubleVector_h

#include  "armadillo.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class Node;

  class DoubleVector : public arma::Col<double>
  {
public:
    ~DoubleVector();
    DoubleVector();
    DoubleVector(const arma::Col<double>& v);
    DoubleVector(const DoubleVector& v);
    DoubleVector(size_type n, double d=0.0);

    DoubleVector& operator=(const arma::Col<double>& v);
    DoubleVector& operator=(const DoubleVector& v);

    double operator*(const DoubleVector& v) const;
    double operator*(const Alat::Node& v) const;
    double scalarProduct(const DoubleVector& v) const;

    void add(double d, const DoubleVector& v);
    void equal(double d);
    void equal(const DoubleVector& v);
    void equal(double d, const DoubleVector& v);
    void equal(double d1, const DoubleVector& v1, double d2, const DoubleVector& v2);
    double norm() const;
    void scale(const double& d);

    std::ostream& writeBin(std::ostream& out) const;
    std::istream& readBin (std::istream& in);
    std::ostream& saveFada(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& loadFada(std::istream& in);
  };
}

/*---------------------------------------------------------*/

#endif
