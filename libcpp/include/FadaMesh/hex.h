#ifndef __FadaMesh_Hex_h
#define __FadaMesh_Hex_h

#include  "typedefs.h"
#include  "Alat/fixarray.h"
#include  "edge.h"
#include  "node.h"
#include  "volumeinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Hex : public VolumeInterface
  {
  private:
    int _oldid, _id, _reftype;                
    Alat::FixArray<6,face_pointer> _sides;
    Alat::FixArray<12,edge_pointer> _edges;
    Alat::FixArray<8,Node*> _nodes;

  public:
    ~Hex();
    Hex();
    Hex(int id);
    std::string getName() const;
    void set_size(std::istream& in, const std::map<int,Node*>& id2node,const std::map<int,edge_pointer>& id2edge, const std::map<int,face_pointer>& id2face);
    int NumberOfNodes() const;
    int NumberOfFaces() const;
    int NumberOfEdges() const;
    const int& id() const;
    int& id();
    const int& oldid() const;
    int& oldid();
    const int& reftype() const;
    int& reftype();    
    const face_pointer& face(int i) const;
    face_pointer& face(int i);
    const edge_pointer& edge(int i) const;
    edge_pointer& edge(int i);
    const Node* node(int i) const;
    Node*& node(int i);
    void write(std::ostream& out, std::string datatype) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
