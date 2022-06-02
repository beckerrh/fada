#include  "Fada/integrationformulahexahedral.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/integrationformulaselectorhexahedralmesh.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegrationFormulaSelectorHexahedralMesh::~IntegrationFormulaSelectorHexahedralMesh(){}
IntegrationFormulaSelectorHexahedralMesh::IntegrationFormulaSelectorHexahedralMesh() : IntegrationFormulaSelectorInterface(){}
IntegrationFormulaSelectorHexahedralMesh::IntegrationFormulaSelectorHexahedralMesh( const IntegrationFormulaSelectorHexahedralMesh& integrationformulaselectorquadrilateralmesh) : IntegrationFormulaSelectorInterface(integrationformulaselectorquadrilateralmesh)
{
  assert(0);
}
IntegrationFormulaSelectorHexahedralMesh& IntegrationFormulaSelectorHexahedralMesh::operator=( const IntegrationFormulaSelectorHexahedralMesh& integrationformulaselectorquadrilateralmesh)
{
  IntegrationFormulaSelectorInterface::operator=(integrationformulaselectorquadrilateralmesh);
  assert(0);
  return *this;
}
std::string IntegrationFormulaSelectorHexahedralMesh::getName() const
{
  return "IntegrationFormulaSelectorHexahedralMesh";
}
IntegrationFormulaSelectorHexahedralMesh* IntegrationFormulaSelectorHexahedralMesh::clone() const
{
  return new IntegrationFormulaSelectorHexahedralMesh(*this);
}

/*--------------------------------------------------------------------------*/

Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorHexahedralMesh::newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
  {
    if(name == "MidPoint")
    {
      return new Fada::HexGauss1;
    }
    else if(name == "Trapez")
    {
      return new Fada::HexTrapez;      
    }
    else
    {
      assert(0);
    }
  }
  else if( ( domain == FadaEnums::BoundarySides ) || ( domain == FadaEnums::InteriorSides )  || ( domain == FadaEnums::CouplingSides ) )
  {
    if(name == "Trapez")
    {
      return new Fada::QuadTrapez;      
    }
    else
    {
      assert(0);
    }
  }
  else
  {
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorHexahedralMesh::newIntegrationFormula(int order, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
  {
    if(order == 0)
    {
      return new Fada::HexGauss1;
    }
    else if(order == 1)
    {
      return new Fada::HexGauss8;
    }
    else if(order == 2)
    {
      return new Fada::HexGauss8;
    }
    else if(order == 3)
    {
      return new Fada::HexGauss27;
    }
    else if(order == 4)
    {
      return new Fada::HexGauss27;
    }
    else if(order == 5)
    {
      return new Fada::HexGauss64;
    }
    else if(order == 6)
    {
      return new Fada::HexGauss64;
    }
    else
    {
      _error_string("newIntegrationFormula", "unknwon order: ");
    }
  }
  else if( ( domain == FadaEnums::BoundarySides ) || ( domain == FadaEnums::InteriorSides )  || ( domain == FadaEnums::CouplingSides ) )
  {
    if(order == 0)
    {
      return new Fada::QuadGauss1;
    }
    else if(order == 1)
    {
      return new Fada::QuadGauss4;
    }
    else if(order == 2)
    {
      return new Fada::QuadGauss4;
    }
    else if(order == 3)
    {
      return new Fada::QuadGauss9;
    }
    else if(order == 4)
    {
      return new Fada::QuadGauss9;
    }
    else if(order == 5)
    {
      return new Fada::QuadGauss16;
    }
    else if(order == 6)
    {
      return new Fada::QuadGauss16;
    }
  }
  else
  {
    _error_string( "newIntegrationFormula", "unknown domaine: " + FadaEnums::domainToString(domain) );
  }
}
