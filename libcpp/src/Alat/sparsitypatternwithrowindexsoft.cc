#include  "Alat/sparsitypatternwithrowindexsoft.h"

using namespace Alat;

/*---------------------------------------------------------*/

SparsityPatternWithRowIndexSoft::~SparsityPatternWithRowIndexSoft()
{}

/*---------------------------------------------------------*/

SparsityPatternWithRowIndexSoft::SparsityPatternWithRowIndexSoft() : Alat::Map<int, Alat::IntSet>() {}

/*---------------------------------------------------------*/

SparsityPatternWithRowIndexSoft::SparsityPatternWithRowIndexSoft(const SparsityPatternWithRowIndexSoft& S) : Alat::Map<int, Alat::IntSet>(S) {}

/*---------------------------------------------------------*/

SparsityPatternWithRowIndexSoft& SparsityPatternWithRowIndexSoft::operator=(const SparsityPatternWithRowIndexSoft& S)
{
  Alat::Map<int, Alat::IntSet>::operator=(S);
  return *this;
}

/*---------------------------------------------------------*/

std::string SparsityPatternWithRowIndexSoft::getName() const
{
  return "SparsityPatternWithRowIndexSoft";
}

/*---------------------------------------------------------*/

Alat::IntSet& SparsityPatternWithRowIndexSoft::row(int i)
{
  return ( *this )[i];
}

/*---------------------------------------------------------*/

int SparsityPatternWithRowIndexSoft::ntotal() const
{
  int ntot = 0;
  for(const_iterator p = this->begin(); p != this->end(); p++)
  {
    ntot += p->second.size();
  }
  return ntot;
}

/*---------------------------------------------------------*/

void SparsityPatternWithRowIndexSoft::reInitTranspose(const SparsityPatternWithRowIndexSoft& s)
{
  Alat::Map<int, Alat::IntSet>::clear();
  for(const_iterator p = s.begin(); p != s.end(); p++)
  {
    int i = p->first;
    for(std::set<int>::const_iterator q = p->second.begin(); q != p->second.end(); q++)
    {
      int j = *q;
      row(j).insert(i);
    }
  }
}
