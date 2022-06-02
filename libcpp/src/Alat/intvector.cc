#include  "Alat/intvector.h"
#include  "Alat/alatio.h"
// #include  "Alat/tokenize.h"
#include  <stdlib.h>
#include  <cassert>
#include  <iterator>

using namespace Alat;

/*--------------------------------------------------------------------------*/

IntVector::~IntVector() {}
IntVector::IntVector() : arma::Col<int>() {}
IntVector::IntVector(const IntVector& v) : arma::Col<int>(v)  {}
IntVector::IntVector(size_type n) : arma::Col<int>(n) {}
// IntVector::IntVector(std::string valuechain) : arma::Col<int>()
// {
//   Alat::reInitFromString(valuechain, *this);
//   // reInitFromString(valuechain);
// }

IntVector& IntVector::operator=(const IntVector& v)
{
  assert(size()==v.size());
  arma::Col<int>::operator=(v);
  return *this;
}
IntVector& IntVector::operator=(const std::vector<int>& v)
{
  assert(size()==v.size());
  arma::Col<int>::operator=(v);
 return *this;
}
bool IntVector::operator==(const IntVector& v) const
{
  assert(size()==v.size());
  for(int i=0;i<size();i++)
  {
    if ( (*this)[i] != v[i])
    {
      return false;
    }
  }
  return true;
}

// void IntVector::set_size(size_t n)
// {
//   this->set_size(n);
// }

// /*--------------------------------------------------------------------------*/
// void IntVector::reInitFromString(std::string valuechain)
// {
//   Alat::StringVector bouts = Alat::Tokenize(valuechain, "|");
//   int n = bouts.size();
//   this->set_size(n);
//   for(int i = 0; i < n; i++)
//   {
//     ( *this )[i] = atoi( bouts[i].c_str() );
//   }
// }

/*----------------------------------------------------------*/
std::ostream& IntVector::writeBin(std::ostream& out) const
{
  out.write( reinterpret_cast<const char*>( &( *this )[0] ), this->size()*sizeof( int ) );
  return out;
}
std::istream& IntVector::readBin(std::istream& in)
{
  in.read( reinterpret_cast<char*>( &( *this )[0] ), this->size()*sizeof( int ) );
  return in;
}
std::ostream& IntVector::saveFada(std::ostream& out, const std::string datatype) const
{
  // std::cerr <<  Vector<T>::size() << " datatype "  << datatype << " " << std::endl;
  out<<IntVector::size()<<" "<<datatype<<" "<<std::endl;
  if(datatype == "ascii")
  {
    // save(out, arma::arma_ascii);
    // out<<*this;
  copy( begin(), end(), std::ostream_iterator<int>(out, " ") );
  }
  else
  {
    // save(out, arma::arma_binary);
    writeBin(out);
  }
  return out;
}
std::istream& IntVector::loadFada(std::istream& in)
{
  // load(in);
  // return in;
  
  int n;
  std::string datatype;
  in>>n>>datatype;
  set_size(n);
  if(datatype == "ascii")
  {
  IntVector::iterator p = begin();
  while( p != end() )
  {
    in>>*p++;
  }
    // in>>*this;
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
// std::ostream& Alat::operator<<(std::ostream& s, const IntVector& A)
// {
//   // s << "size= " << A.size() << "\n";
//   copy( A.begin(), A.end(), std::ostream_iterator<int>(s, " ") );
//   return s;
// }
// std::istream& Alat::operator>>(std::istream& s, IntVector& A)
// {
//   IntVector::iterator p = A.begin();
//   while( p != A.end() )
//   {
//     s>>*p++;
//   }
//   return s;
// }
//
