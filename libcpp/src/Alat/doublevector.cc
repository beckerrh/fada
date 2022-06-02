#include  "Alat/doublevector.h"
#include  "Alat/node.h"
#include  "Alat/tokenize.h"
#include  <cmath>
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
DoubleVector::~DoubleVector() {}
DoubleVector::DoubleVector() : arma::Col<double>() {}
DoubleVector::DoubleVector(const arma::Col<double>& v) : arma::Col<double>(v)  {}
DoubleVector::DoubleVector(const DoubleVector& v) : arma::Col<double>(v)  {}
DoubleVector::DoubleVector(size_type n, double d) : arma::Col<double>(n)
{
  arma::Col<double>::fill(d);
}

/*--------------------------------------------------------------------------*/
DoubleVector& DoubleVector::operator=(const arma::Col<double>& v)
{
  assert(size()==v.size());
  arma::Col<double>::operator=(v);
  return *this;
}
DoubleVector& DoubleVector::operator=(const DoubleVector& v)
{
  assert(size()==v.size());
  arma::Col<double>::operator=(v);
  return *this;
}

/*----------------------------------------------------------*/
double DoubleVector::operator*(const DoubleVector& v) const
{
  return arma::dot(static_cast<const arma::Col<double>&>(*this), static_cast<const arma::Col<double>&>(v));
}

double DoubleVector::operator*(const Alat::Node& v) const
{
  // return arma::dot(static_cast<const arma::Col<double>&>(*this), static_cast<const arma::vec3&>(v));
  // return arma::dot(*this,v);
  const_iterator first  = DoubleVector::begin();
  const_iterator last   = DoubleVector::end();
  Alat::Node::const_iterator first2 = v.begin();
  double d = 0.0;
  while(first != last)
  {
    d += ( *first++ )*( *first2++ );
  }
  return d;
}

/*----------------------------------------------------------*/
double DoubleVector::scalarProduct(const DoubleVector& v) const
{
  // return arma::dot(*this,v);
  return this->operator*(v);
}

/*----------------------------------------------------------*/
void DoubleVector::equal(double d)
{
  fill(d);
}

void DoubleVector::equal(const DoubleVector& v)
{
  static_cast<arma::Col<double>&>(*this).operator=(static_cast<const arma::Col<double>&>(v));
}

void DoubleVector::equal(double d, const DoubleVector& v)
{
  arma::Col<double>& tarma =static_cast<arma::Col<double>&>(*this);
  const arma::Col<double>& varma =static_cast<const arma::Col<double>&>(v);
  tarma = d*varma;
  return;
}

/*----------------------------------------------------------*/
void DoubleVector::equal(double d1, const DoubleVector& v1, double d2, const DoubleVector& v2)
{
  arma::Col<double>& tarma =static_cast<arma::Col<double>&>(*this);
  const arma::Col<double>& varma1 =static_cast<const arma::Col<double>&>(v1);
  const arma::Col<double>& varma2 =static_cast<const arma::Col<double>&>(v2);
  tarma = d1*varma1+d2*varma2;
  return;
}

/*----------------------------------------------------------*/
void DoubleVector::add(double d, const DoubleVector& v)
{
  arma::Col<double>& tarma =static_cast<arma::Col<double>&>(*this);
  const arma::Col<double>& varma =static_cast<const arma::Col<double>&>(v);
  tarma += d*varma;
}

/*----------------------------------------------------------*/
double DoubleVector::norm() const
{
  return arma::norm(static_cast<const arma::Col<double>&>(*this));
}

/*----------------------------------------------------------*/
void DoubleVector::scale(const double& d)
{
  *this *= d;
}
/*----------------------------------------------------------*/
std::ostream& DoubleVector::writeBin(std::ostream& out) const
{
  out.write( reinterpret_cast<const char*>( &( *this )[0] ), this->size()*sizeof( double ) );
  return out;
}
std::istream& DoubleVector::readBin(std::istream& in)
{
  in.read( reinterpret_cast<char*>( &( *this )[0] ), this->size()*sizeof( double ) );
  return in;
}
std::ostream& DoubleVector::saveFada(std::ostream& out, const std::string datatype) const
{
  if(datatype == "ascii")
  {
    save(out, arma::arma_ascii);
  }
  else
  {
    save(out, arma::arma_binary);
  }
  return out;


  // std::cerr <<  Vector<T>::size() << " datatype "  << datatype << " " << std::endl;
  out<<DoubleVector::size()<<" "<<datatype<<" "<<std::endl;
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
std::istream& DoubleVector::loadFada(std::istream& in)
{
  load(in);
  return in;

  int n;
  std::string datatype;
  in>>n>>datatype;
  set_size(n);
  if(datatype == "ascii")
  {
    // in>>*this;
  DoubleVector::iterator p = begin();
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
