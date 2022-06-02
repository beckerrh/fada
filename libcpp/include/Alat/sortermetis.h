#ifndef __Alat_SorterMetis_h
#define __Alat_SorterMetis_h

#include  "sorterinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SorterMetis : public Alat::SorterInterface
  {
private:
    Alat::IntVector* _pinv;

public:
    ~SorterMetis();
    SorterMetis();
    SorterMetis(Alat::IntVector& pinv);
    SorterMetis( const SorterMetis& sortermetis);
    SorterMetis& operator=( const SorterMetis& sortermetis);
    std::string getName() const;
    SorterMetis* clone() const;

    void sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const;
    void sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
