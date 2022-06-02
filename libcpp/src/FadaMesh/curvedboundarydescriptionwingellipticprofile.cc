#include  "Alat/node.h"
#include  "FadaMesh/curvedboundarydescriptionwingellipticprofile.h"
#include  <math.h>
#include  <cassert>

using namespace FadaMesh;

/*---------------------------------------------------*/

CurvedBoundaryDescriptionWingEllipticProfil::CurvedBoundaryDescriptionWingEllipticProfil() : CurvedBoundaryDescriptionBase()
{}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionWingEllipticProfil::CurvedBoundaryDescriptionWingEllipticProfil(const CurvedBoundaryDescriptionWingEllipticProfil& curvedboundarydescription) : CurvedBoundaryDescriptionBase(curvedboundarydescription)
{
  assert(0);
}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionWingEllipticProfil::~CurvedBoundaryDescriptionWingEllipticProfil() {}

/*---------------------------------------------------*/

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryDescriptionWingEllipticProfil::clone() const
{
  return new CurvedBoundaryDescriptionWingEllipticProfil(*this);
}

/*---------------------------------------------------*/

std::string CurvedBoundaryDescriptionWingEllipticProfil::getName() const
{
  return "CurvedBoundaryDescriptionWingEllipticProfil";
}

/*---------------------------------------------------*/

double CurvedBoundaryDescriptionWingEllipticProfil::operator()(double x, double y, double z) const
{
  const Alat::DoubleVector& p = getParameters();
  return ( x-p[0]/2.0 )*( x-p[0]/2.0 )/( p[0]/2.0 )*( p[0]/2.0 ) + y*y/( p[1]*p[0]/2 )*( p[1]*p[0]/2.0 )-1.0;
}

/*---------------------------------------------------*/

void CurvedBoundaryDescriptionWingEllipticProfil::grad(Alat::Node& dst, const Alat::Node& src) const
{
  const Alat::DoubleVector& p = getParameters();
  dst.x() = 2.0*( src.x()-p[0]/2.0 )/( p[0]/2.0 )*( p[0]/2.0 );
  dst.y() = 2.0*src.y()/( p[1]*p[0]/2 )*( p[1]*p[0]/2.0 );
  dst.z() = 0.0;
}
