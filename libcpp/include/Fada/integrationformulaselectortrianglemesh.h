#ifndef __Fada_IntegrationFormulaSelectorTriangleMesh_h
#define __Fada_IntegrationFormulaSelectorTriangleMesh_h

#include  "Fada/integrationformulaselectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaSelectorTriangleMesh : public Fada::IntegrationFormulaSelectorInterface
  {
private:
protected:
public:
    ~IntegrationFormulaSelectorTriangleMesh();
    IntegrationFormulaSelectorTriangleMesh();
    IntegrationFormulaSelectorTriangleMesh( const IntegrationFormulaSelectorTriangleMesh& integrationformulaselectortrianglemesh);
    IntegrationFormulaSelectorTriangleMesh& operator=( const IntegrationFormulaSelectorTriangleMesh& integrationformulaselectortrianglemesh);
    std::string getName() const;
    IntegrationFormulaSelectorTriangleMesh* clone() const;

    Fada::IntegrationFormulaInterface* newIntegrationFormula(int order, FadaEnums::domain domain) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
