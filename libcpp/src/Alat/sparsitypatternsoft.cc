#include  "Alat/sparsitypatternsoft.h"
#include  <cassert>

using namespace Alat;

/*---------------------------------------------------------*/

SparsityPatternSoft::SparsityPatternSoft() : Alat::Vector<Alat::IntSet>( ) {}

/*---------------------------------------------------------*/

SparsityPatternSoft::SparsityPatternSoft(int n) : Alat::Vector<Alat::IntSet>(n) {}

/*---------------------------------------------------------*/

SparsityPatternSoft::SparsityPatternSoft(const SparsityPatternSoft& s) : Alat::Vector<Alat::IntSet>(s) {}

/*---------------------------------------------------------*/

std::string SparsityPatternSoft::getName() const
{
  return "SparsityPatternSoft";
}

/*---------------------------------------------------------*/

int SparsityPatternSoft::ntotal() const
{
  int n = Alat::Vector<Alat::IntSet>::size();
  int ntotal = 0;
  for(int i = 0; i < n; i++)
  {
    ntotal += rowsize(i);
  }
  return ntotal;
}

/*---------------------------------------------------------*/

int SparsityPatternSoft::rowsize(int i) const
{
  return ( *this )[i].size();
}

void SparsityPatternSoft::set_size(int n)
{
  Alat::Vector<Alat::IntSet>::set_size(n);
}

/*---------------------------------------------------------*/

void SparsityPatternSoft::reInitTranspose(int n, const SparsityPatternSoft& s)
{
  Alat::Vector<Alat::IntSet>::set_size(n);
  for(size_t i = 0; i < s.size(); i++)
  {
    for(std::set<int>::const_iterator q = s[i].begin(); q != s[i].end(); q++)
    {
      assert(*q < n);
      Alat::Vector<Alat::IntSet>::operator[](* q).insert(i);
    }
  }
}
