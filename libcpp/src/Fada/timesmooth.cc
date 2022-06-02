#include  "Alat/doublevector.h"

/*---------------------------------------------------------*/

namespace Fada
{
  double timeSmoothSpline(double a, double t)
  {
    if( t >= a )
    {
      return 1.0;
    }
    double ta = t/a;
    return ta*ta*( 3.0 -2.0*ta );
  }
}
