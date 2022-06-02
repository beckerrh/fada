#include  "Alat/node.h"
#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"
#include  "Alat/intvector.h"
#include  <algorithm>
#include  <cassert>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif
#define ARMA_USE_LAPACK
#include  "Alat/armadillo.h"
#include  <cmath>
#include  <iostream>
#include  <iterator>

using namespace Alat;
using namespace std;

/*----------------------------------------------------------*/

DoubleMatrix::~DoubleMatrix() {}
DoubleMatrix::DoubleMatrix() : arma::mat() {}
DoubleMatrix::DoubleMatrix(const DoubleMatrix& A) : arma::mat(A) {}
DoubleMatrix::DoubleMatrix(const arma::mat& A) : arma::mat(A) {}
DoubleMatrix::DoubleMatrix(size_type n_rows, size_type n_cols) : arma::mat(n_rows, n_cols) {}
std::string DoubleMatrix::getName() const
{
  return "Alat::DoubleMatrix";
}

/*----------------------------------------------------------*/
DoubleMatrix& DoubleMatrix::operator=(const arma::mat& d)
{
  arma::mat::operator=(d);
  return *this;
}
DoubleMatrix& DoubleMatrix::operator=(const DoubleMatrix& d)
{
  assert(n_rows==d.n_rows);
  assert(n_cols==d.n_cols);
  arma::mat::operator=(d);
  return *this;
}

/*----------------------------------------------------------*/
DoubleMatrix& DoubleMatrix::operator*=(double d)
{
  iterator first  = DoubleMatrix::begin();
  const_iterator last   = DoubleMatrix::end();
  while(first != last)
  {
    ( *first++ ) *= d;
  }
  return *this;
}

// void DoubleMatrix::reInit(size_t n_rows, size_t n_cols)
// {
//   set_size(n_rows, n_cols);
// }

/*-----------------------------------------------------------------*/
void DoubleMatrix::productVector(DoubleVector& y, const DoubleVector& x, double s) const
{
  // y = s * *this * x;
  // return;
  
  // matrix-vector-multiplication (using VECTOR template)
  const_iterator p  = DoubleMatrix::begin();
  DoubleVector::iterator py = y.begin();
  DoubleVector::const_iterator px;

  while( p != DoubleMatrix::end() )
  {
    px = x.begin();
    for(int j = 0; j < n_cols; j++)
    {
      *py += s*( *p++ )*( *px++ );
    }
    py++;
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::productVectorTransposed(DoubleVector& y, const DoubleVector& x, double s) const
{
  const_iterator p  = DoubleMatrix::begin();
  DoubleVector::iterator py = y.begin();
  DoubleVector::const_iterator px = x.begin();
  
  while( p != DoubleMatrix::end() )
  {
    py = y.begin();
    for(int j = 0; j < n_cols; j++)
    {
      ( *py++ ) += s*( *p++ )*( *px );
    }
    px++;
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::productVector(Alat::Node& y, const Alat::Node& x, double s) const
{
  // matrix-vector-multiplication (using VECTOR template)
  const_iterator p  = DoubleMatrix::begin();
  Alat::Node::iterator py = y.begin();
  Alat::Node::const_iterator px;

  while( p != DoubleMatrix::end() )
  {
    px = x.begin();
    for(int j = 0; j < n_cols; j++)
    {
      *py += s*( *p++ )*( *px++ );
    }
    py++;
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::productVectorTransposed(Alat::Node& y, const Alat::Node& x, double s) const
{
  const_iterator p  = DoubleMatrix::begin();
  Alat::Node::iterator py = y.begin();
  Alat::Node::const_iterator px = x.begin();
  
  while( p != DoubleMatrix::end() )
  {
    py = y.begin();
    for(int j = 0; j < n_cols; j++)
    {
      ( *py++ ) += s*( *p++ )*( *px );
    }
    px++;
  }
}

/*---------------------------------------------------------*/
void DoubleMatrix::identity()
{
  eye();
  // zeros();
  // for(int i = 0; i < n(); i++)
  // {
  //   ( *this )( i, i ) = 1.;
  // }
}

/*---------------------------------------------------------*/
void DoubleMatrix::productMatrix(DoubleMatrix& A, const DoubleMatrix& B, double d) const
{
  /* A = (*this) * B */
  // A.zeros();
  for(int i = 0; i < A.n_rows; i++)
  {
    for(int j = 0; j < A.n_cols; j++)
    {
      for(int k = 0; k < n_cols; k++)
      {
        A(i, j) += d*( *this )( i, k )*B(k, j);
      }
    }
  }
}

/*---------------------------------------------------------*/
void DoubleMatrix::productTransposedMatrix(DoubleMatrix& A, const DoubleMatrix& B, double d) const
{
  /* A = (*this) * B^T */
  A.zeros();
  for(int i = 0; i < A.n_rows; i++)
  {
    for(int j = 0; j < A.n_cols; j++)
    {
      for(int k = 0; k < n_cols; k++)
      {
        A(i, j) += d*( *this )( i, k )*B(j, k);
      }
    }
  }
}

/*---------------------------------------------------------*/
void DoubleMatrix::productMatrixTransposed(DoubleMatrix& A, const DoubleMatrix& B, double d) const
{
  /* A = (*this)^T * B */
  A.zeros();
  for(int i = 0; i < A.n_rows; i++)
  {
    for(int j = 0; j < A.n_cols; j++)
    {
      for(int k = 0; k < n_cols; k++)
      {
        A(i, j) += d*( *this )( k, i )*B(k, j);
      }
    }
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::add(const DoubleMatrix& B)
{
  for(int i = 0; i < ( *this ).n_rows; i++)
  {
    for(int j = 0; j < ( *this ).n_cols; j++)
    {
      ( *this )( i, j ) += B(i, j);
    }
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::add(double s, const DoubleMatrix& B)
{
  for(int i = 0; i < ( *this ).n_rows; i++)
  {
    for(int j = 0; j < ( *this ).n_cols; j++)
    {
      ( *this )( i, j ) += s*B(i, j);
    }
  }
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::add(const DoubleVector& u, const DoubleVector& v, double s)
{
  //QN
  // (*this)=(*this)+s*u*v^T
  for(int i = 0; i < ( *this ).n_rows; i++)
  {
    for(int j = 0; j < ( *this ).n_cols; j++)
    {
      ( *this )( i, j ) += s*u[i]*v[j];
    }
  }
}
//
// /*----------------------------------------------------------*/
// void DoubleMatrix::zeros()
// {
//   iterator first  = begin();
//   const_iterator last   = end();
//   while( first != last)
//   {
//     *first++ = 0.0;
//   }
// }

/*----------------------------------------------------------*/
double DoubleMatrix::norm() const
{
  return arma::norm(static_cast<const arma::mat&>(*this));
  // return arma::norm(*this);
  
  // const_iterator first  = DoubleMatrix::begin();
  // const_iterator last   = DoubleMatrix::end();
  //
  // double d = 0.0;
  // while(first != last)
  // {
  //   d += ( *first )*( *first++ );
  // }
  // return sqrt(d);
}


/*----------------------------------------------------------*/
void DoubleMatrix::gaussJordan()
{
  *this = this->i();
  // arma::inv(*this);
}

/*-----------------------------------------------------------------*/
void DoubleMatrix::addVectorToRow(int i, const DoubleVector& v, double s)
{
  //(*this)(i,:)=(*this)(i,:)+s*v(:)
  for(int j = 0; j < n_cols; j++)
  {
    ( *this )( i, j ) += s*v[j];
  }
}
/*----------------------------------------------------------*/
std::ostream& DoubleMatrix::writeBin(std::ostream& out) const
{
  out.write( reinterpret_cast<const char*>( &( *this )[0] ), this->size()*sizeof( double ) );
  return out;
}
std::istream& DoubleMatrix::readBin(std::istream& in)
{
  in.read( reinterpret_cast<char*>( &( *this )[0] ), this->size()*sizeof( double ) );
  return in;
}
std::ostream& DoubleMatrix::saveFada(std::ostream& out, const std::string datatype) const
{
  assert(0);
  // std::cerr <<  Vector<T>::size() << " datatype "  << datatype << " " << std::endl;
  out<< n_rows << " " << n_cols <<" "<<datatype<<" "<<std::endl;
  if(datatype == "ascii")
  {
    // save(out, arma::arma_ascii);
    // out<<*this;
    copy( begin(), end(), std::ostream_iterator<double>(out, " ") );
  }
  else
  {
    // save(out, arma::arma_binary);
    writeBin(out);
  }
  return out;
}
std::istream& DoubleMatrix::loadFada(std::istream& in)
{
  assert(0);
  // load(in);
  // return in;
  
  int nr, nc;
  std::string datatype;
  in>>nr >> nc >>datatype;
  set_size(nr, nc);
  if(datatype == "ascii")
  {
    // in>>*this;
    DoubleMatrix::iterator p = begin();
    while( p != end() )
    {
      in>>*p++;
    }
  }
  else if(datatype == "binary")
  {
    std::string str;
    while(str == "")
    {
      getline(in, str);
    }
    readBin(in);
  }
  else
  {
    std::cerr<<"*** Vector<T>::read() : wrong datatype \""<<datatype<<"\"\n";
    assert(0);
  }
  return in;
}
// std::ostream& Alat::operator<<(std::ostream& s, const DoubleMatrix& A)
// {
//   // s << "size= " << A.size() << "\n";
//   copy( A.begin(), A.end(), std::ostream_iterator<double>(s, " ") );
//   return s;
// }
// std::istream& Alat::operator>>(std::istream& s, DoubleMatrix& A)
// {
//   DoubleMatrix::iterator p = A.begin();
//   while( p != A.end() )
//   {
//     s>>*p++;
//   }
//   return s;
// }
