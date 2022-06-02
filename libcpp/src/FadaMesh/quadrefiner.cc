#include "FadaMesh/edge.h"
#include "FadaMesh/faceinterface.h"
#include "FadaMesh/quad.h"
#include "FadaMesh/quadrefiner.h"

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

bool QuadRefiner::refine(FaceInterface* f, tree<Edge*>& _Edges, int& edgecount)
{
  Edge& e0 = *( *( f->edge(0) ) );
  Edge& e1 = *( *( f->edge(1) ) );
  Edge& e2 = *( *( f->edge(2) ) );
  Edge& e3 = *( *( f->edge(3) ) );
  int n1, n2;
  n1 = std::min(e0.nref, e2.nref);
  n2 = std::min(e1.nref, e3.nref);
  if(n1 < 2|n2 < 2)
  {
    return 0;
  }
  int en0 = 1, en1 = 1, en2 = 1, en3 = 1;
  std::pair<bool, std::pair<int, int> > inter_e0_e1 = e0.Intercept(e1);
  assert(inter_e0_e1.first);
  if(inter_e0_e1.second.first == 1)
  {
    en0 = -1;
  }
  if(inter_e0_e1.second.second == 1)
  {
    en1 = -1;
  }
  std::pair<bool, std::pair<int, int> > inter_e2_e3 = e2.Intercept(e3);
  assert(inter_e2_e3.first);
  if(inter_e2_e3.second.first == 0)
  {
    en2 = -1;
  }
  if(inter_e2_e3.second.second == 0)
  {
    en3 = -1;
  }

  set_size(n1, n2);

  std::vector< std::vector<edge_pointer> > childs;
  childs.reserve(4);
  childs.resize(4);
  childs[0].reserve(n1);
  childs[2].reserve(n1);
  childs[1].reserve(n2);
  childs[3].reserve(n2);
  childs[0].resize(n1);
  childs[2].resize(n1);
  childs[1].resize(n2);
  childs[3].resize(n2);

  // childs of edges
  if(n1 > 1)
  {
    for(int ii = 0; ii < n1; ii++)
    {
      if(en0 == 1)
      {
        childs[0][ii] = _Edges.child(f->edge(0), ii);
      }
      else
      {
        childs[0][ii] = _Edges.child(f->edge(0), n1-ii-1);
      }
      if(en2 == 1)
      {
        childs[2][ii] = _Edges.child(f->edge(2), ii);
      }
      else
      {
        childs[2][ii] = _Edges.child(f->edge(2), n1-ii-1);
      }
    }
  }
  else
  {
    childs[0][0] = f->edge(0);
    childs[2][0] = f->edge(2);
  }

  if(n2 > 1)
  {
    for(int ii = 0; ii < n2; ii++)
    {
      if(en1 == 1)
      {
        childs[1][ii] = _Edges.child(f->edge(1), ii);
      }
      else
      {
        childs[1][ii] = _Edges.child(f->edge(1), n2-ii-1);
      }
      if(en3 == 1)
      {
        childs[3][ii] = _Edges.child(f->edge(3), ii);
      }
      else
      {
        childs[3][ii] = _Edges.child(f->edge(3), n2-ii-1);
      }
    }
  }
  else
  {
    childs[1][0] = f->edge(1);
    childs[3][0] = f->edge(3);
  }
  // new nodes
  // version ancienne
   int ind=0;
   Node extrem0 =*(( *( childs[0][0] ) )->node( ( 1-en0 )/2 ));
   Node extrem1 =*(( *( childs[2][0] ) )->node( ( 1-en2 )/2 ));
   Node extrem2 =*(( *( childs[0][n1-1] ) )->node( 1-( 1-en0 )/2 ));
   Node extrem3 =*(( *( childs[2][n1-1] ) )->node(1-( 1-en2 )/2 ));
   for(int ii=1; ii<n1;ii++)
    {
     Edge* child0=*(childs[0][ii]);
     Edge* child2=*(childs[2][ii]);
     Node* node0=child0->node((1-en0)/2);
     Node* node2=child2->node((1-en2)/2);

     Node interp_node0, curv_deplacement0(*node0);

     interp_node0.getNode().zeros();
     interp_node0.getNode().add(double(n1-ii)/double(n1),extrem0.getNode());
     interp_node0.getNode().add(double(ii)/double(n1),extrem2.getNode());
     curv_deplacement0.getNode().add(-1.0,interp_node0.getNode());


     Node interp_node2,curv_deplacement2(*node2);
     interp_node2.getNode().zeros();
     interp_node2.getNode().add(double(n1-ii)/double(n1),extrem1.getNode());
     interp_node2.getNode().add(double(ii)/double(n1),extrem3.getNode());
     curv_deplacement2.getNode().add(-1.0,interp_node2.getNode());

     for(int jj=1; jj<n2;jj++)
     {
       Edge* child1=*(childs[1][jj]);
       Edge* child3=*(childs[3][jj]);
       Node* node1=child1->node((1-en1)/2);
       Node* node3=child3->node((1-en3)/2);

       Node interp_node1,curv_deplacement1(*node1);
       interp_node1.getNode().zeros();
       interp_node1.getNode().add(double(n2-jj)/double(n2),extrem0.getNode());
       interp_node1.getNode().add(double(jj)/double(n2),extrem1.getNode());
       curv_deplacement1.getNode().add(-1.0,interp_node1.getNode());

       Node interp_node3,curv_deplacement3(*node3);
       interp_node3.getNode().zeros();
       interp_node3.getNode().add(double(n2-jj)/double(n2),extrem2.getNode());
       interp_node3.getNode().add(double(jj)/double(n2),extrem3.getNode());
       curv_deplacement3.getNode().add(-1.0,interp_node3.getNode());

       Node* N=new Node;

       N->getNode().zeros();
       N->getNode().add(double(n2-jj)/double(n2),interp_node0.getNode());
       N->getNode().add(double(jj)/double(n2),interp_node2.getNode());
       
       //Correction position
       N->getNode().add(0.25,curv_deplacement0.getNode());
       N->getNode().add(0.25,curv_deplacement1.getNode());
       N->getNode().add(0.25,curv_deplacement2.getNode());
       N->getNode().add(0.25,curv_deplacement3.getNode());
       newnodes[ind]=N;
       ind++;
     }
   }
  // horizontal new edges
  ind = 0;
  for(int ii = 0; ii < n1-1; ii++)
  {
    Edge* ne0 = new Edge;
    ne0->node(0) = ( *( childs[0][ii] ) )->node(1-( 1-en0 )/2);
    ne0->node(1) = newnodes[( n2-1 )*ii];
    newedges[ind] = _Edges.insert(_Edges.end(), ne0);
    //( *newedges[ind] )->id() = edgecount++;
    ( *newedges[ind] )->id() = ++edgecount;
    ind++;
    for(int jj = 1; jj < n2-1; jj++)
    {
      Edge* ne = new Edge;
      ne->node(0) = newnodes[( n2-1 )*ii+jj-1];
      ne->node(1) = newnodes[( n2-1 )*ii+jj];
      newedges[ind] = _Edges.insert(_Edges.end(), ne);
     // ( *newedges[ind] )->id() = edgecount++;
      ( *newedges[ind] )->id() = ++edgecount;
      ind++;
    }
    Edge* nef = new Edge;
    nef->node(1) = ( *( childs[2][ii] ) )->node(1-( 1-en2 )/2);
    nef->node(0) = newnodes[( n2-1 )*( ii+1 )-1];
    newedges[ind] = _Edges.insert(_Edges.end(), nef);
    //( *newedges[ind] )->id() = edgecount++;
    ( *newedges[ind] )->id() = ++edgecount;
    ind++;
  }

  // vertical new edges

  for(int ii = 0; ii < n2-1; ii++)
  {
    Edge* ne = new Edge;
    ne->node(0) = ( *( childs[1][ii] ) )->node(1-( 1-en1 )/2);
    ne->node(1) = newnodes[ii];
    newedges[ind] = _Edges.insert(_Edges.end(), ne);
    //( *newedges[ind] )->id() = edgecount++;
    ( *newedges[ind] )->id() = ++edgecount;
    ind++;
    for(int jj = 1; jj < n1-1; jj++)
    {
      Edge* ne = new Edge;
      ne->node(0) = newnodes[( n2-1 )*jj+ii];
      ne->node(1) = newnodes[( n2-1 )*( jj+1 )+ii];
      newedges[ind] = _Edges.insert(_Edges.end(), ne);
     // ( *newedges[ind] )->id() = edgecount++;
      ( *newedges[ind] )->id() = ++edgecount;
      ind++;
    }
    Edge* nef = new Edge;
    nef->node(1) = ( *( childs[3][ii] ) )->node(1-( 1-en3 )/2);
    nef->node(0) = newnodes[( n1-2 )*( n2-1 )+ii];
    newedges[ind] = _Edges.insert(_Edges.end(), nef);
    //( *newedges[ind] )->id() = edgecount++;
    ( *newedges[ind] )->id() = ++edgecount;
    ind++;
  }

  // on rassemble tous les edges dans un vecteur et tous les nodes
  std::vector<edge_pointer > alledges;
  std::vector<Node*> allnodes;
  int nedges = n1*( n2+1 )+n2*( n1+1 );
  int nnodes = ( n1+1 )*( n2+1 );
  allnodes.resize(nnodes);
  allnodes.reserve(nnodes);
  alledges.resize(nedges);
  alledges.reserve(nedges);
  ind = 0;

  // nodes
  allnodes[0] = ( *( childs[0][0] ) )->node( ( 1-en0 )/2 );
  ind = 1;
  for(int ii = 0; ii < n2; ii++)
  {
    allnodes[ind] = ( *( childs[1][ii] ) )->node(1-( 1-en1 )/2);
    ind++;
  }
  for(int ii = 0; ii < n1-1; ii++)
  {
    allnodes[ind] = ( *( childs[0][ii] ) )->node(1-( 1-en0 )/2);
    ind++;
    for(int jj = 0; jj < n2-1; jj++)
    {
      allnodes[ind] = newnodes[ii*( n2-1 )+jj];
      ind++;
    }
    allnodes[ind] = ( *( childs[2][ii] ) )->node(1-( 1-en2 )/2);
    ind++;
  }
  allnodes[ind] = ( *( childs[3][0] ) )->node( ( 1-en3 )/2 );
  ind++;
  for(int ii = 0; ii < n2; ii++)
  {
    allnodes[ind] = ( *( childs[3][ii] ) )->node(1-( 1-en3 )/2);
    ind++;
  }
  //  edges horizontaux
  ind = 0;
  for(int ii = 0; ii < n2; ii++)
  {
    alledges[ind] = childs[1][ii];
    ind++;
  }
  for(int jj = 0; jj < n1-1; jj++)
  {
    for(int ii = 0; ii < n2; ii++)
    {
      alledges[ind] = newedges[jj*n2+ii];
      ind++;
    }
  }
  for(int ii = 0; ii < n2; ii++)
  {
    alledges[ind] = childs[3][ii];
    ind++;
  }

  //nombre de edges horizontaux
  int neh = n2*( n1-1 );
  // ensuite les edges verticaux
  for(int ii = 0; ii < n1; ii++)
  {
    alledges[ind] = childs[0][ii];
    ind++;
  }
  for(int jj = 0; jj < n2-1; jj++)
  {
    for(int ii = 0; ii < n1; ii++)
    {
      alledges[ind] = newedges[jj*n1+ii+neh];
      ind++;
    }
  }

  for(int ii = 0; ii < n1; ii++)
  {
    alledges[ind] = childs[2][ii];
    ind++;
  }

  ind = 0;
  neh += 2*n2;

  for(int ii = 0; ii < n1; ii++)
  {
    for(int jj = 0; jj < n2; jj++)
    {
      FaceInterface* fn = new Quad;
      fn->edge(1) = alledges[jj+ii*n2];
      fn->node(1) = allnodes[ii*( n2+1 )+jj];
      fn->edge(2) = alledges[neh+( jj+1 )*n1+ii];
      fn->node(2) = allnodes[ii*( n2+1 )+jj+1];
      fn->edge(3) = alledges[jj+n2*( ii+1 )];
      fn->node(3) = allnodes[( ii+1 )*( n2+1 )+jj+1];
      fn->edge(0) = alledges[neh+jj*n1+ii];
      fn->node(0) = allnodes[( ii+1 )*( n2+1 )+jj];

      newfaces[ind] = fn;
      ind++;
    }
  }

  return 1;
}

