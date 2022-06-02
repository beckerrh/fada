#ifndef __Alat_StringSet_h
#define __Alat_StringSet_h

#include  "set.h"

/*---------------------------------------------------------*/

namespace Alat
{          
  class StringVector;
  
  class StringSet : public Alat::Set<std::string>
  {
public:
    ~StringSet();
    StringSet();
    StringSet(const StringSet& v);
    StringSet(const StringVector& v);
    StringSet(std::string valuechain, const std::string& sep="|");
    void reInitFromString(std::string valuechain, const std::string& sep="|");
    StringSet& operator=(const StringSet& v);
  };
}

/*---------------------------------------------------------*/

#endif
