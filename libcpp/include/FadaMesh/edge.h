#ifndef __FadaMesh_Edge_h
#define __FadaMesh_Edge_h

#include  "Alat/fixarray.h"
#include  "faceinterface.h"
#include  "typedefs.h"
#include  "Alat/node.h"
#include <map>

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Edge
  {
  private:
    int _oldid, _id, _boundaryid;
    Alat::FixArray<2,Node*> _nodes;
    Alat::FixArray<2,face_pointer> _faces;
    int _copy;

  public:
    int nref, nfaces, nvolumes;
    bool _to_refine, _to_coarsen;
    Edge();
    Edge(int id); 
    Edge(std::istream& is);
    ~Edge(){}
    std::string getName() const {return "Edge";}
    void set_size(std::istream& in, const std::map<int,Node*>& id2node,const std::map<int,face_pointer>& id2face);

    const int& id() const {return _id;}
    int& id() {return _id;}
    
    const int& oldid() const {return _oldid;}
    int& oldid() {return _oldid;}
    
    const int& boundaryid() const {return _boundaryid;}
    int& boundaryid() {return _boundaryid;}

    const Node* node(int i) const {return _nodes[i];}
    Node*& node(int i) {return _nodes[i];}
    
    const face_pointer face(int i) const {return _faces[i];}
    face_pointer& face(int i) {return _faces[i];}
    
    bool to_refine() const {return _to_refine;}
    bool& to_refine() {return _to_refine;}
    
    bool to_coarsen() const {return _to_coarsen;}
    bool& to_coarsen() {return _to_coarsen;}
    
    int& copy(){return _copy;}
    const int copy() const {return _copy;}
    
    std::pair<bool, std::pair <int,int> > Intercept(const Edge & e); 

    void write(std::ostream& out, std::string datatype) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
