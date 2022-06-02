#include  "FadaMesh/tri.h"
#include  "FadaMesh/edge.h"
#include  "FadaMesh/node.h"

using namespace std;
using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

Tri::Tri() : FaceInterface()
{
  _oldid = _id =-1 ;                
  for(int ii=0;ii<3;ii++) _edges[ii]=NULL;
  for(int ii=0;ii<3;ii++) _nodes[ii]=NULL;
}

/*--------------------------------------------------------------------------*/

Tri::Tri(int id) : FaceInterface(), _oldid(id), _id(id)
{
  for(int ii=0;ii<3;ii++) _edges[ii]=NULL;
  for(int ii=0;ii<3;ii++) _nodes[ii]=NULL;
}
/*--------------------------------------------------------------------------*/
Tri::Tri(std::istream& in)
{
  in >> _id >> _oldid;
  for(int i=0;i<3;i++) in >> (*edge(i))->id();
  for(int i=0;i<3;i++) in >> node(i)->id();  
}
/*--------------------------------------------------------------------------*/

void Tri::write(std::ostream& out, std::string datatype) const
{
  out << id() << " " << oldid() << " " << " ";
  for(int i=0;i<3;i++) out << (*edge(i))->id() << " ";
  for(int i=0;i<3;i++) out << node(i)->id() << " ";
  out << " ";
}

/*--------------------------------------------------------------------------*/

void Tri::set_size( std::istream& in, const std::map<int,Node*>& id2node, const std::map<int,edge_pointer>& id2edge)
{ 
  int ii;
  for(int i=0;i<3;i++) {in>>ii;edge(i) = id2edge.find(ii)->second; assert(*edge(i));}
  for(int i=0;i<3;i++) {in>>ii;node(i) = id2node.find(ii)->second; assert(node(i));}
}

/*--------------------------------------------------------------------------*/
int Tri::LocalEdge(const Edge & e){
 int local_side=-1;
 
 for(int i=0; i<3;i++)
     if((*edge(i))->id()==e.id()) {local_side=i; break;}
 if(local_side<0) {
   std::cerr<<"Edge Id :"<<e.id()<<'\n';
   for(int i=0; i<3;i++) std::cerr<<"Local Edge: "<<i<<"  id:"<<(*edge(i))->id()<<'\n';
 }
 assert(local_side!=-1);
 return local_side;
 }




