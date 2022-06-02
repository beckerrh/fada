#ifndef __Fada_IntegrationFormulaSelectorHexahedralMesh_h
#define __Fada_IntegrationFormulaSelectorHexahedralMesh_h

#include  "Fada/integrationformulaselectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaSelectorHexahedralMesh : public Fada::IntegrationFormulaSelectorInterface
  {
private:
protected:
public:
    ~IntegrationFormulaSelectorHexahedralMesh();
    IntegrationFormulaSelectorHexahedralMesh();
    IntegrationFormulaSelectorHexahedralMesh( const IntegrationFormulaSelectorHexahedralMesh& integrationformulaselectorquadrilateralmesh);
    IntegrationFormulaSelectorHexahedralMesh& operator=( const IntegrationFormulaSelectorHexahedralMesh& integrationformulaselectorquadrilateralmesh);
    std::string getName() const;
    IntegrationFormulaSelectorHexahedralMesh* clone() const;

    Fada::IntegrationFormulaInterface* newIntegrationFormula(int order, FadaEnums::domain domain) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
