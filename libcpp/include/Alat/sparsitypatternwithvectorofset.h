#ifndef __Alat_SparsityPatternWithVectorOfSet_h
#define __Alat_SparsityPatternWithVectorOfSet_h

#include  "map.h"
#include  "set.h"
#include  "vector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternWithVectorOfSet : public Alat::Vector< std::pair<int, Alat::IntSet>  >
  {
public:
    typedef Alat::Vector< std::pair<int, Alat::IntSet> >::const_iterator const_iterator;
    typedef Alat::Vector< std::pair<int, Alat::IntSet> >::iterator iterator;

public:
    ~SparsityPatternWithVectorOfSet();
    SparsityPatternWithVectorOfSet();
    SparsityPatternWithVectorOfSet(const SparsityPatternWithVectorOfSet& S);
    SparsityPatternWithVectorOfSet& operator=( const SparsityPatternWithVectorOfSet& S);
    std::string getName() const;
    int ntotal() const;
  };
}

/*---------------------------------------------------------*/

#endif
