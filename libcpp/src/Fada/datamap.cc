#include  "Fada/datamap.h"
#include  <cassert>
#include  <stdlib.h>

using namespace Fada;

/*---------------------------------------------------------*/

DataMap::~DataMap() {}

/*---------------------------------------------------------*/

DataMap::DataMap() : Alat::Map<std::string, const Fada::DataInterface*>() {}

/*---------------------------------------------------------*/

DataMap::DataMap(const DataMap& D) : Alat::Map<std::string, const Fada::DataInterface*>()
{
}

/*---------------------------------------------------------*/

DataMap& DataMap::operator=(const DataMap& D)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

std::string DataMap::getName() const
{
  return "Fada::DataMap";
}

/*---------------------------------------------------------*/

void DataMap::setData(const std::string& name, const Fada::DataInterface* data)
{
  if( ( *this ).hasKey(name) )
  {
    std::cerr<<"*** ERROR Fada::DataMap: parameter "<<name<<" already defined\n";
    assert(0);
    exit(1);
  }
  ( *this )[name] = data;
}

/*---------------------------------------------------------*/

const Fada::DataInterface* DataMap::getData(const std::string& name) const
{
  if( !( ( *this ).hasKey(name) ) )
  {
    std::cerr<<"*** ERROR Fada::DataMap: parameter "<<name<<" not defined\n";
    std::cerr << "I have: " << *this << "\n";
    assert(0);
    exit(1);
  }
  return ( *this )[name];
}
