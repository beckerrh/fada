#ifndef __Alat_StringDoubleVectorMap_h
#define __Alat_StringDoubleVectorMap_h

#include  "map.h"
#include  "doublevector.h"

/*---------------------------------------------------------*/

namespace Alat
{
  class StringSet;
  
  class StringDoubleVectorMap : public Alat::Map<std::string, Alat::DoubleVector>
  {
public:
    ~StringDoubleVectorMap();
    StringDoubleVectorMap();
    StringDoubleVectorMap(const StringDoubleVectorMap& v);
    StringDoubleVectorMap(const StringSet& v, const StringIntMap& ncomps);
    StringDoubleVectorMap& operator=(const StringDoubleVectorMap& v);
    double norm2() const;
    void zeros();
  };
}

/*---------------------------------------------------------*/

#endif
