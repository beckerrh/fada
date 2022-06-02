#include  "Alat/stringset.h"
#include  "Alat/tokenize.h"
#include  <iterator>

using namespace Alat;

/*--------------------------------------------------------------------------*/
StringSet::~StringSet() {}
StringSet::StringSet() : Alat::Set<std::string>() {}
StringSet::StringSet(const StringSet& v) : Alat::Set<std::string>(v)  {}
StringSet::StringSet(const StringVector& v)
{ 
  clear();
  for(int i=0;i<v.size();i++)
  {
    insert(v[i]);
  }
}
StringSet::StringSet(std::string valuechain, const std::string& sep) : Alat::Set<std::string>()
{
  reInitFromString(valuechain, sep);
}

StringSet& StringSet::operator=(const StringSet& v)
{
  Alat::Set<std::string>::operator=(v);
  return *this;
}

/*--------------------------------------------------------------------------*/
void StringSet::reInitFromString(std::string valuechain, const std::string& sep)
{
  assert(0);
  Alat::StringVector bouts = Alat::Tokenize(valuechain, sep);
  for(int i = 0; i < bouts.size(); i++)
  {
    insert(bouts[i].c_str());
  }
}
