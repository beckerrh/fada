#include  "Alat/stringset.h"
#include  "Alat/stringvector.h"
#include  "Alat/tokenize.h"
#include  <iterator>

using namespace Alat;

/*--------------------------------------------------------------------------*/
StringVector::~StringVector() {}
StringVector::StringVector() : Alat::Vector<std::string>() {}
StringVector::StringVector(const StringVector& v) : Alat::Vector<std::string>(v)  {}
StringVector::StringVector(const StringSet& v) : Alat::Vector<std::string>()
{
  set_size( v.size() );
  std::copy( v.begin(), v.end(), begin() );
}

StringVector::StringVector(int n) : Alat::Vector<std::string>(n, "") {}
StringVector::StringVector(std::string valuechain, const std::string& sep) : Alat::Vector<std::string>()
{
  reInitFromString(valuechain, sep);
}

StringVector::StringVector(std::istream_iterator<std::string> begin, std::istream_iterator<std::string> end) : Alat::Vector<std::string>(begin, end) {}

StringVector& StringVector::operator=(std::string n)
{
  Alat::StringVector::equal(n);
  return *this;
}

StringVector& StringVector::operator=(const StringVector& v)
{
  Alat::StringVector::equal(v);
  return *this;
}

/*--------------------------------------------------------------------------*/
void StringVector::reInitFromString(std::string valuechain, const std::string& sep)
{
  assert(0);
  Alat::StringVector bouts = Alat::Tokenize(valuechain, sep);
  int n = bouts.size();
  Vector<std::string>::set_size(n);
  for(int i = 0; i < n; i++)
  {
    ( *this )[i] =  bouts[i].c_str();
  }
}

/*--------------------------------------------------------------------------*/
bool StringVector::contains(const StringSet& v) const
{
  if( size() != v.size() )
  {
    return false;
  }
  for(const_iterator p = begin(); p != end(); p++)
  {
    if( v.find(*p) == v.end() )
    {
      return false;
    }
  }
  return true;
}
