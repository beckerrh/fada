#include  "Fada/variablesmap.h"
#include  "Fada/variableinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

VariablesMap::~VariablesMap()
{}

VariablesMap::VariablesMap() : Alat::Map<std::string, VariableInterface*>()
{}

VariablesMap::VariablesMap( const VariablesMap& variablesmap) : Alat::Map<std::string, VariableInterface*>(variablesmap)
{
  assert(0);
}

VariablesMap& VariablesMap::operator=( const VariablesMap& variablesmap)
{
  Alat::Map<std::string, VariableInterface*>::operator=(variablesmap);
  assert(0);
  return *this;
}

std::string VariablesMap::getName() const
{
  return "VariablesMap";
}

VariablesMap* VariablesMap::clone() const
{
  assert(0);
//return new VariablesMap(*this);
}

/*--------------------------------------------------------------------------*/

std::ostream& Fada::operator<<(std::ostream& os, const VariablesMap& variablesmap)
{
  for(Alat::Map<std::string, VariableInterface*>::const_iterator p = variablesmap.begin();p!=variablesmap.end();p++)
  {
    os << p->first << " ==> " << p->second->getVarName() << "\n";
  }
  return os;
}
