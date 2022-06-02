#include  "Alat/sparsitypattern.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/intvector.h"
#include  "Alat/sortermetis.h"
#include  <cassert>
#include  <algorithm>
#ifdef METIS
#include  "metis.h"
#endif

using namespace Alat;

/*--------------------------------------------------------------------------*/
SorterMetis::~SorterMetis(){}
SorterMetis::SorterMetis() : Alat::SorterInterface(), _pinv(NULL){}
SorterMetis::SorterMetis(Alat::IntVector& pinv) : Alat::SorterInterface(), _pinv(&pinv){}
SorterMetis::SorterMetis( const SorterMetis& sortermetis) : Alat::SorterInterface(sortermetis), _pinv(sortermetis._pinv)
{
  assert(0);
}
SorterMetis& SorterMetis::operator=( const SorterMetis& sortermetis)
{
  Alat::SorterInterface::operator=(sortermetis);
  assert(0);
  return *this;
}
std::string SorterMetis::getName() const
{
  return "SorterMetis";
}
SorterMetis* SorterMetis::clone() const
{
  assert(0);
//return new SorterMetis(*this);
}

/*--------------------------------------------------------------------------*/
void SorterMetis::sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const
{
    sort( p, matrix->getSparsityPattern() );
}

/*--------------------------------------------------------------------------*/
#if defined (METIS) 
void SorterMetis::sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const
{
  idx_t* vwgt = NULL;
  idx_t* options = NULL;

  int n = sparsitypattern->n();
  int ntotal = sparsitypattern->ntotal();
  assert(n <= ntotal);
  idx_t nvtxs[1];
  nvtxs[0] = n;
  idx_t* xadj = new idx_t[n+1];
  for(int i = 0; i < n+1; i++)
  {
    xadj[i] = sparsitypattern->rowstart(i)-i;
  }
  int count = 0;
  // int count2 = 0;
  idx_t* adjncy = new idx_t[ntotal-n];
  for(int i = 0; i < n; i++)
  {
    for(int pos = sparsitypattern->rowstart(i); pos != sparsitypattern->rowstop(i); pos++)
    { 
      // count2++;
      int j = sparsitypattern->col(pos);
      if(j != i)
      {
        adjncy[count++] = j;
      }
    }
  }
  // assert(count2 == ntotal);
  assert(count == ntotal-n);

  if(_pinv)
  {
    int metis = METIS_NodeND( nvtxs, xadj, adjncy, vwgt, options, &( p[0] ), &( ( *_pinv )[0] ) );
  }
  else
  {
    Alat::IntVector pinv( p.size() );
    int metis = METIS_NodeND( nvtxs, xadj, adjncy, vwgt, options, &( p[0] ), &( pinv[0] ) );
  }

  delete[] xadj;
  delete[] adjncy;
}

#else
void SorterMetis::sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const
{
  _error_string("sort", "requires metis !");
}

#endif
