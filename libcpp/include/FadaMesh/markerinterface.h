#ifndef __FadaMesh_MarkerInterface_h
#define __FadaMesh_MarkerInterface_h

#include  "Alat/doublevector.h"
#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const
    {
      return "MarkerInterface";
    }

public:
    MarkerInterface() : Alat::InterfaceBase() {}
    ~MarkerInterface() {}
    virtual Alat::DoubleVector& getIndicator() = 0;
    virtual void write(std::string outfilename, std::string datatype = "binary")=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
