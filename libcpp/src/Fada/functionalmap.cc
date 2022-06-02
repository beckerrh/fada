#include  "Fada/functionalmap.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

FunctionalMap::~FunctionalMap()
{}

/*--------------------------------------------------------------------------*/

FunctionalMap::FunctionalMap() : Alat::Map<std::string, FunctionalInterface*>()
{}

/*--------------------------------------------------------------------------*/

FunctionalMap::FunctionalMap( const FunctionalMap& functionalmap) : Alat::Map<std::string, FunctionalInterface*>(functionalmap)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

FunctionalMap& FunctionalMap::operator=( const FunctionalMap& functionalmap)
{
  Alat::Map<std::string, FunctionalInterface*>::operator=(functionalmap);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string FunctionalMap::getName() const
{
  return "FunctionalMap";
}
