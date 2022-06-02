#include  "FadaMesh/trirefiner.h"
#include  "FadaMesh/faceinterface.h"
#include  "FadaMesh/edge.h"
#include  "FadaMesh/node.h"

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

bool TriRefiner::refine(FaceInterface* f,tree<Edge*>& _Edges, int& edgecount)
{

// les trois edges du triangle
  Edge& e0=*(*(f->edge(0)));      
  Edge& e1=*(*(f->edge(1))); 
  Edge& e2=*(*(f->edge(2)));  
// nombre de découpage de chaque edge du triangle       
  int n0=e0.nref,n1=e1.nref,n2=e2.nref;

// l'un au moins de des edges n'est pas raffiné, on sort
  if(std::min(std::min(n0,n1),n2)<2) return 0;

// On vérifie que l'on ne découpe pas en plus de deux  
  assert(std::max(std::max(n0,n1),n2)==2);
  
//les entiers ci-dessous permettent de connaitre le sens de parcours 
// des trois edges : 
// 1: du noeud 0 vers le noeud 1
// -1: du noeud 1 vers le noeud 0   
  int  en0=1, en1=1, en2=1;
//On vérifie que e0 et e1 ont un noeud en commun   
  std::pair<bool, std::pair<int,int> > inter_e0_e1=e0.Intercept(e1);
  assert(inter_e0_e1.first);
  
// on initialise le sens de parcours de e0 et e1 en fonction  
// des indices locaux du noeuds commun à e0 et e1 
  if(inter_e0_e1.second.first==1) en0=-1;
  if(inter_e0_e1.second.second==1) en1=-1; 

// On vérifie que e0 et e2 ont un noeud en commun  
  std::pair<bool, std::pair<int,int> > inter_e0_e2=e0.Intercept(e2);
  assert(inter_e0_e2.first);
  
// on initialise le sens de parcours de e2  en fonction  
// de l'indices local du noeud commun à e0 et e1  
  if(inter_e0_e2.second.second==1) en2=-1;

// on dimensionne les vecteurs pour les nouveaux edges et les 
// nouvelles faces. Il s'agit ici des edges internes puisque les edges
// du contour sont créés par la classe EdgeRefiner
  reInit();

// afin de pouvoir définir les nouvelles faces et leur affecter
// les edges qui forment leur contour, on définit le vecteur 
// childs pour y mettre les sous-edges
  std::vector< std::vector<edge_pointer> > childs(3);
  childs[0].reserve(2);
  childs[1].reserve(2);
  childs[2].reserve(2);
  for(int ii=0; ii<2;ii++)
  {
// on utilise le sens de parcours des edges 
// pour numéroter correctement les deux sous-edges  
    if(en0==1)
      childs[0][ii]=_Edges.child(f->edge(0),ii);
    else
      childs[0][ii]=_Edges.child(f->edge(0),1-ii); 
    if(en1==1) 
      childs[1][ii]=_Edges.child(f->edge(1),ii);
    else 
      childs[1][ii]=_Edges.child(f->edge(1),1-ii);  
    if(en2==1)
      childs[2][ii]=_Edges.child(f->edge(2),ii);
    else
      childs[2][ii]=_Edges.child(f->edge(2),1-ii);
  }


// On crée dans cette partie les trois edges intérieurs

// le premier edge est celui qui relie le milieu du coté 0 à celui du coté 1
// On oriente l'edge de manière à aller du noeud ayant le plus petit identifiant
// vers l'autre
  Edge* ne0= new Edge;
  if(((*childs[0][0])->node(1-(1-en0)/2))->id()< ((*childs[1][0])->node(1-(1-en1)/2))->id()) 
  {
    ne0->node(0)=(*childs[0][0])->node(1-(1-en0)/2);
    ne0->node(1)=(*childs[1][0])->node(1-(1-en1)/2);
  } 
  else 
  {
    ne0->node(1)=(*childs[0][0])->node(1-(1-en0)/2);
    ne0->node(0)=(*childs[1][0])->node(1-(1-en1)/2); 
  }
  
// On insére le nouvel edge dans l'arbre des edges et on range le pointeur
// associé dans le vecteur newedges  
  newedges[0]=_Edges.insert(_Edges.end(),ne0);
// On affecte à ce nouvel edge un identifiant et on met à jour le compteur de edge   
  //(*newedges[0])->id()=edgecount++;
  (*newedges[0])->id()=++edgecount;

// On fait de même pour les deux autres edges à créer

  Edge* ne1= new Edge;
  if(((*childs[1][0])->node(1-(1-en1)/2))->id()<((*childs[2][0])->node(1-(1-en2)/2))->id())
  {
    ne1->node(0)=(*childs[1][0])->node(1-(1-en1)/2);
    ne1->node(1)=(*childs[2][0])->node(1-(1-en2)/2);
  }
  else
  {
    ne1->node(1)=(*childs[1][0])->node(1-(1-en1)/2);
    ne1->node(0)=(*childs[2][0])->node(1-(1-en2)/2);
  }
  
  
  newedges[1]=_Edges.insert(_Edges.end(),ne1);
  (*newedges[1])->id()=++edgecount;


  Edge* ne2= new Edge;
  if(((*childs[2][0])->node(1-(1-en2)/2))->id()<((*childs[0][0])->node(1-(1-en0)/2))->id())
  {
    ne2->node(0)=(*childs[2][0])->node(1-(1-en2)/2);
    ne2->node(1)=(*childs[0][0])->node(1-(1-en0)/2);
  }
  else
  {
    ne2->node(1)=(*childs[2][0])->node(1-(1-en2)/2);
    ne2->node(0)=(*childs[0][0])->node(1-(1-en0)/2);
  }  
  newedges[2]=_Edges.insert(_Edges.end(),ne2);
  (*newedges[2])->id()=++edgecount;


// On crée dans cette partie les quatre sous-faces 
  
         // face 0
  FaceInterface* f0= new Tri;

// on affecte à la face créée les trois edges et 
// les trois nodes correspondants  
  f0->edge(0)= childs[0][0];
  f0->node(0)=(*childs[1][0])->node(1-(1-en1)/2);

  f0->edge(1)= childs[1][0];
  f0->node(1)=(*childs[0][0])->node(1-(1-en0)/2);

  f0->edge(2)= newedges[0];
  f0->node(2)=(*childs[0][0])->node((1-en0)/2);
  
  
  newfaces[0]=f0;
  
        // face 1
  FaceInterface* f1= new Tri;
  f1->edge(0)= childs[1][1];
  f1->node(0)=(*childs[2][0])->node(1-(1-en2)/2);

  f1->edge(1)= childs[2][1];
  f1->node(1)=(*childs[1][0])->node(1-(1-en1)/2);

  f1->edge(2)= newedges[1];
  f1->node(2)=(*childs[1][1])->node(1-(1-en1)/2);
  
  newfaces[1]=f1;
  
         // face 2
  FaceInterface* f2= new Tri;
  f2->edge(0)= childs[0][1];
  f2->node(0)=(*childs[2][0])->node(1-(1-en2)/2);

  f2->edge(2)= childs[2][0];
  f2->node(2)=(*childs[0][0])->node(1-(1-en0)/2);

  f2->edge(1)= newedges[2];
  f2->node(1)=(*childs[0][1])->node(1-(1-en0)/2);
  
  
  newfaces[2]=f2;
  
         // face 3
  FaceInterface* f3= new Tri;
  f3->edge(0)= newedges[0];
  f3->node(0)=(*childs[2][0])->node(1-(1-en2)/2);

  f3->edge(1)= newedges[1];
  f3->node(1)=(*childs[0][0])->node(1-(1-en0)/2);

  f3->edge(2)= newedges[2];
  f3->node(2)=(*childs[1][0])->node(1-(1-en1)/2);
  
  newfaces[3]=f3;

  return 1;  
}   
