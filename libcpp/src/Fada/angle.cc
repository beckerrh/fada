#include  "Fada/angle.h"
#include  <cmath>

namespace Fada
{
  double phi(double x, double y)
  {
    if(y >= 0.0)
    {
      return atan2(y, x);
    }
    else
    {
      return 2.0*M_PI-atan2(-y, x);
    }
  }
}