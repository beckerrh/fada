#ifndef __FadaMesh_MarkerRandom_h
#define __FadaMesh_MarkerRandom_h

#include  "markerbase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerRandom : public MarkerBase
  {
private:
    int _n;
    void _mark();

public:
    MarkerRandom(int n, double marking_parameter) : MarkerBase(marking_parameter), _n(n) {}
    ~MarkerRandom() {}
    std::string getName() const
    {
      return "MarkerRandom";
    }

    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
