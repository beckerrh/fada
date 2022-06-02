#include  "Alat/node.h"
#include  "FadaMesh/curvedboundarydescriptionnaca.h"
#include  <math.h>
#include  <cassert>

using namespace FadaMesh;

/*---------------------------------------------------*/

CurvedBoundaryDescriptionNaca::CurvedBoundaryDescriptionNaca() : CurvedBoundaryDescriptionBase()
{}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionNaca::CurvedBoundaryDescriptionNaca(const CurvedBoundaryDescriptionNaca& curvedboundarydescription) : CurvedBoundaryDescriptionBase(curvedboundarydescription)
{
  assert(0);
}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionNaca::~CurvedBoundaryDescriptionNaca() {}

/*---------------------------------------------------*/

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryDescriptionNaca::clone() const
{
  return new CurvedBoundaryDescriptionNaca(*this);
}

/*---------------------------------------------------*/

std::string CurvedBoundaryDescriptionNaca::getName() const
{
  return "CurvedBoundaryDescriptionNaca";
}

/*---------------------------------------------------*/

double CurvedBoundaryDescriptionNaca::operator()(double x, double y, double z) const
{
  const Alat::DoubleVector& p = getParameters();
  assert(p.size() == 5);

  double d = p[0]*sqrt(x) + p[1]*x + p[2]*x*x + p[3]*x*x*x + p[4]*x*x*x*x;

  if(y >= 0.0)
  {
    return d - y;
  }
  return d + y;
}

/*---------------------------------------------------*/

void CurvedBoundaryDescriptionNaca::grad(Alat::Node& dst, const Alat::Node& src) const
{
  const Alat::DoubleVector& p = getParameters();
  dst.x() = 0.5*p[0]/sqrt( src.x() ) + p[1] + 2.0*p[2]*src.x() + 3.0*p[3]*src.x()*src.x() + 4.0*p[4]*src.x()*src.x()*src.x();

  if(src.y() >= 0.0)
  {
    dst.y() = -1.0;
  }
  else
  {
    dst.y() = 1.0;
  }
  dst.z() = 0.0;
}
