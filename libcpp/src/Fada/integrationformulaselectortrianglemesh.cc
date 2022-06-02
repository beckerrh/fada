#include  "Fada/integrationformulatriangle.h"
#include  "Fada/integrationformulaselectortrianglemesh.h"
#include  "Fada/integrationformulaline.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegrationFormulaSelectorTriangleMesh::~IntegrationFormulaSelectorTriangleMesh(){}
IntegrationFormulaSelectorTriangleMesh::IntegrationFormulaSelectorTriangleMesh() : IntegrationFormulaSelectorInterface(){}
IntegrationFormulaSelectorTriangleMesh::IntegrationFormulaSelectorTriangleMesh( const IntegrationFormulaSelectorTriangleMesh& integrationformulaselectortrianglemesh) : IntegrationFormulaSelectorInterface(integrationformulaselectortrianglemesh)
{
  assert(0);
}
IntegrationFormulaSelectorTriangleMesh& IntegrationFormulaSelectorTriangleMesh::operator=( const IntegrationFormulaSelectorTriangleMesh& integrationformulaselectortrianglemesh)
{
  IntegrationFormulaSelectorInterface::operator=(integrationformulaselectortrianglemesh);
  assert(0);
  return *this;
}
std::string IntegrationFormulaSelectorTriangleMesh::getName() const
{
  return "IntegrationFormulaSelectorTriangleMesh";
}
IntegrationFormulaSelectorTriangleMesh* IntegrationFormulaSelectorTriangleMesh::clone() const
{
  return new IntegrationFormulaSelectorTriangleMesh(*this);
}

/*--------------------------------------------------------------------------*/

Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorTriangleMesh::newIntegrationFormula(const std::string& name, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
  {
    if(name == "MidPoint")
    {
      return new Fada::TriangleMidPoint;
    }
    else if(name == "Trapez")
    {
      return new Fada::TriangleTrapez;
    }
    else if(name == "Degree4")
    {
      return new Fada::TriangleDegree4;
    }
    else if(name == "Degree6")
    {
      return new Fada::TriangleDegree6;
    }
    else if(name == "Degree8")
    {
      return new Fada::TriangleDegree8;
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

Fada::IntegrationFormulaInterface* IntegrationFormulaSelectorTriangleMesh::newIntegrationFormula(int order, FadaEnums::domain domain) const
{
  if( domain == FadaEnums::Cells )
  {
    if(order == 0)
    {
      return new Fada::TriangleMidPoint;
    }
    else if(order==1)
    {
      return new Fada::TriangleDegree4;
      return new Fada::TriangleRotatedTrapez;
    }
    else if(order==2)
    {
      return new Fada::TriangleDegree4;
      return new Fada::TriangleRotatedTrapez;
    }
    else if(order==3)
    {
      return new Fada::TriangleDegree4;
    }
    else if(order==4)
    {
      return new Fada::TriangleDegree6;
    }
    else if(order==5)
    {
      return new Fada::TriangleDegree6;
    }
    else if(order==6)
    {
      return new Fada::TriangleDegree8;
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
