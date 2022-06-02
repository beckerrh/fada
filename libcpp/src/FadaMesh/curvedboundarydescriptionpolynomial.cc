#include  "FadaMesh/curvedboundarydescriptionpolynomial.h"
#include  <cassert>

using namespace FadaMesh;

/*---------------------------------------------------*/

CurvedBoundaryDescriptionLinear2d::CurvedBoundaryDescriptionLinear2d(const Alat::Node& n1,  const Alat::Node& n2) : CurvedBoundaryDescriptionBase(), _n1(n1), _n2(n2)
{
  _a = _n2.y()-_n1.y();
  _b = _n1.x()-_n2.x();
  _c = -( _a*_n1.x()+_b*_n1.y() );
}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionLinear2d::CurvedBoundaryDescriptionLinear2d(const CurvedBoundaryDescriptionLinear2d& curvedboundarydescription) : CurvedBoundaryDescriptionBase(curvedboundarydescription), _n1(curvedboundarydescription._n1), _n2(curvedboundarydescription._n2)

{
  assert(0);
}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionLinear2d::~CurvedBoundaryDescriptionLinear2d() {}

/*---------------------------------------------------*/

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryDescriptionLinear2d::clone() const
{
  return new CurvedBoundaryDescriptionLinear2d(*this);
}

/*---------------------------------------------------*/

std::string CurvedBoundaryDescriptionLinear2d::getName() const
{
  return "CurvedBoundaryDescriptionLinear2d";
}

/*---------------------------------------------------*/

double CurvedBoundaryDescriptionLinear2d::operator()(double x, double y, double z) const
{
  return _a*x+_b*y+_c;
}

/*---------------------------------------------------*/

void CurvedBoundaryDescriptionLinear2d::grad(Alat::Node& dst, const Alat::Node& src) const
{
  dst.x() = _a;
  dst.y() = _b;
  dst.z() = 0.0;
}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionQuadratic2d::CurvedBoundaryDescriptionQuadratic2d() : CurvedBoundaryDescriptionBase()
{}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionQuadratic2d::CurvedBoundaryDescriptionQuadratic2d(const CurvedBoundaryDescriptionQuadratic2d& curvedboundarydescription) : CurvedBoundaryDescriptionBase(curvedboundarydescription)
{}

/*---------------------------------------------------*/

CurvedBoundaryDescriptionQuadratic2d::~CurvedBoundaryDescriptionQuadratic2d() {}

/*---------------------------------------------------*/

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryDescriptionQuadratic2d::clone() const
{
  return new CurvedBoundaryDescriptionQuadratic2d(*this);
}

/*---------------------------------------------------*/

std::string CurvedBoundaryDescriptionQuadratic2d::getName() const
{
  return "CurvedBoundaryDescriptionQuadratic2d";
}

/*---------------------------------------------------*/

double CurvedBoundaryDescriptionQuadratic2d::operator()(double x, double y, double z) const
{
  const Alat::DoubleVector& p = getParameters();
  assert(p.size() == 6);
  return p[0] + p[1]*x + p[2]*y + p[3]*x*x + p[4]*y*y + p[5]*x*y;
}

/*---------------------------------------------------*/

void CurvedBoundaryDescriptionQuadratic2d::grad(Alat::Node& dst, const Alat::Node& src) const
{
  const Alat::DoubleVector& p = getParameters();
  dst.x() = p[1] + 2.0*p[3]*src.x() + p[5]*src.y();
  dst.y() = p[2] + 2.0*p[4]*src.y() + p[5]*src.x();
  dst.z() = 0.0;
}
