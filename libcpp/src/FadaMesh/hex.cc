#include  "FadaMesh/hex.h"

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
Hex::~Hex(){}
Hex::Hex():_oldid(0), _id(0), _reftype(0){}
Hex::Hex(int id):_id(id){}
std::string Hex::getName() const {return "Hex";}
void Hex::set_size(std::istream& in, const std::map<int,Node*>& id2node,const std::map<int,edge_pointer>& id2edge, const std::map<int,face_pointer>& id2face)
{
  int ii;
  for(int i=0;i<6;i++) {in>>ii;face(i) = id2face.find(ii)->second; assert(*face(i));}
  for(int i=0;i<12;i++) {in>>ii;edge(i) = id2edge.find(ii)->second; assert(*edge(i));}
  for(int i=0;i<8;i++) {in>>ii;node(i) = id2node.find(ii)->second; assert(node(i));}
}
int Hex::NumberOfNodes() const {return 8;}
int Hex::NumberOfFaces() const {return 6;}
int Hex::NumberOfEdges() const {return 12;}
const int& Hex::id() const {return _id;}
int& Hex::id() {return _id;}
const int& Hex::oldid() const {return _oldid;}
int& Hex::oldid() {return _oldid;}
const int& Hex::reftype() const {return _reftype;}
int& Hex::reftype() {return _reftype;}
    
const face_pointer& Hex::face(int i) const {return _sides[i];}
face_pointer& Hex::face(int i) {return _sides[i];}
const edge_pointer& Hex::edge(int i) const{ return _edges[i];}
edge_pointer& Hex::edge(int i) {return _edges[i];}
const Node* Hex::node(int i) const {return _nodes[i];}
Node*& Hex::node(int i) {return _nodes[i];}


void Hex::write(std::ostream& out, std::string datatype) const
{
  out << id() << " " << oldid() << " " << " ";
  for(int i=0;i<6;i++) out << (*face(i))->id() << " ";
  for(int i=0;i<12;i++) out << (*edge(i))->id() << " ";
  for(int i=0;i<8;i++) out << node(i)->id() << " ";
}
