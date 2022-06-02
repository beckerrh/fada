#include  "Fada/integrationdata.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

IntegrationData::~IntegrationData()
{}

/*--------------------------------------------------------------------------*/

IntegrationData::IntegrationData()
{}

/*--------------------------------------------------------------------------*/

IntegrationData::IntegrationData( const IntegrationData& IntegrationData)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

IntegrationData& IntegrationData::operator=( const IntegrationData& IntegrationData)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string IntegrationData::getName() const
{
  return "IntegrationData";
}
