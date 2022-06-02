#ifndef __Fada_IntegrationFormulaSelectorInterface_h
#define __Fada_IntegrationFormulaSelectorInterface_h

#include  "Alat/interfacebase.h"
#include  "fadalightenums.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaInterface;
  
class IntegrationFormulaSelectorInterface : public Alat::InterfaceBase
{
protected:
  std::string getInterfaceName() const;

public:
  ~IntegrationFormulaSelectorInterface();
  IntegrationFormulaSelectorInterface();
  IntegrationFormulaSelectorInterface( const IntegrationFormulaSelectorInterface& integrationformulaselectorinterface);
  IntegrationFormulaSelectorInterface& operator=( const IntegrationFormulaSelectorInterface& integrationformulaselectorinterface);
  std::string getName() const;
  IntegrationFormulaSelectorInterface* clone() const;

  virtual IntegrationFormulaInterface* newIntegrationFormula(int order, FadaEnums::domain domain) const=0;
  virtual IntegrationFormulaInterface* newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const=0;
};
}

/*--------------------------------------------------------------------------*/

#endif
