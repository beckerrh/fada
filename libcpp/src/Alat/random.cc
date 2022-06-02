#include  "Alat/random.h"
#include  <stdlib.h>

namespace Alat
{
  double rangedRandomNumber(double fMin, double fMax)
  {
    double random =   (double)rand();
    return fMin + random * ( fMax - fMin )/ (double) RAND_MAX;
  }
}