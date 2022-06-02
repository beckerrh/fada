#ifndef  __Fada_changeIntegrationPoint_h
#define  __Fada_changeIntegrationPoint_h

#include  "Alat/node.h"

/*-----------------------------------------*/

namespace Fada
{
  class FemInterface;
}

namespace Fada
{
  void changeIntegrationPoint(Alat::Node& sr, const Alat::Node& sl, int offset);
  // void findCorrectOffset(Alat::Node& sr, const Alat::Node& sl, Fada::FemInterface* FEML, Fada::FemInterface* FEMR);
}

#endif
