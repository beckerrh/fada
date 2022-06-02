#ifndef  __FadaMesh_CurvedBoundaryDescriptionNozzle_h
#define  __FadaMesh_CurvedBoundaryDescriptionNozzle_h

#include  "curvedboundarydescriptionbase.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionNozzle : public CurvedBoundaryDescriptionBase
  {
public:
    CurvedBoundaryDescriptionNozzle();
    CurvedBoundaryDescriptionNozzle(const CurvedBoundaryDescriptionNozzle& curvedboundarydescription);
    ~CurvedBoundaryDescriptionNozzle();
    FadaMesh::CurvedBoundaryDescriptionInterface* clone() const;
    std::string getName() const;
    double operator()(double x, double y, double z) const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
  };
}

/*---------------------------------------------------*/

#endif
