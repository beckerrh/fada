#include  "Alat/stringdoublevectormap.h"
#include  "Alat/stringset.h"
#include  "Alat/tokenize.h"
#include  <iterator>

using namespace Alat;

/*--------------------------------------------------------------------------*/
StringDoubleVectorMap::~StringDoubleVectorMap() {}
StringDoubleVectorMap::StringDoubleVectorMap() : Alat::Map<std::string, DoubleVector>() {}
StringDoubleVectorMap::StringDoubleVectorMap(const StringDoubleVectorMap& v) : Alat::Map<std::string, DoubleVector>(v)  {}
StringDoubleVectorMap::StringDoubleVectorMap(const StringSet& v, const StringIntMap& ncomps) : Alat::Map<std::string, DoubleVector>()
{
  assert(v.size()==ncomps.size());
  for(StringSet::const_iterator p = v.begin(); p != v.end(); p++)
  {
    ( *this )[*p] = DoubleVector(ncomps[*p]);
  }
}

StringDoubleVectorMap& StringDoubleVectorMap::operator=(const StringDoubleVectorMap& v)
{
  Alat::Map<std::string, DoubleVector>::operator=(v);
  return *this;
}

void StringDoubleVectorMap::zeros()
{
  for(StringDoubleVectorMap::iterator p = begin(); p != end(); p++)
  {
    p->second.zeros();
  }
}
