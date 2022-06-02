#include  "FadaMesh/faceinterface.h"
#include  "FadaMesh/edge.h"
#include  "FadaMesh/node.h"

using namespace std;
using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

Edge::Edge() 
{
  _oldid = _id = _boundaryid =-1; nref = 1; _to_refine=0; _to_coarsen=0; _copy=0;
  nfaces=0;nvolumes=0;
  for(int ii=0;ii<2;ii++) {_nodes[ii] = NULL;_faces[ii] = NULL;}
}

/*--------------------------------------------------------------------------*/

Edge::Edge(int id) : _oldid(id), _id(id), _boundaryid(-1), _copy(0)
{
  nfaces=0;nref=1;_to_refine=0;_to_coarsen=0;
  for(int ii=0;ii<2;ii++) {_nodes[ii] = NULL;_faces[ii]=NULL;}
}
/*--------------------------------------------------------------------------*/

Edge::Edge(std::istream& in)
{
  nfaces=0;nref=1;_to_refine=0;_to_coarsen=0;
  for(int ii=0;ii<2;ii++) {_nodes[ii] = NULL;_faces[ii]=NULL;}
  in >> _id >> _oldid >> nref >> _boundaryid>>_copy;
  for(int i=0;i<2;i++) in >> node(i)->getNode();
}
/*--------------------------------------------------------------------------*/

void Edge::write(std::ostream& out, std::string datatype) const
{
  out << id() << " " << oldid() << " " << nref<< " " << boundaryid() << " "<<_copy<<" ";
  for(int i=0;i<2;i++) out << node(i)->id() << " ";
  for(int i=0;i<2;i++) {if(face(i)!=NULL) out << (*face(i))->id() << " "; else out << -1<<" ";}
  out<<'\n';
}
/*--------------------------------------------------------------------------*/

void Edge::set_size(std::istream& in, const std::map<int,Node*>& id2node,const std::map<int,face_pointer>& id2face )
{
  in  >> nref >> _boundaryid>>_copy;
  int nodenumber, facenumber;
  for(int i=0;i<2;i++) {in>>nodenumber; node(i) = id2node.find(nodenumber)->second;}
  int i=0;
  in>>facenumber; if(facenumber>=0) {face(i) = id2face.find(facenumber)->second;i=i+1;}
  in>>facenumber; if(facenumber>=0) {face(i) = id2face.find(facenumber)->second;i=i+1;}

  nfaces=i;
  
}
/*--------------------------------------------------------------------------*/

std::pair<bool, std::pair <int,int> > Edge::Intercept(const Edge & e)
{
  std::pair<bool, std::pair <int,int> > rep=std::make_pair<bool,std::pair <int,int> >(0,std::make_pair<int,int>(-1,-1));
  
  int id0=e.node(0)->id(),id1=e.node(1)->id();
  if(id0==node(0)->id()) {rep.first=1;rep.second=std::make_pair<int,int>(0,0);}
  if(id0==node(1)->id()) {rep.first=1;rep.second=std::make_pair<int,int>(1,0);}
  if(id1==node(0)->id()) {rep.first=1;rep.second=std::make_pair<int,int>(0,1);}
  if(id1==node(1)->id()) {rep.first=1;rep.second=std::make_pair<int,int>(1,1);}
  return rep;
}


