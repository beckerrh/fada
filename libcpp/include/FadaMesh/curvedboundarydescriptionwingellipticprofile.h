#ifndef  __FadaMesh_CurvedBoundaryDescriptionWingEllipticProfil_h
#define  __FadaMesh_CurvedBoundaryDescriptionWingEllipticProfil_h

#include  "curvedboundarydescriptionbase.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionWingEllipticProfil : public CurvedBoundaryDescriptionBase
  {
public:
    CurvedBoundaryDescriptionWingEllipticProfil();
    CurvedBoundaryDescriptionWingEllipticProfil(const CurvedBoundaryDescriptionWingEllipticProfil& curvedboundarydescription);
    ~CurvedBoundaryDescriptionWingEllipticProfil();
    FadaMesh::CurvedBoundaryDescriptionInterface* clone() const;
    std::string getName() const;
    double operator()(double x, double y, double z) const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
  };
}

/*---------------------------------------------------*/

#endif
