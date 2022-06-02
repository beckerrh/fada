#include "FadaMesh/basicadaptivemesh2d.h"
#include "FadaMesh/coarsener2d.h"
#include "FadaMesh/coarseninfo.h"
#include  <limits>

using namespace FadaMesh;
using namespace std;


/*--------------------------------------------------------------------------*/
void Coarsener2d::adaptMesh(std::string filename)
{
  Alat::IntVector marked_fadalightmesh_cells;
  std::ifstream file( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** ERROR Coarsener2d::adaptMesh() cannot open file \""<<filename<<"\"\n";
    exit(5);
  }
  marked_fadalightmesh_cells.loadFada(file);
  file.close();
  for(int i = 0; i < marked_fadalightmesh_cells.size(); i++)
  {
    _markCellToCoarsen(_getCellMap2d()[marked_fadalightmesh_cells[i]]);
  }
  _makeRegular();
  _coarsen();
  _updateCopyEdge();
  _updateIds();
  _getMesh()->constructNumbering();
  _getMesh()->constructCellMap();
  _getMesh()->updateHangingInfo();
  _constructAdaptInfo();
  _getMesh()->reInitFadaMesh();
  // _getMesh()->constructNumbering();
}

/*--------------------------------------------------------------------------*/
void Coarsener2d::globalCoarsen()
{
  for(face_pointer p = _getFaces().begin(); p != _getFaces().end(); p++)
  {
    ( *p )->to_coarsen() = 0;
  }
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    if( _getFaces().depth(f) )
    {
      _markCellToCoarsen(f);
    }
  }
  _makeRegular();
  _coarsen();
  // std::cerr << "After coarsen " << _getNodeId2Id() << "\n";
  _updateCopyEdge();
  _updateIds();
  // std::cerr << "After Bidouille " << _getNodeId2Id() << "\n";
  _getMesh()->constructNumbering();
  // std::cerr << "After constructNumbering " << _getNodeId2Id() << "\n";
  _getMesh()->constructCellMap();
  _getMesh()->updateHangingInfo();
  _constructAdaptInfo();
  _getMesh()->reInitFadaMesh();
  // std::cerr << "After reInitFadaMesh " << _getNodeId2Id() << "\n";
}
/*--------------------------------------------------------------------------*/
void Coarsener2d::_updateIds()
{
  for(NodeSet::iterator p = _getNodes().begin(); p != _getNodes().end(); p++)
  {
    Node* node = *p; 
    int oldid = node->id();
    node->id() = _getNodeId2Id()[oldid];
  }  
  // for(edge_leafpointer p = _getEdges().begin_leaf(); p != _getEdges().end_leaf(); p++)
  // {
  //   Edge* edge = *p;
  //   int oldid = edge->id();
  //   edge->id() = _getEdgeId2Id()[oldid];
  // }
  // // on numérote les feuilles de l'arbre des faces
  // int count_face = 0;
  // for(face_leafpointer p = _getFaces().begin_leaf(); p != _getFaces().end_leaf(); p++)
  // {
  //   FaceInterface* face = *p;
  //   int oldid = face->id();
  //   face->id() = _getFaceId2Id()[oldid];
  // }
}

/*--------------------------------------------------------------------------*/
void Coarsener2d::_updateCopyEdge()
{
  int ie = 0;
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    FaceInterface* F = *f;
    for(int ii = 0; ii < F->NumberOfEdges(); ii++)
    {
      edge_pointer E = F->edge(ii);
      const face_pointer N = _neighbour(f, E);
      if(N == NULL)
      {
        ( *E )->copy() = 0;
        continue;
      }
      int nchild = _getFaces().number_of_children(N);
      if(nchild == 0)
      {
        ( *E )->copy() = 0;
      }
      else
      {
        ( *E )->copy() = 1;
        ie++;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void Coarsener2d::_coarsen()
{
  _getCellMapOk() = false;
  _getNumberingOk() = false;
  _edgecoarse.clear();
  _facecoarse.clear();
  _nodecoarse.clear();
  //delete faces and store infos
  FaceSet FacesToDelete;
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    FaceInterface* F = *f;
    int id = F->id();
    if( F->to_coarsen() )
    {
      face_pointer fp = _getFaces().parent(f);
      _facecoarse[fp].push_back(_getFaceId2Id()[id]);
      for(int ii = 0; ii < F->NumberOfNodes(); ii++)
      {
        int nodeid = _getNodeId2Id()[F->node(ii)->id()];
        if( std::find(_nodecoarse[fp].begin(), _nodecoarse[fp].end(), nodeid) == _nodecoarse[fp].end() )
        {
          _nodecoarse[fp].push_back(nodeid);
        }
      }
      for(int ii = 0; ii < F->NumberOfEdges(); ii++)
      {
        int edgeid = _getEdgeId2Id()[( *F->edge(ii) )->id()];
        if( std::find(_edgecoarse[fp].begin(), _edgecoarse[fp].end(), edgeid) == _edgecoarse[fp].end() )
        {
          _edgecoarse[fp].push_back(edgeid);
        }
      }

      // delete face
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
      FacesToDelete.insert(f);
    }
    else
    {
      _facecoarse[f].push_back(_getFaceId2Id()[id]);
      for(int ii = 0; ii < F->NumberOfNodes(); ii++)
      {
        _nodecoarse[f].push_back( _getNodeId2Id()[F->node(ii)->id()] );
      }
      for(int ii = 0; ii < F->NumberOfEdges(); ii++)
      {
        _edgecoarse[f].push_back(_getEdgeId2Id()[( *F->edge(ii) )->id()]);
      }
    }
  }
  for(FaceSet::const_iterator p = FacesToDelete.begin(); p != FacesToDelete.end(); p++)
  {
    _getFaces().erase(*p);
  }
  //delete edges and pointers to faces to be deleted
  EdgeSet EdgesToDelete;
  for(edge_leafpointer p = _getEdges().begin_leaf(); p != _getEdges().end_leaf(); p++)
  {
    Edge* E = *p;
    if( ( E->face(0) == NULL ) && ( E->face(1) == NULL ) )
    {
      EdgesToDelete.insert(p);
    }
  }
  for(EdgeSet::const_iterator ep = EdgesToDelete.begin(); ep != EdgesToDelete.end(); ep++)
  {
    _getEdges().erase(*ep);
  }
  //delete nodes
  NodeSet NodesToKeep;
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    FaceInterface* F = *f;
    for(int ii = 0; ii < F->NumberOfNodes(); ii++)
    {
      NodesToKeep.insert( F->node(ii) );
      assert( _getNodes().find( F->node(ii) ) != _getNodes().end() );
    }
  }
  NodeSet NodesToKeep2;
  for(edge_leafpointer e = _getEdges().begin_leaf(); e != _getEdges().end_leaf(); e++)
  {
    Edge* E = *e;
    NodesToKeep2.insert( E->node(0) );
    NodesToKeep2.insert( E->node(1) );
  }
  if(NodesToKeep != NodesToKeep2)
  {
    NodeSet result;
    set_difference( NodesToKeep.begin(), NodesToKeep.end(), NodesToKeep2.begin(), NodesToKeep2.end(), std::insert_iterator<NodeSet>( result, result.end() ) );
    for(NodeSet::const_iterator p = result.begin(); p != result.end(); p++)
    {
      std::cerr<<( *p )->id()<<" ";
    }
    std::cerr<<"\n";
    set_difference( NodesToKeep2.begin(), NodesToKeep2.end(), NodesToKeep.begin(), NodesToKeep.end(), std::insert_iterator<NodeSet>( result, result.end() ) );
    for(NodeSet::const_iterator p = result.begin(); p != result.end(); p++)
    {
      std::cerr<<( *p )->id()<<" ";
    }
    std::cerr<<"\n";
    assert(0);
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
void Coarsener2d::_constructAdaptInfo()
{
  _getMesh()->createGeometryObject("CoarsenInfo");
  FadaMesh::CoarsenInfo* _coarseninfo = dynamic_cast<FadaMesh::CoarsenInfo*>( _getMesh()->getGeometryObject("CoarsenInfo") );
  Alat::SparsityPattern& oldnodes = _coarseninfo->getOldNodes();
  Alat::SparsityPattern& oldcells = _coarseninfo->getOldCells();
  Alat::SparsityPattern& oldsides = _coarseninfo->getOldSides();
  Alat::IntVector& nodeoldtonew =   _coarseninfo->getNodeNewToOld();
  // std::cerr << "@@@ Coarsener2d::_constructAdaptInfo() getNodeId2Id() =" << _getNodeId2Id() << "\n";
  {
    // nodes
    Alat::Vector<Alat::IntVector> SPS( _nodecoarse.size() );
    // for(std::map<face_pointer, Alat::IntVector>::const_iterator p = _nodecoarse.begin(); p != _nodecoarse.end(); p++)
      for(CoarseMap::const_iterator p = _nodecoarse.begin(); p != _nodecoarse.end(); p++)
    {
      int iK = _getFaceId2Id()[( *( p->first ) )->id()];
      SPS[iK].set_size(p->second.size());
      SPS[iK] = p->second;
      // for(std::vector<int>::const_iterator q = p->second.begin(); q != p->second.end(); q++)
      // {
      //   SPS[iK].push_back(*q);
      // }
    }
    oldnodes.set_size(SPS);
  }
  {
    // edges
    Alat::Vector<Alat::IntVector> SPS( _edgecoarse.size() );
    // for(std::map<face_pointer, Alat::IntVector>::const_iterator p = _edgecoarse.begin(); p != _edgecoarse.end(); p++)
      for(CoarseMap::const_iterator p = _edgecoarse.begin(); p != _edgecoarse.end(); p++)
    {
      int iK = _getFaceId2Id()[( *( p->first ) )->id()];
      SPS[iK].set_size(p->second.size());
      SPS[iK] = p->second;
      // for(std::vector<int>::const_iterator q = p->second.begin(); q != p->second.end(); q++)
      // {
      //   SPS[iK].push_back(*q);
      // }
    }
    oldsides.set_size(SPS);
  }
  {
    // cells
    Alat::Vector<Alat::IntVector> SPS( _facecoarse.size() );
    // for(std::map<face_pointer, Alat::IntVector>::const_iterator p = _facecoarse.begin(); p != _facecoarse.end(); p++)
      for(CoarseMap::const_iterator p = _facecoarse.begin(); p != _facecoarse.end(); p++)
    {
      int iK = _getFaceId2Id()[( *( p->first ) )->id()];
      SPS[iK].set_size(p->second.size());
      SPS[iK] = p->second;
      // for(std::vector<int>::const_iterator q = p->second.begin(); q != p->second.end(); q++)
      // {
      //   SPS[iK].push_back(*q);
      // }
    }
    oldcells.set_size(SPS);
  }
}

/*--------------------------------------------------------------------------*/

void Coarsener2d::_markCellToCoarsen(face_pointer f)
{
  ( *f )->to_coarsen() = 1;
}

/*--------------------------------------------------------------------------*/

void Coarsener2d::_makeRegular()
{
  for(face_pointer f = _getFaces().begin(); f != _getFaces().end(); f++)
  {
    int nchild = _getFaces().number_of_children(f);
    if(nchild)
    {
      ( *f )->to_coarsen() = 0;
    }
  }
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    int depth = _getFaces().depth(f);
    if(depth == 0)
    {
      ( *f )->to_coarsen() = 0;
    }
    face_pointer P = _getFaces().parent(f);
    if(P == NULL)
    {
      continue;
    }
    int nchild = _getFaces().number_of_children(P);
    bool allcoarsen = 1;
    for(int ii = 0; ii < nchild; ii++)
    {
      if( ( *_getFaces().child(P, ii) )->to_coarsen() == 0 )
      {
        allcoarsen = 0;
        break;
      }
    }
    if(!allcoarsen)
    {
      for(int ii = 0; ii < nchild; ii++)
      {
        ( *_getFaces().child(P, ii) )->to_coarsen() = 0;
      }
    }
  }
  std::map<int, FaceSet> _nodeid2cell;
  _nodeid2cell.clear();
  int coarsen = 0;
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    FaceInterface* F = *f;
    if( F->to_coarsen() )
    {
      coarsen++;
    }
    for(int ii = 0; ii < F->NumberOfNodes(); ii++)
    {
      _nodeid2cell[( F->node(ii) )->id()].insert(f);
    }
  }
  while(1)
  {
    int avoid_coarsen = 0;

    for(std::map<int, FaceSet>::iterator np = _nodeid2cell.begin(); np != _nodeid2cell.end(); np++)
    {
      int id = np->first;
      const FaceSet& faces_of_node = np->second;
      // int nlevelsignore = INT_MAX;
      // int maxlevel = INT_MIN;
      int nlevelsignore = numeric_limits<int>::max();
      int maxlevel = numeric_limits<int>::min();
      for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
      {
        int newdepth = _getFaces().depth(*q);
        if( ( *( *q ) )->to_refine() == 1 )
        {
          newdepth += 1;
        }
        nlevelsignore = std::min(nlevelsignore, newdepth);
        maxlevel = std::max(maxlevel, newdepth);
      }
      if(maxlevel > nlevelsignore+1)
      {
        for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
        {
          int newdepth = _getFaces().depth(*q);
          std::cerr<<"--- "<<newdepth<<" "<<( *( *q ) )->to_refine()<<std::endl;
        }
        assert(0);
      }
      else if(maxlevel == nlevelsignore+1)
      {
        bool all_finest_to_coarse = 1;
        for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
        {
          int newdepth = _getFaces().depth(*q);
          if(newdepth == maxlevel)
          {
            if( !( *( *q ) )->to_coarsen() )
            {
              all_finest_to_coarse = 0;
              break;
            }
          }
        }
        for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
        {
          int newdepth = _getFaces().depth(*q);
          if(newdepth == nlevelsignore)
          {
            if( ( !all_finest_to_coarse )  && ( ( *( *q ) )->to_coarsen() ) )
            {
              // go to parent and avoid coarsening of all childs
              face_pointer qp = _getFaces().parent(*q);
              assert(qp != NULL);
              for(int ii = 0; ii < _getFaces().number_of_children(qp); ii++)
              {
                ( *_getFaces().child(qp, ii) )->to_coarsen() = 0;
              }

              avoid_coarsen++;
            }
          }
        }
      }
    }
    if(avoid_coarsen == 0)
    {
      break;
    }
  }
}
