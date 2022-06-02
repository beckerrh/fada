#ifndef __Alat_StringDoubleMap_h
#define __Alat_StringDoubleMap_h

#include  "map.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class StringSet;
  
  class StringDoubleMap : public Alat::Map<std::string, double>
  {
public:
    ~StringDoubleMap();
    StringDoubleMap();
    StringDoubleMap(const StringDoubleMap& v);
    StringDoubleMap(const StringSet& v);
    StringDoubleMap& operator=(const StringDoubleMap& v);
    double norm2() const;
    void zeros();
  };
}

/*---------------------------------------------------------*/

#endif
