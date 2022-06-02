#ifndef __Fada_IntegrationFormulaSelectorQuadrilateralMesh_h
#define __Fada_IntegrationFormulaSelectorQuadrilateralMesh_h

#include  "Fada/integrationformulaselectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class IntegrationFormulaSelectorQuadrilateralMesh : public Fada::IntegrationFormulaSelectorInterface
  {
private:
protected:
public:
    ~IntegrationFormulaSelectorQuadrilateralMesh();
    IntegrationFormulaSelectorQuadrilateralMesh();
    IntegrationFormulaSelectorQuadrilateralMesh( const IntegrationFormulaSelectorQuadrilateralMesh& integrationformulaselectorquadrilateralmesh);
    IntegrationFormulaSelectorQuadrilateralMesh& operator=( const IntegrationFormulaSelectorQuadrilateralMesh& integrationformulaselectorquadrilateralmesh);
    std::string getName() const;
    IntegrationFormulaSelectorQuadrilateralMesh* clone() const;

    Fada::IntegrationFormulaInterface* newIntegrationFormula(int order, FadaEnums::domain domain) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
