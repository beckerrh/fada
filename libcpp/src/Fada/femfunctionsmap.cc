#include  "Fada/femfunctionsmap.h"
#include  "Alat/systemvectorinterface.h"
#include  "Alat/variablevectorinterface.h"
#include  "Alat/stringset.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
FemFunctionsMap::~FemFunctionsMap(){}
FemFunctionsMap::FemFunctionsMap() :  Alat::Map<std::string, FemFunctions>(){}
  
FemFunctionsMap::FemFunctionsMap(const Fada::LocalGlobal::VectorMap& vectormap) : Alat::Map<std::string, FemFunctions>()
{
  for(Fada::LocalGlobal::VectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
  {
    Alat::StringIntMap ncomps;
    Alat::StringSet varnames = p->second->getVariables();
    for(Alat::StringSet::const_iterator q=varnames.begin(); q!=varnames.end();q++)
    {
      ncomps[*q] = p->second->getVector(*q)->getNComponents();
    }
    (*this)[p->first].basicInit(ncomps);
  }    
}

FemFunctionsMap::FemFunctionsMap( const Fada::FemFunctionsMap& femfunctionsmap)
{
  ( *this ).operator=(femfunctionsmap);
}

FemFunctionsMap& FemFunctionsMap::operator=( const Fada::FemFunctionsMap& femfunctionsmap)
{
  ( *this ).operator=(femfunctionsmap);
}

std::string FemFunctionsMap::getName() const
{
  return "FemFunctionsMap";
}
