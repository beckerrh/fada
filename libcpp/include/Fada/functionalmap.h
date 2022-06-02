#ifndef __Fada_FunctionalMap_h
#define __Fada_FunctionalMap_h

#include  "functionalinterface.h"
#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FunctionalMap : public Alat::Map<std::string, FunctionalInterface*>
  {
private:
protected:
public:
    ~FunctionalMap();
    FunctionalMap();
    FunctionalMap( const FunctionalMap& functionalmap);
    FunctionalMap& operator=( const FunctionalMap& functionalmap);
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
