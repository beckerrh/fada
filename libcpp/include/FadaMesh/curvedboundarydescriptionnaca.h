#ifndef  __FadaMesh_CurvedBoundaryDescriptionNaca_h
#define  __FadaMesh_CurvedBoundaryDescriptionNaca_h

#include  "curvedboundarydescriptionbase.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionNaca : public CurvedBoundaryDescriptionBase
  {
public:
    CurvedBoundaryDescriptionNaca();
    CurvedBoundaryDescriptionNaca(const CurvedBoundaryDescriptionNaca& curvedboundarydescription);
    ~CurvedBoundaryDescriptionNaca();
    FadaMesh::CurvedBoundaryDescriptionInterface* clone() const;
    std::string getName() const;
    double operator()(double x, double y, double z) const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
  };
}

/*---------------------------------------------------*/

#endif
