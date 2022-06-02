#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/integrationformulaselectorquadrilateralmesh.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorQuadrilateralMesh::~IntegrationFormulaSelectorQuadrilateralMesh()
{}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorQuadrilateralMesh::IntegrationFormulaSelectorQuadrilateralMesh() : IntegrationFormulaSelectorInterface()
{}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorQuadrilateralMesh::IntegrationFormulaSelectorQuadrilateralMesh( const IntegrationFormulaSelectorQuadrilateralMesh& integrationformulaselectorquadrilateralmesh) : IntegrationFormulaSelectorInterface(integrationformulaselectorquadrilateralmesh)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorQuadrilateralMesh& IntegrationFormulaSelectorQuadrilateralMesh::operator=( const IntegrationFormulaSelectorQuadrilateralMesh& integrationformulaselectorquadrilateralmesh)
{
  IntegrationFormulaSelectorInterface::operator=(integrationformulaselectorquadrilateralmesh);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string IntegrationFormulaSelectorQuadrilateralMesh::getName() const
{
  return "IntegrationFormulaSelectorQuadrilateralMesh";
}

/*--------------------------------------------------------------------------*/

IntegrationFormulaSelectorQuadrilateralMesh* IntegrationFormulaSelectorQuadrilateralMesh::clone() const
{
  return new IntegrationFormulaSelectorQuadrilateralMesh(*this);
}

/*--------------------------------------------------------------------------*/

Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorQuadrilateralMesh::newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
  {
    if(name == "MidPoint")
    {
      return new Fada::QuadGauss1;
    }
    else if(name == "Trapez")
    {
      return new Fada::QuadTrapez;
    }
    else if(name == "Gauss1")
    {
      return new Fada::QuadGauss1;
    }
    else if(name == "Gauss4")
    {
      return new Fada::QuadGauss4;
    }
    else if(name == "Gauss9")
    {
      return new Fada::QuadGauss9;
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
      return new Fada::LineTrapez;
    }
    else if(name == "Gauss1")
    {
      return new Fada::LineGauss1;
    }
    else if(name == "Gauss2")
    {
      return new Fada::LineGauss2;
    }
    else if(name == "Gauss3")
    {
      return new Fada::LineGauss3;
    }
    else if(name == "Gauss4")
    {
      return new Fada::LineGauss4;
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

Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorQuadrilateralMesh::newIntegrationFormula(int order, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
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
    else if(order == 7)
    {
      return new Fada::QuadGauss25;
    }
    else if(order == 8)
    {
      return new Fada::QuadGauss25;
    }
    else if(order == 9)
    {
      return new Fada::QuadGauss36;
    }
    else if(order == 10)
    {
      return new Fada::QuadGauss36;
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
      return new Fada::LineGauss1;
    }
    else if(order == 1)
    {
      return new Fada::LineGauss2;
    }
    else if(order == 2)
    {
      return new Fada::LineGauss2;
    }
    else if(order == 3)
    {
      return new Fada::LineGauss3;
    }
    else if(order == 4)
    {
      return new Fada::LineGauss3;
    }
    else if(order == 5)
    {
      return new Fada::LineGauss4;
    }
    else if(order == 6)
    {
      return new Fada::LineGauss4;
    }
    else if(order == 7)
    {
      return new Fada::LineGauss5;
    }
    else if(order == 8)
    {
      return new Fada::LineGauss5;
    }
    else if(order == 9)
    {
      return new Fada::LineGauss6;
    }
    else if(order == 10)
    {
      return new Fada::LineGauss6;
    }
  }
  else
  {
    _error_string( "newIntegrationFormula", "unknown domaine: " + FadaEnums::domainToString(domain) );
  }
}
