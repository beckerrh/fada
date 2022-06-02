#ifndef __Fada_ParameterTranslator_h
#define __Fada_ParameterTranslator_h

#include  "Alat/map.h"
#include  "Alat/stringset.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ParameterTranslator
  {
private:
    Alat::Map<std::string, Alat::Map<std::string, std::string>  > _dico;
    Alat::Map<std::string, Alat::StringSet > integrators_of_param;

protected:
public:
    ~ParameterTranslator();
    ParameterTranslator();
    ParameterTranslator( const ParameterTranslator& parametertranslator);
    ParameterTranslator& operator=( const ParameterTranslator& parametertranslator);
    std::string getName() const;
    void addEntry(std::string integratorname, std::string model_param_name, std::string translation );
    const Alat::StringSet& getIntegratorsOfParameter(std::string model_param_name) const;
    std::string translate(std::string integratorname, std::string model_param_name) const;
    std::string translateIfDefined(std::string integratorname, std::string model_param_name) const;
    bool entryExists(std::string integratorname, std::string model_param_name) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
