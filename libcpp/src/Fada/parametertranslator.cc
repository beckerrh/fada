#include  "Fada/parametertranslator.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

ParameterTranslator::~ParameterTranslator()
{}

/*--------------------------------------------------------------------------*/

ParameterTranslator::ParameterTranslator()
{}

/*--------------------------------------------------------------------------*/

ParameterTranslator::ParameterTranslator( const ParameterTranslator& parametertranslator)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

ParameterTranslator& ParameterTranslator::operator=( const ParameterTranslator& parametertranslator)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string ParameterTranslator::getName() const
{
  return "ParameterTranslator";
}

/*--------------------------------------------------------------------------*/

void ParameterTranslator::addEntry(std::string integratorname, std::string model_param_name, std::string translation )
{
  if( !entryExists(integratorname, model_param_name) )
  {
    // _dico[integratorname].insert( std::make_pair<std::string, std::string>(model_param_name, translation) );
    _dico[integratorname].insert( std::make_pair(model_param_name, translation) );
    integrators_of_param[model_param_name].insert(integratorname);
  }
  else
  {
    std::cerr<<"**** Error ParameterTranslator::addEntry: an entry for integrator "<<integratorname<<" and parameter "<<model_param_name<<"already exists"<<'\n';
    assert(0);
    exit(1);
  }
}

/*--------------------------------------------------------------------------*/

const Alat::StringSet& ParameterTranslator::getIntegratorsOfParameter(std::string model_param_name) const
{
  return integrators_of_param[model_param_name];
}

/*--------------------------------------------------------------------------*/

std::string ParameterTranslator::translate(std::string integratorname, std::string model_param_name) const
{
  if( entryExists(integratorname, model_param_name) )
  {
    return _dico[integratorname][model_param_name];
  }
  else
  {
    std::cerr<<"**** Error ParameterTranslator::translate: for integrator "<<integratorname<<"  parameter "<<model_param_name<<"is not defined in dictionnary"<<'\n';
    assert(0);
    exit(1);
    return model_param_name;
  }
  return model_param_name;
}

/*--------------------------------------------------------------------------*/

std::string ParameterTranslator::translateIfDefined(std::string integratorname, std::string model_param_name) const
{
  if( _dico.find(integratorname) != _dico.end() )
  {
    if( _dico[integratorname].find(model_param_name) != _dico[integratorname].end() )
    {
      return _dico[integratorname][model_param_name];
    }
    else
    {
      std::cerr<<"**** Warning ParameterTranslator::translateIfDefined: for integrator"
      <<integratorname<<"  parameter "<<model_param_name<<"is not defined in dictionnary"<<'\n';
      return model_param_name;
    }
  }
  else
  {
    std::cerr<<"**** Warning ParameterTranslator::translateIfDefined: integrator "<<integratorname<<" is not defined in dictionnary"<<'\n';
    assert(0);
    exit(1);
    return model_param_name;
  }
  return model_param_name;
}

/*--------------------------------------------------------------------------*/

bool ParameterTranslator::entryExists(std::string integratorname, std::string model_param_name) const
{
  if( _dico.find(integratorname) == _dico.end() )
  {
    return false;
  }
  if( _dico[integratorname].find(model_param_name) == _dico[integratorname].end() )
  {
    return false;
  }
  return true;
}
