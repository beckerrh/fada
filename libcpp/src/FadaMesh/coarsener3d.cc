#include "FadaMesh/basicadaptivemesh3d.h"
#include "FadaMesh/coarsener3d.h"
#include  <limits>

using namespace FadaMesh;
using namespace std;


/*--------------------------------------------------------------------------*/
void Coarsener3d::adaptMesh(std::string filename)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void Coarsener3d::globalCoarsen()
{
  for(volume_pointer p = _getVolumes().begin(); p != _getVolumes().end(); p++)
  {
    ( *p )->to_coarsen() = 0;
  }
  for(volume_leafpointer v = _getVolumes().begin_leaf(); v !=_getVolumes().end_leaf(); v++)
  {
    if( _getVolumes().depth(v) )
    {
      _markCellToCoarsen(v);
    }
  }
  _makeRegular();
   _coarsen();
   // _updateCopyEdge();
   // _updateIds();
   _getMesh()->constructNumbering();
   _getMesh()->constructCellMap();
   _getMesh()->updateHangingInfo();
   _constructAdaptInfo();
   _getMesh()->reInitFadaMesh();
}

/*--------------------------------------------------------------------------*/
void Coarsener3d::_coarsen()
{
  _getCellMapOk() = false;
  _getNumberingOk() = false;

  _volumecoarse.clear();
  _facecoarse.clear();
  _edgecoarse.clear();
  _nodecoarse.clear();


  //delete volumes and store infos
  VolumeSet VolumesToDelete;
  for(volume_leafpointer v = _getVolumes().begin_leaf(); v != _getVolumes().end_leaf(); v++)
  {
    VolumeInterface* V = *v;
    int id = V->id();
    if( V->to_coarsen() )
    {
      volume_pointer vp = _getVolumes().parent(v);
      _volumecoarse[vp].push_back(getVolumeId2Id()[id]);
      for(int ii = 0; ii < V->NumberOfNodes(); ii++)
      {
        int nodeid = _getNodeId2Id()[V->node(ii)->id()];
        if( std::find(_nodecoarse[vp].begin(), _nodecoarse[vp].end(), nodeid) == _nodecoarse[vp].end() )
        {
          _nodecoarse[vp].push_back(nodeid);
        }
      }
      for(int ii = 0; ii < V->NumberOfEdges(); ii++)
      {
        int edgeid = _getEdgeId2Id()[( *V->edge(ii) )->id()];
        if( std::find(_edgecoarse[vp].begin(), _edgecoarse[vp].end(), edgeid) == _edgecoarse[vp].end() )
        {
          _edgecoarse[vp].push_back(edgeid);
        }
      }
      for(int ii = 0; ii < V->NumberOfFaces(); ii++)
      {
        int faceid = _getFaceId2Id()[( *V->face(ii) )->id()];
        if( std::find(_facecoarse[vp].begin(), _facecoarse[vp].end(), faceid) == _facecoarse[vp].end() )
        {
          _facecoarse[vp].push_back(faceid);
        }
      }

      // delete volume
      for(int ii = 0; ii < V->NumberOfFaces(); ii++)
      {
        face_pointer f = V->face(ii);
        FaceInterface* F= *f;
        for(int iii = 0; iii < 2; iii++)
        {
          if( ( F->volume(iii) != NULL ) && ( F->volume(iii) == v ) )
          {
            F->volume(iii) = NULL;
          }
        }
        for(int ii = 0; ii < F->NumberOfEdges(); ii++)
        {
          Edge* E = *F->edge(ii);
          for(int iii = 0; iii < 2; iii++)
          {
            if( ( E->face(iii) != NULL ) && ( E->face(iii) == f ) )
            {
              E->face(iii) = NULL;
            }
          }
        }
      }
      VolumesToDelete.insert(v);
    }
    else
    {
      _volumecoarse[v].push_back(getVolumeId2Id()[id]);

      for(int ii = 0; ii < V->NumberOfNodes(); ii++)
      {
        _nodecoarse[v].push_back( _getNodeId2Id()[V->node(ii)->id()] );
      }
      for(int ii = 0; ii < V->NumberOfEdges(); ii++)
      {
        _edgecoarse[v].push_back(_getEdgeId2Id()[( *V->edge(ii) )->id()]);
      }
      for(int ii = 0; ii < V->NumberOfFaces(); ii++)
      {
        _facecoarse[v].push_back(_getFaceId2Id()[( *V->face(ii) )->id()]);
      }
    }
  }
  for(VolumeSet::const_iterator p = VolumesToDelete.begin(); p != VolumesToDelete.end(); p++)
  {
    _getVolumes().erase(*p);
  }

  //delete faces and pointers of volumes to be deleted
  FaceSet FacesToDelete;
  for(face_leafpointer p = _getFaces().begin_leaf(); p != _getFaces().end_leaf(); p++)
  {
    FaceInterface* F = *p;
    if( ( F->volume(0) == NULL ) && ( F->volume(1) == NULL ) )
    {
      FacesToDelete.insert(p);
    }
  }
  for(FaceSet::const_iterator ef = FacesToDelete.begin(); ef !=FacesToDelete.end(); ef++)
  {
    _getFaces().erase(*ef);
  }

  //delete edges and pointers of volumes to be deleted

  EdgeSet EdgesToDelete;
  for(edge_leafpointer p = _getEdges().begin_leaf(); p != _getEdges().end_leaf(); p++)
  {
//    Edge* E = *p;
//    bool del=1;
//    for (int ii=0; ii<E->nfaces;ii++)
//    {
//      if( ( E->face(ii) != NULL ) )
//      {
//          del=0;
//      }
//    }
//    if (del) EdgesToDelete.insert(p);

    EdgesToDelete.insert(p);
  }
  for(EdgeSet::const_iterator ep = EdgesToDelete.begin(); ep != EdgesToDelete.end(); ep++)
  {
    _getEdges().erase(*ep);
  }
  //delete nodes
  NodeSet NodesToKeep;
  for(volume_leafpointer v = _getVolumes().begin_leaf(); v != _getVolumes().end_leaf(); v++)
  {
    VolumeInterface* V = *v;
    for(int ii = 0; ii < V->NumberOfNodes(); ii++)
    {
      NodesToKeep.insert( V->node(ii) );
      assert( _getNodes().find( V->node(ii) ) != _getNodes().end() );
    }
  }
  NodeSet NodesToDelete;
  for(NodeSet::const_iterator p = _getNodes().begin(); p != _getNodes().end(); p++)
  {
    if( NodesToKeep.find(*p) == NodesToKeep.end() )
    {
      NodesToDelete.insert(*p);
    }
  }
  for(NodeSet::const_iterator p = NodesToDelete.begin(); p != NodesToDelete.end(); p++)
  {
    _getNodes().erase(*p);
  }
  assert( _getNodes().size() == NodesToKeep.size() );

}

/*--------------------------------------------------------------------------*/
void Coarsener3d::_constructAdaptInfo()
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

void Coarsener3d::_markCellToCoarsen(volume_pointer v)
{
  ( *v )->to_coarsen() = 1;
}

/*--------------------------------------------------------------------------*/

void Coarsener3d::_makeRegular()
{
  assert(0);
}
