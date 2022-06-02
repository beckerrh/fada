#include  "FadaMesh/edgerefiner.h"
#include  "FadaMesh/faceinterface.h"
#include  "FadaMesh/node.h"

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
void  EdgeRefiner::refine(edge_pointer ep, int& nodecount, int & edgecount)
{
  Edge* e=*ep;
  int n=e->nref;
  set_size(n);

 // on construit les noeuds sur l'edge raffiné 
  for(int ii=0;ii<n-1;ii++)
  {
    Node* N = new Node;
    (*N).getNode().zeros();
    N->getNode().add(double(ii+1)/double(n),(*(e->node(0))).getNode());
    N->getNode().add(double(n-ii-1)/double(n),(*(e->node(1))).getNode());
    newnodes[ii]=N;
    N->id()=++nodecount;
  }
  // On construit les nouveaux edges
  // le premier
  Edge* en0 = new Edge;
  en0->node(0)=e->node(0);
  en0->node(1)=newnodes[0];
  en0->boundaryid()=e->boundaryid();
  newedges[0]=en0;
  en0->id()=++edgecount;
  // les noeuds intermédiaires     
  for(int ii=1;ii<n-1;ii++)
  {
    Edge* en = new Edge;
    newedges[ii]=en;
    en->node(0)=newnodes[ii];
    en->node(1)=newnodes[ii+1];
    en->boundaryid()=e->boundaryid();
    en->id()=++edgecount;
  }
  // le dernier
  Edge* enf = new Edge;
  enf->node(0)=newnodes[n-2];
  enf->node(1)=e->node(1);
  enf->boundaryid()=e->boundaryid();
  enf->id()=++edgecount;
  newedges[n-1]=enf;
  e->to_refine()=0;
}   




