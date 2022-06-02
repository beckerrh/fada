#include  "Fada/feminterface.h"
#include  "Fada/changeintegrationpoint.h"
#include  <stdlib.h>
#include  <cassert>

/*-----------------------------------------*/

void Fada::changeIntegrationPoint(Alat::Node& sr, const Alat::Node& sl, int offset)
{
  // std::cerr << "changeIntegrationPoint() offset " << offset << "\n";
  sr = sl;
  // pour non-structuré 2D
  if(offset == -1)
  {
    sr.x() = 1.0-sl.x();
  }
  // pour non-structuré 3D
  else if(offset == 0)
  {
    sr.x() = sl.y();
    sr.y() = sl.x();
  }
  else if(offset == 1)
  {
    sr.x() = -sl.x();
    sr.y() = sl.y();
  }
  else if(offset == 2)
  {
    sr.x() = -sl.y();
    sr.y() = -sl.x();
  }
  else if(offset == 3)
  {
    sr.x() = sl.x();
    sr.y() = -sl.y();
  }
  else
  {
    std::cerr<<"changeIntegrationPoint"<<" Error offset "<< offset << std::endl;
    assert(0);
    exit(1);
  }
}

/*-----------------------------------------*/

// void Fada::findCorrectOffset(Alat::Node& sr, const Alat::Node& sl, Fada::FemInterface* FEML, Fada::FemInterface* FEMR)
// {
//   for(int offset=0;offset<4;offset++)
//   {
//     Fada::changeIntegrationPoint(sr, sl, offset);
//     FEML->reInitReferencePointBoundary(sl);
//     FEMR->reInitReferencePointBoundary(sr);
//
//     Alat::Node vl, vr;
//     vl = FEML->x();
//     vr = FEMR->x();
//     vr -= vl;
//     // if(vr.norm() < 1e-6)
//     if(arma::norm(vr) < 1e-6)
//     {
//       std::cerr << "Fada::findCorrectOffset() :  correct offset : "<< offset << "\n";
//       assert(0);
//     }
//   }
//   std::cerr << "Fada::findCorrectOffset() : no correct offset found !\n";
//   assert(0);
// }
