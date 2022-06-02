#ifndef __Fada_LocalDofInformation_h
#define __Fada_LocalDofInformation_h

#include  "Alat/fixarray.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LocalDofInformation :  Alat::FixArray<4, int>
  {
private:
protected:
public:
    ~LocalDofInformation();
    LocalDofInformation();
    LocalDofInformation( const LocalDofInformation& localdofinformation);
    LocalDofInformation& operator=( const LocalDofInformation& localdofinformation);
    std::string getName() const;
    LocalDofInformation* clone() const;
    int& nodes();
    int& edges();
    int& sides();
    int& cells();
    int nodes() const;
    int edges() const;
    int sides() const;
    int cells() const;
  };
  std::ostream& operator<<(std::ostream& os, const LocalDofInformation& A);
}

/*--------------------------------------------------------------------------*/

#endif
