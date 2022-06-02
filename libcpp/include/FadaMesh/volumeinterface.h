#ifndef __FadaMesh_VolumeInterface_h
#define __FadaMesh_VolumeInterface_h

#include  <string>
#include  <ostream>
#include  "typedefs.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{ 
  class Node;
  class Edge;
  class Face;
  
  
  class VolumeInterface
  {
    bool _to_refine, _to_coarsen;
    int _depth;
  public:
    VolumeInterface() : _to_refine(0), _depth(0) {}
    virtual std::string getName() const=0;

    virtual int NumberOfNodes() const=0;
    virtual int NumberOfFaces() const=0;
    virtual int NumberOfEdges() const =0;
    virtual const Node* node(int i) const=0;
    virtual Node*& node(int i)=0;
    virtual face_pointer& face(int i)=0;
    virtual const face_pointer& face(int i) const=0;
    virtual edge_pointer& edge(int i) =0;
    virtual const edge_pointer& edge(int i) const=0;

    virtual const int& id() const=0;
    virtual int& id()=0;
    virtual const int& oldid() const=0;
    virtual int& oldid()=0;
    int depth() const {return _depth;}
    int& depth() {return _depth;}
    
    virtual void write(std::ostream& out, std::string datatype) const=0;
    bool to_refine() const {return _to_refine;}
    bool& to_refine() {return _to_refine;}
    bool to_coarsen() const {return _to_coarsen;}
    bool& to_coarsen() {return _to_coarsen;}
  };
}

/*--------------------------------------------------------------------------*/

#endif
