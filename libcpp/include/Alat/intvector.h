#ifndef __Alat_IntVector_h
#define __Alat_IntVector_h

#include  <vector>
#include  "armadillo.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class IntVector : public arma::Col<int>
  {
public:
    ~IntVector();
    IntVector();
    IntVector(const IntVector& v);
    IntVector(size_type n);
    IntVector& operator=(const IntVector& v);
    IntVector& operator=(const std::vector<int>& v);
    bool operator==(const IntVector& v) const;

    std::ostream& writeBin(std::ostream& out) const;
    std::istream& readBin (std::istream& in);
    std::ostream& saveFada(std::ostream& out, const std::string datatype = "binary") const;
    std::istream& loadFada(std::istream& in);
  };
}

/*---------------------------------------------------------*/

#endif
