#include  "FadaMesh/curvedboundarydescriptionconstructor.h"
#include  "FadaMesh/curvedboundarydescriptionnaca.h"
#include  "FadaMesh/curvedboundarydescriptionnozzle.h"
#include  "FadaMesh/curvedboundarydescriptionpolynomial.h"
#include  "FadaMesh/curvedboundarydescriptionwingellipticprofile.h"
#include  "Alat/stringvector.h"
#include  <cassert>

using namespace FadaMesh;
using namespace std;

/*---------------------------------------------------*/
CurvedBoundaryDescriptionConstructor::~CurvedBoundaryDescriptionConstructor() {}
CurvedBoundaryDescriptionConstructor::CurvedBoundaryDescriptionConstructor() {}

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryDescriptionConstructor::newDescription(std::string name) const
{
  if(name == "CurvedBoundaryDescriptionQuadratic2d")
  {
    return new CurvedBoundaryDescriptionQuadratic2d;
  }
  else if(name == "CurvedBoundaryDescriptionNozzle")
  {
    return new CurvedBoundaryDescriptionNozzle;
  }
  else if(name == "CurvedBoundaryDescriptionWingEllipticProfil")
  {
    return new CurvedBoundaryDescriptionWingEllipticProfil;
  }
  else if(name == "CurvedBoundaryDescriptionNaca")
  {
    return new CurvedBoundaryDescriptionNaca;
  }
  else
  {
    std::cerr << "*** CurvedBoundaryDescriptionConstructor() unkwnown CurvedBoundaryDescription " << name << "\n";
    assert(0);
  }
}