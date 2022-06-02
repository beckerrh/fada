#include  "Alat/sparsitypatternwithvectorofset.h"

using namespace Alat;

/*---------------------------------------------------------*/

SparsityPatternWithVectorOfSet::~SparsityPatternWithVectorOfSet() {}

/*---------------------------------------------------------*/

SparsityPatternWithVectorOfSet::SparsityPatternWithVectorOfSet() : Alat::Vector< std::pair<int, Alat::IntSet> >() {}

/*---------------------------------------------------------*/

SparsityPatternWithVectorOfSet::SparsityPatternWithVectorOfSet(const SparsityPatternWithVectorOfSet& S) : Alat::Vector< std::pair<int, Alat::IntSet> >() {}

/*---------------------------------------------------------*/

SparsityPatternWithVectorOfSet& SparsityPatternWithVectorOfSet::operator=( const SparsityPatternWithVectorOfSet& S)
{
  Alat::Vector< std::pair<int, Alat::IntSet>  >::operator=(S);
  return *this;
}

/*---------------------------------------------------------*/

std::string SparsityPatternWithVectorOfSet::getName() const
{
  return "SparsityPatternWithVectorOfSet";
}

/*---------------------------------------------------------*/

int SparsityPatternWithVectorOfSet::ntotal() const
{
  int ntot = 0;
  for(const_iterator p = this->begin(); p != this->end(); p++)
  {
    ntot += p->second.size();
  }
  return ntot;
}
