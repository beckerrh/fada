#ifndef __FadaMesh_MarkerCoarsePercentage_h
#define __FadaMesh_MarkerCoarsePercentage_h

#include  "markerbase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerCoarsePercentage : public MarkerBase
  {
private:
    void _mark();

public:
    MarkerCoarsePercentage(double marking_parameter) : MarkerBase(marking_parameter) {}
    ~MarkerCoarsePercentage() {}
    std::string getName() const
    {
      return "MarkerCoarsePercentage";
    }
    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
