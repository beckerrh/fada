#ifndef __FadaMesh_SetDefs_h
#define __FadaMesh_SetDefs_h

#include  "typedefs.h"
#include  "volumeinterface.h"
#include  "node.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  struct NodeCompare
    {
      bool operator()(const Node* N, const Node* M) const
      {
        return N->id() < M->id();
      }
    };
  struct EdgeCompare
  {
    bool operator()(edge_pointer p, edge_pointer q) const
    {
      return ( * p )->id() < ( * q )->id();
    }
  };
  struct FaceCompare
    {
      bool operator()(face_pointer p, face_pointer q) const
      {
        return ( * p )->id() < ( * q )->id();
      }
    };
  struct VolumeCompare
    {
      bool operator()(volume_pointer p, volume_pointer q) const
      {
        return ( * p )->id() < ( * q )->id();
      }
    };

  typedef std::set<Node*, NodeCompare>  NodeSet;
  typedef std::set<edge_pointer,EdgeCompare>  EdgeSet;
  typedef std::set<face_pointer, FaceCompare>  FaceSet;
  typedef std::set<volume_pointer, VolumeCompare>  VolumeSet;

}

/*--------------------------------------------------------------------------*/

#endif
