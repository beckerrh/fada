#ifndef __Alat_VectorInterface_h
#define __Alat_VectorInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class VectorInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~VectorInterface();
    VectorInterface();
    VectorInterface( const Alat::VectorInterface& vectorinterface);
    VectorInterface& operator=( const Alat::VectorInterface& vectorinterface);
    virtual VectorInterface* clone() const;

    virtual double norm() const;
    virtual double scalarProduct(const Alat::VectorInterface* v) const;
    virtual void equal(const Alat::VectorInterface* v);
    virtual void equal(double d);
    virtual void add(const double& d, const Alat::VectorInterface* v);
    virtual void scale(const double& d);
    virtual void zeros();
    virtual std::ostream& writeAscii(std::ostream& os) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
