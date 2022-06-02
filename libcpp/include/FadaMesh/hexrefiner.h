#ifndef __FadaMesh_HexRefiner_h
#define __FadaMesh_HexRefiner_h

#include "hexahedron.h"
#include "faceinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{ 
class Quad;
class HexRefiner
{
private:
  FadaMesh::Hexahedron hexnumbering;
  std::vector<Node*> newnodes;
  std::vector<edge_pointer> newedges;
  std::vector<face_pointer> newfaces;
  std::vector<VolumeInterface*> newvolumes;
  Alat::FixArray<6, bool > faceorientation;
  Alat::FixArray<6, Alat::FixArray<4, int> > vfaceoffset;
  void _createInternalSubface(VolumeInterface* v, int ind, int if0, int ind_local_subface0,
                               int if1, int ind_local_subface1,  tree<FaceInterface*>& _Faces,
                              int& facecount,
                              const std::vector<Node*> centernodes);

  void _createSubVolume(int ind, const face_pointer& face0, const face_pointer& face5, const Alat::FixArray<4,face_pointer> lateralfaces);
  void _findFaceOrientationInVolume(VolumeInterface* v);
  void _findSubVolumeOffset(const face_pointer& face0,const face_pointer& face5,const Alat::FixArray<4,face_pointer> lateralfaces,Alat::FixArray<4, int>& vnodeoffset5,Alat::FixArray<4, edge_pointer>& lateral_edges,Alat::FixArray<4, int>& lateralfacesoffset)const;
  void _findSubFaceOffset();
 face_pointer  subFace(VolumeInterface* v, tree<FaceInterface*>& _Faces,int iface, int isubface) const;


public:
  HexRefiner(){}
  void reInit();
  std::vector<Node*>& getNewNodes(){return newnodes;}
  std::vector<edge_pointer>& getNewEdges(){return newedges;}
  std::vector<face_pointer>& getNewFaces(){return newfaces;}
  std::vector<VolumeInterface*>& getNewVolumes() {return newvolumes;}
  bool Coarse() {return 0;}
  bool refine(VolumeInterface* v, tree<FaceInterface *> &_Faces, int& facecount, tree<Edge*>& _Edges, int& edgecount);

};
}

/*--------------------------------------------------------------------------*/

#endif
