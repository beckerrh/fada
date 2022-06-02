#ifndef __FadaMesh_MarkerBase_h
#define __FadaMesh_MarkerBase_h

#include  "markerinterface.h"
#include  "Alat/intvector.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerBase : public MarkerInterface
  {
protected:
    double _marking_parameter;
    Alat::DoubleVector _indicator;
    Alat::IntVector _marked_cells;
    Alat::IntVector _C;

protected:
    void _sort();

public:
    MarkerBase(double marking_parameter);
    ~MarkerBase() {}
    std::string getName() const
    {
      return "MarkerBase";
    }

    Alat::DoubleVector& getIndicator()
    {
      return _indicator;
    }

    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
