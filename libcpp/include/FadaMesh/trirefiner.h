#ifndef  __FadaMesh_TriRefiner_h
#define  __FadaMesh_TriRefiner_h

#include  "typedefs.h"
#include  "tri.h"
#include  "facerefinerinterface.h"
#include  <algorithm>

/*
--------------------------------------------------------------------------
//
//
//   o
//   | \
//   |  \
//   o---o
//   |\  /\
//   | \/  \
//   o--o---o
//
//
--------------------------------------------------------------------------
*/

namespace FadaMesh
{
  class TriRefiner : public FaceRefinerInterface
  {
  public:

    TriRefiner() : FaceRefinerInterface() {}
    void reInit()
    {
      newedges.reserve(3); newedges.resize(3);
      newfaces.reserve(4); newfaces.resize(4);
    }
    bool refine(FaceInterface* f,tree<Edge*>& _Edges, int& edgecount);
  };
}

/*--------------------------------------------------------------------------*/

#endif
