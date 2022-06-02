#include  "Alat/sparsitypatternfixarraysoft.h"

using namespace Alat;
using namespace std;

/*-------------------------------------------------------------*/

template<int N>
SparsityPatternFixArraySoft<N>::~SparsityPatternFixArraySoft()
{}

/*-------------------------------------------------------------*/

template<int N>
SparsityPatternFixArraySoft<N>::SparsityPatternFixArraySoft() : Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >()
{}

/*-------------------------------------------------------------*/

template<int N>
SparsityPatternFixArraySoft<N>::SparsityPatternFixArraySoft(const SparsityPatternFixArraySoft& S) : Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >(S)
{}

/*-------------------------------------------------------------*/

template<int N>
SparsityPatternFixArraySoft<N>::SparsityPatternFixArraySoft(int n) : Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >(n) {}

/*-------------------------------------------------------------*/

template<int N>
SparsityPatternFixArraySoft<N>& SparsityPatternFixArraySoft<N>::operator=(const SparsityPatternFixArraySoft<N>& S)
{
  Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >::operator=(S);
  return *this;
}

/*-------------------------------------------------------------*/

template<int N>
std::string SparsityPatternFixArraySoft<N>::getName() const
{
  return "SparsityPatternFixArraySoft";
}

/*-------------------------------------------------------------*/

template<int N>
int SparsityPatternFixArraySoft<N>::ntotal() const
{
  int n = Alat::Vector<Alat::Set<Alat::FixArray<N, int> > >::size();
  int ntotal = 0;
  for(int i = 0; i < n; i++)
  {
    ntotal += ( *this )[i].size();
  }
  return ntotal;
}

/*-------------------------------------------------------------*/

template<int N>
int SparsityPatternFixArraySoft<N>::rowsize(int i) const
{
  return ( *this )[i].size();
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

template class Alat::SparsityPatternFixArraySoft<2>;
