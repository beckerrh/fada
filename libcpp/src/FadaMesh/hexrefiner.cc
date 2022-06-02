#include "FadaMesh/hexrefiner.h"
#include "FadaMesh/faceinterface.h"
#include "FadaMesh/volumeinterface.h"
#include "FadaMesh/edge.h"
#include "FadaMesh/quad.h"
#include "FadaMesh/hex.h"
#include "FadaMesh/hexrefiner.h"
#include "Alat/intvector.h"
#include <cmath>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
void HexRefiner::_createInternalSubface(VolumeInterface* v, int ind, int if0, int ind_local_subface0,
                                        int if1, int ind_local_subface1,
                                        tree<FaceInterface*>& _Faces,
                                        int& facecount,
                                        const std::vector<Node*> centernodes)
{
  FaceInterface * f= new Quad;
  const face_pointer subf_0= _Faces.child(v->face(if0),ind_local_subface0);
  const face_pointer subf_1= _Faces.child(v->face(if1),ind_local_subface1);
  f->node(0)= centernodes[if0];
  f->node(2)= centernodes[if1];
  f->node(3)= newnodes[0];
  int id0=centernodes[if0]->id();
  int id1=centernodes[if1]->id();
  int id0bis;
  bool edges_found=false;
  for (int i=0; i<4; i++)
  {
    edge_pointer ei=(*subf_0)->edge(i);
    if (((*ei)->node(0)->id()==id0 ) |(((*ei)->node(1)->id()==id0)))
    {
      if ((*ei)->node(0)->id()==id0)
      {
        id0bis=(*ei)->node(1)->id();
      }
      else
      {
        id0bis=(*ei)->node(0)->id();
      }
      for (int j=0; j<4; j++)
      {
        edge_pointer ej=(*subf_1)->edge(j);
        if ((((*ej)->node(0)->id()==id1)&&((*ej)->node(1)->id()==id0bis))|(((*ej)->node(1)->id()==id1)&&((*ej)->node(0)->id()==id0bis)))
        {
          edges_found=true;
          f->edge(1)=ej;
          break;
        }
      }
      if (edges_found)
      {
        f->edge(0)=ei;
        if ((*ei)->node(0)->id()==id0)
        {
         f->node(1)=(*ei)->node(1);
        }
        else
        {
          f->node(1)=(*ei)->node(0);
        }
      }
    }
    if (edges_found) break;
  }
assert(edges_found);
f->edge(2)=newedges[if1];
f->edge(3)=newedges[if0];
newfaces[ind] = _Faces.insert(_Faces.end(), f);
(*newfaces[ind])->id()=++facecount;

}
/*--------------------------------------------------------------------------*/
void HexRefiner::reInit()
{
  newnodes.reserve(1); newnodes.resize(1);
  newedges.reserve(6); newedges.resize(6);
  newfaces.reserve(12); newfaces.resize(12);
  newvolumes.reserve(8); newvolumes.resize(8);
}
/*--------------------------------------------------------------------------*/

void HexRefiner::_createSubVolume(int ind, const face_pointer& face0,const face_pointer& face5, const Alat::FixArray<4,face_pointer> lateralfaces)
{
  VolumeInterface* vn= new Hex;

  Alat::FixArray<4, int> vnodeoffset5;
  Alat::FixArray<4, edge_pointer> lateral_edges;
  Alat::FixArray<4, int> lateralfacesoffset;
  _findSubVolumeOffset(face0,face5,lateralfaces,vnodeoffset5,lateral_edges,lateralfacesoffset);
  vn->face(0)=face0;vn->face(5)=face5;
  vn->face(1)=lateralfaces[lateralfacesoffset[0]];
  vn->face(3)=lateralfaces[lateralfacesoffset[1]];
  vn->face(4)=lateralfaces[lateralfacesoffset[2]];
  vn->face(2)=lateralfaces[lateralfacesoffset[3]];
  for (int in=0; in<4; in++)
  {
    vn->node(in)=(*face0)->node(in);
    vn->node(in+4)=(*face5)->node(vnodeoffset5[in]);
  }
  for (int ie=0; ie<4; ie++)
  {
    vn->edge(ie)=(*face0)->edge(ie);
    vn->edge(ie+4)=lateral_edges[ie];
    vn->edge(ie+8)=(*face5)->edge(vnodeoffset5[ie]);
  }
  newvolumes[ind]=vn;
}
/*--------------------------------------------------------------------------*/
void HexRefiner::_findSubVolumeOffset(const face_pointer& face0,const face_pointer& face5,const Alat::FixArray<4,face_pointer> lateralfaces,Alat::FixArray<4, int>& vnodeoffset5,Alat::FixArray<4, edge_pointer>& lateral_edges,Alat::FixArray<4, int>& lateralfacesoffset)const
{
  int nedges_found=0;

  // faces laterales
  bool found;
  for (int ie=0; ie<4; ie ++)
  {
    int id_node0_face0=(*face0)->node(ie)->id();
    int id_node1_face0=(*face0)->node((ie+1)%4)->id();
    found=false;
    for (int ifl=0; ifl<4; ifl++)
    {
      const face_pointer facelateral=lateralfaces[ifl];

      for (int k=0; k<4; k++)
      {
        int id_node0_edgek_face1=(*((*facelateral)->edge(k)))->node(0)->id();
        int id_node1_edgek_face1=(*((*facelateral)->edge(k)))->node(1)->id();
        if ((id_node0_edgek_face1==id_node0_face0 && id_node1_edgek_face1==id_node1_face0)|(id_node0_edgek_face1==id_node1_face0 && id_node1_edgek_face1==id_node0_face0))
        {
          found=true;
          lateralfacesoffset[ie]=ifl;
//          for (int iverif=0; iverif<ie; iverif++) if (lateralfacesoffset[iverif]==ifl) assert(0);
        }
        if (found) break;
      }
      if (found) break;
    }
    if (!found) assert(0);
  }
  const face_pointer face1=lateralfaces[lateralfacesoffset[0]];
  for (int i=0; i<2; i++)
  {
    int id_node_face0=(*face0)->node(i)->id();
    found=false;
    for (int k=0; k<4; k++)
    {
      int id_node0_edgek_face1=(*((*face1)->edge(k)))->node(0)->id();
      int id_node1_edgek_face1=(*((*face1)->edge(k)))->node(1)->id();
      if ((id_node0_edgek_face1==id_node_face0 | id_node1_edgek_face1==id_node_face0))
      {
        for (int j=0; j<4; j++)
        {
          int id_node_face5=(*face5)->node(j)->id();
          if ((id_node0_edgek_face1==id_node_face5 | id_node1_edgek_face1==id_node_face5))
          {
            vnodeoffset5[i]=j;
            lateral_edges[i]=(*face1)->edge(k);
            nedges_found++;
            found=true;
          }
          if (found) break;
        }
      }
      if (found) break;
    }
  }
  assert(nedges_found==2);
  nedges_found=0;

  const face_pointer face4=lateralfaces[lateralfacesoffset[2]];
  for (int i=2; i<4; i++)
  {
    int id_node_face0=(*face0)->node(i)->id();
    found=false;
    for (int k=0; k<4; k++)
    {
      int id_node0_edgek_face4=(*((*face4)->edge(k)))->node(0)->id();
      int id_node1_edgek_face4=(*((*face4)->edge(k)))->node(1)->id();
      if ((id_node0_edgek_face4==id_node_face0 | id_node1_edgek_face4==id_node_face0))
      {
        for (int j=0; j<4; j++)
        {
          int id_node_face5=(*face5)->node(j)->id();
          if ((id_node0_edgek_face4==id_node_face5 | id_node1_edgek_face4==id_node_face5))
          {
            vnodeoffset5[i]=j;
            lateral_edges[i]=(*face4)->edge(k);
            nedges_found++;
            found=true;
          }
          if (found) break;
        }
      }
      if (found) break;
    }
  }
  assert(nedges_found==2);
}

/*--------------------------------------------------------------------------*/
void HexRefiner::_findFaceOrientationInVolume(VolumeInterface* v)
{
  Alat::FixArray<4, int> subface_node_map;
  subface_node_map[0]=2;subface_node_map[1]=0;
  subface_node_map[2]=1;subface_node_map[3]=3;
  for (int ii=0; ii<6; ii++)
  {
    for (int i=0; i<4;i++)
    {
      int idvnode=v->node(hexnumbering.getLocalNodeIndiceOfSide(i,ii))->id();
      int j;
      for (j=0; j<4; j++)
      {
        if((*v->face(ii))->node(j)->id()==idvnode) break;
      }
      assert(j<4);
      vfaceoffset[ii][i]=subface_node_map[j];
    }
  }
}



/*--------------------------------------------------------------------------*/

bool HexRefiner::refine(VolumeInterface* v, tree<FaceInterface*>& _Faces, int& facecount, tree<Edge*>& _Edges,int& edgecount)
{
  assert (v->NumberOfFaces()==6);
  // détermination de l'offset de v
  _findFaceOrientationInVolume(v);
  std::vector<Node*> centernodes(6);
  for (int iface=0; iface<6; iface++)
  {
    const face_pointer fp=_Faces.child(v->face(iface),0);
    centernodes[iface]=(*fp)->node(3);
  }
  // stockage des noeuds milieux des 12 edges
  Alat::FixArray<12, Node*> nodeofedge;
  for (int ie=0;ie<12;ie++)
  {
    const edge_pointer ep=_Edges.child(v->edge(ie),0);
    nodeofedge[ie]=(*ep)->node(1);
  }
  reInit();
  // Nouveau noeud au centre
  Node* N=new Node;
  N->getNode().zeros();
  for(int i=0; i<8;i++)
  {
    // N->getNode().add(0.125,(*(v->node(i))).getNode());
    N->getNode() += (*(v->node(i))).getNode();
  }
  newnodes[0]=N;
  // les six edges internes
  for(int iface=0;iface<6;iface++)
  {
    Edge* ne= new Edge;
    ne->node(0)=centernodes[iface];
    ne->node(1)=N;
    newedges[iface]=_Edges.insert(_Edges.end(),ne);
    (*newedges[iface])->id()=++edgecount;
  }
  // les 12 nouvelles faces internes
  int ind=0;

  //les 4 orthogonales à Oz (0-5)
  {
    _createInternalSubface(v,ind,1,vfaceoffset[1][3],3,vfaceoffset[3][0],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,3,vfaceoffset[3][3],4,vfaceoffset[4][1],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,4,vfaceoffset[4][0],2,vfaceoffset[2][1],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,2,vfaceoffset[2][0],1,vfaceoffset[1][0],_Faces,facecount,centernodes);ind++;
  }
  //les 4 orthogonales à Oy (1-4)
  {
    _createInternalSubface(v,ind,0,vfaceoffset[0][1],3,vfaceoffset[3][0],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,3,vfaceoffset[3][1],5,vfaceoffset[5][3],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,5,vfaceoffset[5][0],2,vfaceoffset[2][3],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,2,vfaceoffset[2][0],0,vfaceoffset[0][0],_Faces,facecount,centernodes);ind++;
  }
  //les 4 orthogonales à Ox (2-3)
  {
    _createInternalSubface(v,ind,0,vfaceoffset[0][3],4,vfaceoffset[4][0],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,4,vfaceoffset[4][3],5,vfaceoffset[5][1],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,5,vfaceoffset[5][3],1,vfaceoffset[1][2],_Faces,facecount,centernodes);ind++;
    _createInternalSubface(v,ind,1,vfaceoffset[1][3],0,vfaceoffset[0][1],_Faces,facecount,centernodes);ind++;
  }

  // les 8 nouveaux hexaèdres
  ind=0;
  Alat::FixArray<4,face_pointer> lateralfaces;
  lateralfaces[0]=subFace(v,_Faces,1,0); lateralfaces[1]=newfaces[7];
  lateralfaces[2]=subFace(v,_Faces,2,0); lateralfaces[3]=newfaces[11];
  _createSubVolume(ind,subFace(v,_Faces,0,0),newfaces[3],lateralfaces);
  ind++;
  lateralfaces[0]=subFace(v,_Faces,1,3);lateralfaces[1]=newfaces[4];
  lateralfaces[2]=newfaces[11]; lateralfaces[3]=subFace(v,_Faces,3,0);
  _createSubVolume(ind,subFace(v,_Faces,0,1),newfaces[0],lateralfaces);
  ind++;
  lateralfaces[0]=newfaces[7]; lateralfaces[1]=subFace(v,_Faces,4,0);
  lateralfaces[2]=subFace(v,_Faces,2,1); lateralfaces[3]=newfaces[8];
  _createSubVolume(ind,subFace(v,_Faces,0,3),newfaces[2],lateralfaces);
  ind++;
  lateralfaces[0]=newfaces[4]; lateralfaces[1]=subFace(v,_Faces,4,1);
  lateralfaces[2]=newfaces[8]; lateralfaces[3]=subFace(v,_Faces,3,3);
  _createSubVolume(ind,subFace(v,_Faces,0,2),newfaces[1],lateralfaces);
  ind++;
  lateralfaces[0]=subFace(v,_Faces,2,3); lateralfaces[1]=newfaces[10];
  lateralfaces[2]=subFace(v,_Faces,1,1); lateralfaces[3]=newfaces[6];
  _createSubVolume(ind,newfaces[3],subFace(v,_Faces,5,0),lateralfaces);
  ind++;
  lateralfaces[0]=newfaces[10]; lateralfaces[1]=subFace(v,_Faces,3,1);
  lateralfaces[2]=subFace(v,_Faces,1,2); lateralfaces[3]=newfaces[5];
  _createSubVolume(ind,newfaces[0],subFace(v,_Faces,5,3),lateralfaces);
  ind++;
  lateralfaces[0]=subFace(v,_Faces,2,2); lateralfaces[1]=newfaces[9];
  lateralfaces[2]=newfaces[6]; lateralfaces[3]=subFace(v,_Faces,4,3);
  _createSubVolume(ind,newfaces[2],subFace(v,_Faces,5,1),lateralfaces);
  ind++;
  lateralfaces[0]=newfaces[9]; lateralfaces[1]=subFace(v,_Faces,3,2);
  lateralfaces[2]=newfaces[5]; lateralfaces[3]=subFace(v,_Faces,4,2);
  _createSubVolume(ind,newfaces[1],subFace(v,_Faces,5,2),lateralfaces);
  return 1;
}
/*--------------------------------------------------------------------------*/
face_pointer  HexRefiner::subFace(VolumeInterface* v, tree<FaceInterface*>& _Faces, int iface, int isubface) const
{
  return _Faces.child(v->face(iface),vfaceoffset[iface][isubface]);
}
