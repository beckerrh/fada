#ifndef __Alat_SparsityPatternWithRowIndexSoft_h
#define __Alat_SparsityPatternWithRowIndexSoft_h

#include  "map.h"
#include  "set.h"
#include  "vector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternWithRowIndexSoft : public Alat::Map<int, Alat::IntSet>
  {
public:
    typedef Alat::Map<int, Alat::IntSet>::const_iterator const_iterator;
    typedef Alat::Map<int, Alat::IntSet>::iterator iterator;

public:
    ~SparsityPatternWithRowIndexSoft();
    SparsityPatternWithRowIndexSoft();
    SparsityPatternWithRowIndexSoft(const SparsityPatternWithRowIndexSoft& S);
    SparsityPatternWithRowIndexSoft& operator=(const SparsityPatternWithRowIndexSoft& S);
    std::string getName() const;

    Alat::IntSet& row(int i);

    int ntotal() const;
    void reInitTranspose(const SparsityPatternWithRowIndexSoft& s);
  };
}

/*---------------------------------------------------------*/

#endif
