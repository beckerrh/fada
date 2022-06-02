#ifndef  __FadaMesh_QuadRefiner_h
#define  __FadaMesh_QuadRefiner_h

#include  "typedefs.h"
#include  "facerefinerinterface.h"
#include  <algorithm>

/*--------------------------------------------------------------------------*/
//
//
//   o------o------o
//   |      |      |
//   |      |      |
//   o------o------o
//   |      |      |
//   |      |      |
//   o------o------o
//
//
//
//
namespace FadaMesh
{ 

  class QuadRefiner : public FaceRefinerInterface
  {
  public:

    QuadRefiner() : FaceRefinerInterface() {}
    void set_size(int n1,int n2)
    { 
      int nn=(n1-1)*(n2-1);
      newnodes.reserve(nn); newnodes.resize(nn);
      int ne=n1*(n2-1)+n2*(n1-1);
      newedges.reserve(ne); newedges.resize(ne);
      int nf=n1*n2;
      newfaces.reserve(nf); newfaces.resize(nf);
    }
    bool coarse() {return 0;}
    bool refine(FaceInterface* f, tree<Edge*>& _Edges, int& edgecount);
  };
}

/*--------------------------------------------------------------------------*/

#endif
