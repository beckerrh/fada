#ifndef __FadaMesh_MarkerOptimalCoarse_h
#define __FadaMesh_MarkerOptimalCoarse_h

#include  "markerbase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MarkerOptimalCoarse : public MarkerBase
  {
private:
    double _s, _p, _alpha;
    int _dimension;
    void _mark();
    double _f(int i, int n, double total, double sum) const;

public:
    MarkerOptimalCoarse(double marking_parameter, int dimension) : MarkerBase(marking_parameter), _dimension(dimension), _s(marking_parameter) {}
    ~MarkerOptimalCoarse() {}
    std::string getName() const
    {
      return "MarkerOptimalCoarse";
    }

    void write(std::string outfilename, std::string datatype = "binary");
  };
}

/*--------------------------------------------------------------------------*/

#endif
