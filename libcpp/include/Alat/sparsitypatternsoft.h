#ifndef __Alat_SparsityPatternSoft_h
#define __Alat_SparsityPatternSoft_h

#include  "set.h"
#include  "vector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternSoft : public Alat::Vector<Alat::IntSet>
  {
public:
    SparsityPatternSoft();
    SparsityPatternSoft(int n);
    SparsityPatternSoft(const SparsityPatternSoft& s);
    std::string getName() const;

    int ntotal() const;
    int  rowsize(int i) const;
    void set_size(int n);
    void reInitTranspose(int n, const SparsityPatternSoft& s);
  };
}

/*---------------------------------------------------------*/

#endif
