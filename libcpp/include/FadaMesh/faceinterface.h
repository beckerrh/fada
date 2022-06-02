#ifndef __FadaMesh_FaceInterface_h
#define __FadaMesh_FaceInterface_h

#include  <string>
#include  "typedefs.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Node;
  class Edge;
  class VolumeInterface;
  
  class FaceInterface
  {
  public:
    int _id;
  protected:
    
  bool _to_refine, _to_coarsen;
  int _depth;
//  int _id;


  public:
    int nvolumes;
    FaceInterface() : _to_refine(0),_to_coarsen(0), _depth(0), nvolumes(0),_id(-1) {}
    ~FaceInterface(){}
    virtual std::string getName() const=0;

    virtual int NumberOfNodes() const=0;
    virtual int NumberOfEdges() const=0;
    
    virtual const Node* node(int i) const=0;
    virtual Node*& node(int i)=0;
    
    virtual const edge_pointer& edge(int i) const=0;
    virtual edge_pointer& edge(int i)=0;
    
    virtual const volume_pointer& volume(int i) const{assert(0);}
    virtual volume_pointer& volume(int i) {assert(0);}

    virtual int id() const=0;
    virtual int& id()=0;
    virtual const int& oldid() const=0;
    virtual int& oldid()=0;
   
    virtual const int& boundaryid() const=0;
    virtual int& boundaryid()=0;
    
    bool to_refine() const {return _to_refine;}
    bool& to_refine() {return _to_refine;}
    bool to_coarsen() const {return _to_coarsen;}
    bool& to_coarsen() {return _to_coarsen;}
    int depth() const {return _depth;}
    int& depth() {return _depth;}
    
    virtual int LocalEdge(const Edge & e){assert(0);}
    virtual void write(std::ostream& out, std::string datatype) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
