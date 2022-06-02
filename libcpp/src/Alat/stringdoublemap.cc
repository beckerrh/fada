#include  "Alat/stringdoublemap.h"
#include  "Alat/stringset.h"
#include  "Alat/tokenize.h"
#include  <iterator>

using namespace Alat;

/*--------------------------------------------------------------------------*/
StringDoubleMap::~StringDoubleMap() {}
StringDoubleMap::StringDoubleMap() : Alat::Map<std::string, double>() {}
StringDoubleMap::StringDoubleMap(const StringDoubleMap& v) : Alat::Map<std::string, double>(v)  {}
StringDoubleMap::StringDoubleMap(const StringSet& v) : Alat::Map<std::string, double>()
{
  for(StringSet::const_iterator p = v.begin(); p != v.end(); p++)
  {
    ( *this )[*p] = 0.0;
  }
}

StringDoubleMap& StringDoubleMap::operator=(const StringDoubleMap& v)
{
  Alat::Map<std::string, double>::operator=(v);
  return *this;
}

double StringDoubleMap::norm2() const
{
  double norm = 0.0;
  for(StringDoubleMap::const_iterator p = begin(); p != end(); p++)
  {
    norm += p->second;
  }
  return norm;
}

void StringDoubleMap::zeros()
{
  for(StringDoubleMap::iterator p = begin(); p != end(); p++)
  {
    p->second = 0.0;
  }
}
