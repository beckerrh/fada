#ifndef __Alat_SorterCuthillMcKee_h
#define __Alat_SorterCuthillMcKee_h

#include  "sorterinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
  class SparsityPattern;
}

namespace Alat
{
  class SorterCuthillMcKee : public Alat::SorterInterface
  {
public:
    ~SorterCuthillMcKee();
    SorterCuthillMcKee();
    SorterCuthillMcKee( const SorterCuthillMcKee& sortercuthillmckee);
    SorterCuthillMcKee& operator=( const SorterCuthillMcKee& sortercuthillmckee);
    std::string getName() const;
    SorterCuthillMcKee* clone() const;

    void sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const;
    void sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
