#include  "Fada/parabolicprofile.h"
#include <iostream>

/*---------------------------------------------------------*/

namespace Fada
{
  //
  // equals 1 in the middle
  //
  double parabolicProfile2d(double a, double b, double x)
  {                       
    double diff = b-a;
    double scale = -4.0/diff/diff;
    return scale*(x-a)*(x-b);
  }
  double parabolicProfile3d(double ax, double bx, double az, double bz, double x, double z)
  {
    double diff = (bx-ax)*(bz-az);
    double scale = 16.0/diff/diff;
    return scale*(x-ax)*(x-bx)*(z-az)*(z-bz);    
  }
}
