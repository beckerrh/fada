#include  "FadaMesh/quad.h"
#include  "FadaMesh/edge.h"
#include  "FadaMesh/volumeinterface.h"

using namespace std;
using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

Quad::Quad() :_oldid (-1),_boundaryid(-1) , FaceInterface()
{
  for(int ii=0;ii<4;ii++) _edges[ii]=NULL;
  for(int ii=0;ii<4;ii++) _nodes[ii]=NULL;
  for(int ii=0;ii<2;ii++) _volumes[ii]=NULL;
}

/*--------------------------------------------------------------------------*/

Quad::Quad(int id) : _oldid(-1), _boundaryid(-1), FaceInterface()
{
  _id=id;
  for(int ii=0;ii<4;ii++) _edges[ii]=NULL;
  for(int ii=0;ii<4;ii++) _nodes[ii]=NULL;
  for(int ii=0;ii<2;ii++) _volumes[ii]=NULL;
}
/*--------------------------------------------------------------------------*/
Quad::Quad(std::istream& in)
{
  in >> _id >> _oldid>>_boundaryid;
  for(int i=0;i<4;i++) in >> (*edge(i))->id();
  for(int i=0;i<4;i++) in >> node(i)->id();  
}
/*--------------------------------------------------------------------------*/

void Quad::write(std::ostream& out, std::string datatype) const
{
  out << id() << " " << oldid()<<" "<< _boundaryid<< " " << " ";
  for(int i=0;i<4;i++) out << (*edge(i))->id() << " ";
  for(int i=0;i<4;i++) out << node(i)->id() << " ";
  for(int i=0;i<2;i++) {if(volume(i)!=NULL) out << (*volume(i))->id() << " "; else out << -1<<" ";}
  out<<'\n';
  
}

/*--------------------------------------------------------------------------*/

void Quad::set_size(std::istream& in, const std::map<int,Node*>& id2node, const std::map<int,edge_pointer>& id2edge, const std::map<int, volume_pointer>& id2volume)
{
  int ii;
  for(int i=0;i<4;i++) {in>>ii;edge(i) = id2edge.find(ii)->second;assert(id2edge.find(ii)!=id2edge.end()); assert(*edge(i));}
  for(int i=0;i<4;i++) {in>>ii;node(i) = id2node.find(ii)->second;assert(id2node.find(ii)!=id2node.end()); assert(node(i));}
  int i=0,volumenumber;
  in>>volumenumber; if(volumenumber>=0) {volume(i) = id2volume.find(volumenumber)->second;i=i+1;}
  in>>volumenumber; if(volumenumber>=0) {volume(i) = id2volume.find(volumenumber)->second;i=i+1;}
}
/*--------------------------------------------------------------------------*/

void Quad::set_size(std::istream& in, const std::map<int,Node*>& id2node, const std::map<int,edge_pointer>& id2edge)
{
  int ii;
  for(int i=0;i<4;i++) {in>>ii;edge(i) = id2edge.find(ii)->second;assert(id2edge.find(ii)!=id2edge.end()); assert(*edge(i));}
  for(int i=0;i<4;i++) {in>>ii;node(i) = id2node.find(ii)->second;assert(id2node.find(ii)!=id2node.end()); assert(node(i));}
  for(int i=0;i<2;i++) {in>>ii;volume(i)=NULL;}
}
/*--------------------------------------------------------------------------*/
// int Quad::LocalEdge(const Edge & e){
// int local_side=-1;
 
// for(int i=0; i<4;i++)
//     if((*edge(i))->id()==e.id()) {local_side=i; break;}
// if(local_side<0) {
//   std::cerr<<"Edge Id :"<<e.id()<<'\n';
//   for(int i=0; i<4;i++) std::cerr<<"Local Edge: "<<i<<"  id:"<<(*edge(i))->id()<<'\n';
// }
// assert(local_side!=-1);
// return local_side;
// }
