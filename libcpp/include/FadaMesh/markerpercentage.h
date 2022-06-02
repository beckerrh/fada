#ifndef __FadaMesh_MarkerPercentage_h
#define __FadaMesh_MarkerPercentage_h

#include  "markerbase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerPercentage : public MarkerBase
  {
private:
    void _mark();

public:
    MarkerPercentage(double marking_parameter) : MarkerBase(marking_parameter) {}
    ~MarkerPercentage() {}
    std::string getName() const
    {
      return "MarkerPercentage";
    }
    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
