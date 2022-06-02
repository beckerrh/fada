#include  "Alat/assemblematrix.h"
#include  "Alat/assemblevector.h"
#include  "stdlib.h"
#include  <cassert>

using namespace Alat;

/*---------------------------------------------------------*/

AssembleMatrix::~AssembleMatrix() {}

/*---------------------------------------------------------*/

AssembleMatrix::AssembleMatrix() : Alat::DoubleVector(), _ncomp(-1), _mcomp(-1), _n(-1), _m(-1) {}

/*---------------------------------------------------------*/

AssembleMatrix::AssembleMatrix(const AssembleMatrix& A) : Alat::DoubleVector(1), _ncomp(A._ncomp), _mcomp(A._mcomp), _n(A._n), _m(A._m) {}

/*---------------------------------------------------------*/

// AssembleMatrix::AssembleMatrix(int ncomp, int mcomp) : Alat::DoubleVector(), _ncomp(ncomp), _mcomp(mcomp), _n(-1), _m(-1), _nmc(ncomp*mcomp) {}

/*---------------------------------------------------------*/

AssembleMatrix& AssembleMatrix::operator=(const AssembleMatrix& A)
{
  Alat::DoubleVector::operator=(A);
  _ncomp = A._ncomp;
  _mcomp = A._mcomp;
  _n = A._n;
  _m = A._m;
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

std::string AssembleMatrix::getName() const
{
  return "AssembleMatrix";
}

/*---------------------------------------------------------*/

int AssembleMatrix::ncomp() const
{
  return _ncomp;
}

/*---------------------------------------------------------*/

int AssembleMatrix::mcomp() const
{
  return _mcomp;
}

/*---------------------------------------------------------*/

int AssembleMatrix::n() const
{
  return _n;
}

/*---------------------------------------------------------*/

int AssembleMatrix::m() const
{
  return _m;
}

/*---------------------------------------------------------*/

const double& AssembleMatrix::operator()(int icomp, int jcomp, int i, int j) const
{
  assert(i<_n);
  assert(j<_m);
  return ( *this )[_nmc*( j+_m*i ) + jcomp+icomp*_mcomp];
}

/*---------------------------------------------------------*/

double& AssembleMatrix::operator()(int icomp, int jcomp, int i, int j)
{
  assert(i<_n);
  assert(j<_m);
  return ( *this )[_nmc*( j+_m*i ) + jcomp+icomp*_mcomp];
}

/*---------------------------------------------------------*/

void AssembleMatrix::set_size(int n)
{
  std::cerr << "*** ERROR in AssembleMatrix::set_size(int n): please use this function with two or four arguments (ncomp, n) (ncomp,mcomp, n, m)\n";
  assert(0);
  exit(1);
}

/*---------------------------------------------------------*/
void AssembleMatrix::set_size(int ncomp, int mcomp, int n, int m)
{
  _ncomp = ncomp;
  _mcomp = mcomp;
  _nmc = _ncomp*_mcomp;
  _n = n;
  _m = m;
  assert(_n>=0);
  assert(_m>=0);
  Alat::DoubleVector::set_size(_ncomp*_mcomp*_n*_m);
  Alat::DoubleVector::zeros();
}

/*---------------------------------------------------------*/
void AssembleMatrix::set_size(int ncomp, int n)
{
  set_size(ncomp, ncomp, n, n);
}

/*---------------------------------------------------------*/
void AssembleMatrix::reInitTranspose(const AssembleMatrix& A)
{
  AssembleMatrix::set_size( A.mcomp(), A.ncomp(), A.m(), A.n() );
  for(int icomp = 0; icomp < _ncomp; icomp++)
  {
    for(int jcomp = 0; jcomp < _mcomp; jcomp++)
    {
      for(int i = 0; i < _n; i++)
      {
        for(int j = 0; j < _m; j++)
        {
          ( *this )( icomp, jcomp, i, j ) = A(jcomp, icomp, j, i);
        }
      }
    }
  }
}

/*---------------------------------------------------------*/
void AssembleMatrix::productVector(AssembleVector& out, const Alat::AssembleVector& in, double d) const
{
  assert(_ncomp==out.ncomp());
  assert(_mcomp==in.ncomp());
  assert(_n==out.nloc());
  assert(_m==in.nloc());
  for(int icomp = 0; icomp < _ncomp; icomp++)
  {
    for(int jcomp = 0; jcomp < _mcomp; jcomp++)
    {
      for(int i = 0; i < _n; i++)
      {
        for(int j = 0; j < _m; j++)
        {
          // out[icomp][i] += d*( *this )( icomp, jcomp, i, j ) * in[jcomp][j];
          out(icomp,i) += d*( *this )( icomp, jcomp, i, j ) * in(jcomp,j);
        }
      }
    }
  }
}
/*---------------------------------------------------------*/
void AssembleMatrix::productVectorTransposed(AssembleVector& out, const Alat::AssembleVector& in, double d) const
{
  for(int icomp = 0; icomp < _ncomp; icomp++)
  {
    for(int jcomp = 0; jcomp < _mcomp; jcomp++)
    {
      for(int i = 0; i < _n; i++)
      {
        for(int j = 0; j < _m; j++)
        {
          // out[jcomp][j] += d*( *this )( icomp, jcomp, i, j ) * in[icomp][i];
          out(jcomp,j) += d*( *this )( icomp, jcomp, i, j ) * in(icomp,i);
        }
      }
    }
  }
}
