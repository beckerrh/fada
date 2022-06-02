#ifndef __FadaMesh_MarkerMean_h
#define __FadaMesh_MarkerMean_h

#include  "markerbase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerMean : public MarkerBase
  { 
  private:
    void _mark();
    
  public:
    MarkerMean(double marking_parameter) : MarkerBase(marking_parameter) {}
    ~MarkerMean() {}
    std::string getName() const {return "MarkerMean";}
    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
