#ifndef __Fada_IntegrationData_h
#define __Fada_IntegrationData_h

#include  <string>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationData
  {
private:

public:
    ~IntegrationData();
    IntegrationData();
    IntegrationData( const IntegrationData& IntegrationData);
    IntegrationData& operator=( const IntegrationData& IntegrationData);
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
