#ifndef __Fada_VariablesMap_h
#define __Fada_VariablesMap_h

#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class VariableInterface;

  class VariablesMap : public Alat::Map<std::string, VariableInterface*>
  {
public:
    ~VariablesMap();
    VariablesMap();
    VariablesMap( const VariablesMap& variablesmap);
    VariablesMap& operator=( const VariablesMap& variablesmap);
    std::string getName() const;
    VariablesMap* clone() const;
  };
  std::ostream& operator<<(std::ostream& os, const VariablesMap& variablesmap);
}

/*--------------------------------------------------------------------------*/

#endif
