#ifndef __FadaMesh_CurvedBoundaryDescriptionInterface_h
#define __FadaMesh_CurvedBoundaryDescriptionInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
  class Node;
}

namespace FadaMesh
{
  class CurvedBoundaryDescriptionInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~CurvedBoundaryDescriptionInterface();
    CurvedBoundaryDescriptionInterface();
    CurvedBoundaryDescriptionInterface( const CurvedBoundaryDescriptionInterface& curvedboundarydescriptioninterface);
    CurvedBoundaryDescriptionInterface& operator=( const CurvedBoundaryDescriptionInterface& curvedboundarydescriptioninterface);

    virtual double operator()(double x, double y, double z) const;
    virtual void grad(Alat::Node& dst, const Alat::Node& src) const = 0;
    virtual double newton(Alat::Node&) const = 0;
    virtual double newton(Alat::Node&, const Alat::Node& x1, const Alat::Node& x2) const=0;
    virtual void write(std::ostream& out, std::string type = "ascii") const;
    virtual void read(std::istream& in);
    virtual Alat::DoubleVector& getParameters() = 0;
    virtual const Alat::DoubleVector& getParameters() const = 0;
    virtual CurvedBoundaryDescriptionInterface* clone() const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
