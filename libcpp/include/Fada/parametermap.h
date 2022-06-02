#ifndef __Fada_ParameterMap_h
#define __Fada_ParameterMap_h

#include  "Alat/map.h"
#include  "Alat/parameterfile.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  template<class T>
  class ParameterMap : public Alat::Map<std::string, T>
  {
private:
    std::string _name;

public:
    ~ParameterMap();
    ParameterMap();
    ParameterMap(const ParameterMap& P);
    ParameterMap(const std::string& name);
    ParameterMap& operator=(const ParameterMap& name);
    void basicInit(const Alat::ParameterFile* parameterfile);
    void addModelNameAndId(const std::string& modelname, int id);
  };
}

/*--------------------------------------------------------------------------*/

#endif
