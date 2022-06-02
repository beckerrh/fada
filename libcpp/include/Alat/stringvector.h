#ifndef __Alat_StringVector_h
#define __Alat_StringVector_h

#include  "vector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class StringSet;
  
  class StringVector : public Alat::Vector<std::string>
  {
public:
    ~StringVector();
    StringVector();
    StringVector(const StringVector& v);
    StringVector(const StringSet& v);
    StringVector(int n);
    StringVector(std::string valuechain, const std::string& sep="|");
    StringVector(std::istream_iterator<std::string> begin, std::istream_iterator<std::string> end);
    StringVector& operator=(std::string n);
    void reInitFromString(std::string valuechain, const std::string& sep="|");
    StringVector& operator=(const StringVector& v);
    bool contains(const StringSet& v) const;
  };
}

/*---------------------------------------------------------*/

#endif
