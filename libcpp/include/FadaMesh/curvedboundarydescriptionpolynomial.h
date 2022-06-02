#ifndef  __FadaMesh_CurvedBoundaryDescriptionPolynomial_h
#define  __FadaMesh_CurvedBoundaryDescriptionPolynomial_h

#include  "Alat/node.h"
#include  "curvedboundarydescriptionbase.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionLinear2d : public CurvedBoundaryDescriptionBase
  {
    const Alat::Node& _n1, _n2;
    double _a, _b, _c;
public:
    CurvedBoundaryDescriptionLinear2d(const Alat::Node& n1,  const Alat::Node& n2);
    CurvedBoundaryDescriptionLinear2d(const CurvedBoundaryDescriptionLinear2d& curvedboundarydescription);
    ~CurvedBoundaryDescriptionLinear2d();
    FadaMesh::CurvedBoundaryDescriptionInterface* clone() const;
    std::string getName() const;
    double operator()(double x, double y, double z) const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
  };

  class CurvedBoundaryDescriptionQuadratic2d : public CurvedBoundaryDescriptionBase
  {
public:
    CurvedBoundaryDescriptionQuadratic2d();
    CurvedBoundaryDescriptionQuadratic2d(const CurvedBoundaryDescriptionQuadratic2d& curvedboundarydescription);
    ~CurvedBoundaryDescriptionQuadratic2d();
    FadaMesh::CurvedBoundaryDescriptionInterface* clone() const;
    std::string getName() const;
    double operator()(double x, double y, double z) const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
  };
}

/*---------------------------------------------------*/

#endif
