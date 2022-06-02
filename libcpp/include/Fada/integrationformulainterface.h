#ifndef __Fada_IntegrationFormulaInterface_h
#define __Fada_IntegrationFormulaInterface_h

#include  "Alat/interfacebase.h"
// #include  "Alat/node.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Node;
}
namespace Fada
{
  class IntegrationFormulaInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~IntegrationFormulaInterface();
    IntegrationFormulaInterface();
    IntegrationFormulaInterface( const IntegrationFormulaInterface& integrationformulainterface);
    IntegrationFormulaInterface& operator=( const IntegrationFormulaInterface& integrationformulainterface);
    std::string getName() const;

    virtual int n() const = 0;
    virtual double weight(int k) const = 0;
    virtual void point(Alat::Node& v, int k) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
