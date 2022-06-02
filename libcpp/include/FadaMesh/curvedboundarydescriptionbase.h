#ifndef  __FadaMesh_CurvedBoundaryDescriptionBase_h
#define  __FadaMesh_CurvedBoundaryDescriptionBase_h

#include  "curvedboundarydescriptioninterface.h"
#include  "Alat/doublevector.h"

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryDescriptionBase : public FadaMesh::CurvedBoundaryDescriptionInterface
  {
private:
    Alat::DoubleVector _parameters;

protected:
    double getParameter(int i) const;

public:
    ~CurvedBoundaryDescriptionBase();
    CurvedBoundaryDescriptionBase();
    CurvedBoundaryDescriptionBase(const CurvedBoundaryDescriptionBase& curvedboundarydescriptionbase);
    CurvedBoundaryDescriptionBase& operator=(const CurvedBoundaryDescriptionBase& curvedboundarydescriptionbase);

    Alat::DoubleVector& getParameters();
    const Alat::DoubleVector& getParameters() const;
    void grad(Alat::Node& dst, const Alat::Node& src) const;
    double newton(Alat::Node&) const;
    double newton(Alat::Node&, const Alat::Node& x1, const Alat::Node& x2) const;
    void write(std::ostream& out, std::string datatype = "ascii") const;
    void read(std::istream& in);
  };
}

/*---------------------------------------------------*/

#endif
