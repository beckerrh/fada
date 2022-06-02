#ifndef __Fada_FemFunctionsMap_h
#define __Fada_FemFunctionsMap_h

#include  "Alat/map.h"
#include  "femfunctions.h"
#include  "localglobal.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{                              
  class FemFunctionsMap : public Alat::Map<std::string, FemFunctions>
  {
public:
    ~FemFunctionsMap();
    FemFunctionsMap();
    FemFunctionsMap(const Fada::LocalGlobal::VectorMap& vectormap);
    FemFunctionsMap( const Fada::FemFunctionsMap& femfunctionsmap);
    FemFunctionsMap& operator=( const Fada::FemFunctionsMap& femfunctionsmap);
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
