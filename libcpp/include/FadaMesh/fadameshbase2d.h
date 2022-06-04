#ifndef __FadaMesh_FadaMeshBase2d_h
#define __FadaMesh_FadaMeshBase2d_h

#include  "fadameshbase.h"

/*---------------------------------------------------------*/

namespace FadaMesh
{
  template<int NODESPERCELL>
  class FadaMeshBase2d : public FadaMeshBase<2, NODESPERCELL, NODESPERCELL, 2>
  {
public:
    FadaMeshBase2d<NODESPERCELL>( );
    std::string getName() const;
    int getNEdgesPerCell(int i) const;
    int getNEdgesPerSide(int i) const;
    int getVtkType() const;
    // std::string getEnsightType() const;
    int getBoundaryVtkType() const;
  };
}

/*---------------------------------------------------------*/

#endif
