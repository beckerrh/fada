#include  "Alat/tokenize.h"

namespace Fada
{
  // alphak = k x alpha
  void nitsche(double& gamma, double& alphakL, double& alphakR, double hS, double hL, double hR, double kL, double kR)
  {
    double dL = hL*hL*kR;
    double dR = hR*hR*kL;
    double d = 1./( dL+dR );
    // gamma = hS*kL*kR*d;
    gamma = 0.5*(hL+hR)*kL*kR*d;
    alphakR = dR*kR*d;
    alphakL = dL*kL*d;
  }
}