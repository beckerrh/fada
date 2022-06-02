#include  "Alat/assemblevector.h"
#include  "stdlib.h"
#include  <cassert>

using namespace Alat;

/*---------------------------------------------------------*/
AssembleVector::~AssembleVector() {}
AssembleVector::AssembleVector() : Alat::DoubleMatrix() {}
AssembleVector::AssembleVector(const AssembleVector& A) : Alat::DoubleMatrix(A) {}
AssembleVector& AssembleVector::operator=(const AssembleVector& A)
{
  Alat::DoubleMatrix::operator=(A);
  return *this;
}

std::string AssembleVector::getName() const
{
  return "AssembleVector";
}
std::ostream& Alat::operator<<(std::ostream& os, const AssembleVector& g)
{
  os << "ncomp nloc " << g.ncomp() << " " << g.nloc() << "\n";
  for(int icomp=0; icomp<g.ncomp();icomp++)
  {
    for(int i=0; i<g.nloc();i++)
    {
      os << g(icomp,i)<<" ";
    }
    os << "\n";
  }
  return os;
}

/*---------------------------------------------------------*/
int AssembleVector::ncomp() const
{
  return Alat::DoubleMatrix::n_rows;
}

int AssembleVector::nloc() const
{
  return n_cols;
}

/*---------------------------------------------------------*/
void AssembleVector::set_size(int ncomp, int nloc)
{
  // std::cerr << "AssembleVector::reInit( ncomp="<<ncomp<<" nloc="<<nloc<<"\n";
  Alat::DoubleMatrix::set_size(ncomp, nloc);
  zeros();
  // std::cerr << "after reInit: " << *this << "\n";
  // std::cerr << "after reInit: _n=" << ncomp << " _m="<< n() << "\n";
  // std::cerr << "after reInit: _n=" << _n << " _m="<< _m << "\n";
}
void AssembleVector::set_size(const AssembleVector& v)
{
  AssembleVector::set_size( v.ncomp(), v.nloc() );
}

/*---------------------------------------------------------*/
double AssembleVector::operator*(const AssembleVector& v) const
{
  assert( size() == v.size() );
  double d = 0.0;
  for(int icomp = 0; icomp < ncomp(); icomp++)
  {
    for(int i = 0; i < nloc(); i++)
    {
      d += ( *this )(icomp,i)*v(icomp,i);
    }
  }
  return d;
}