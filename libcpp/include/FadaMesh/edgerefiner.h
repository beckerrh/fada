#ifndef __FadaMesh_EdgeRefiner_h
#define __FadaMesh_EdgeRefiner_h
#include  "hanging_info.h"
#include "edge.h"
/*--------------------------------------------------------------------------*/

namespace FadaMesh
{ 

//    Nodes
//          
//     1 
//     |
//     .
//     .
//     . 
//     |  
//     3     
//     |
//     2  
//     |   
//     0 
//          
  class EdgeRefiner 
  {
  private: 
    std::vector<Node*> newnodes;
    std::vector<Edge*> newedges;

  public:

    EdgeRefiner(){}
    void  set_size(int n)
    { 
      newnodes.reserve(n-1); newnodes.resize(n-1);
      newedges.reserve(n);newedges.resize(n);
    }
    std::vector<Node*>& getNewNodes(){return newnodes;}
    std::vector<Edge*>& getNewEdges(){return newedges;}     
    void  refine(edge_pointer ep, int& nodecount, int & edgecount);
  };
}

/*--------------------------------------------------------------------------*/

#endif
