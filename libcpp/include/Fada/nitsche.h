#ifndef  __Fada_Nitsche_h
#define  __Fada_Nitsche_h

#include  "Alat/stringvector.h"

/*---------------------------------------------*/

namespace Fada
{
  void nitsche(double& gamma, double& alphaL, double& alphaR, double hS, double hL, double hR, double kL, double kR);
}

#endif
