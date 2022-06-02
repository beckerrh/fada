#ifndef __Alat_SparsityPatternFixArraySoft_h
#define __Alat_SparsityPatternFixArraySoft_h

#include  "fixarray.h"
#include  "intvector.h"
#include  "set.h"
#include  "vector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  template<int N>
  class SparsityPatternFixArraySoft : public Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >
  {
public:
    typedef typename Alat::Set<Alat::FixArray<N, int> >::const_iterator const_iterator;

public:
    ~SparsityPatternFixArraySoft();
    SparsityPatternFixArraySoft();
    SparsityPatternFixArraySoft(const SparsityPatternFixArraySoft& S);
    SparsityPatternFixArraySoft(int n);
    SparsityPatternFixArraySoft& operator=(const SparsityPatternFixArraySoft& S);

    std::string getName() const;

    int ntotal() const;
    int rowsize(int i) const;
  };
}

/*---------------------------------------------------------*/

#endif
