#include "FadaMesh/basicadaptivemesh3d.h"
#include "FadaMesh/refiner3d.h"
// #include "FadaMesh/quadtotri.h"
#include "FadaMesh/refineinfo.h"

using namespace FadaMesh;
using namespace std;


/*--------------------------------------------------------------------------*/
void Refiner3d::globalRefine(int nref)
{
  for(int i = 0; i < nref; i++)
  {
    for(volume_leafpointer v = _getVolumes().begin_leaf(); v != _getVolumes().end_leaf(); v++)
    {
      _markCellToRefine(v);
    }
    _refine();
    //    _updateCopyFaceAndEdge();
    _constructAdaptInfo();
    //    _getMesh()->updateHangingInfo();
    _getMesh()->reInitFadaMesh();
  }
}

/*--------------------------------------------------------------------------*/
//void Refiner3d::_updateCopyFaceAndEdge()
//{
//  assert(0);
//  int ie = 0;
//  for(volume_leafpointer v = _getVolumes().begin_leaf(); v != _getVolumes().end_leaf(); v++)
//  {
//    VolumeInterface* V = *v;
//    for(int ii = 0; ii < V->NumberOfFaces(); ii++)
//    {
//      face_pointer F = V->face(ii);
//      const volume_pointer N = _neighbour(v, F);
//      if(N == NULL)
//      {
//        if( ( *F )->Copy() )
//        {
//          std::cerr<<"updatCopy ref"<<( *F )->boundaryid()<<'\n';
//        }
//        ( *F )->Copy() = false;
//        continue;
//      }
//      int nchild = _getVolumes().number_of_children(N);
//      if(nchild == 0)
//      {
//        ( *F )->Copy() = false;
//      }
//      else
//      {
//        ( *F )->Copy() = true;
//        ie++;
//      }
//    }
//  }
//EDGES????
//}

/*--------------------------------------------------------------------------*/
void Refiner3d::adaptMesh(std::string filename)
{
  assert(0);
  //  Alat::IntVector marked_conchamesh_cells;
  //  std::ifstream file( filename.c_str() );
  //  if( !file.is_open() )
  //  {
  //    std::cerr<<"*** ERROR BasicRefiner3d::refine() cannot open file \""<<filename<<"\"\n";
  //    exit(5);
  //  }

  //  marked_conchamesh_cells.Read(file);
  //  file.close();

  //  for(int i = 0; i < marked_conchamesh_cells.size(); i++)
  //  {
  //    int imarked = marked_conchamesh_cells[i];
  //    volume_pointer f = _getCellMap()[imarked];
  //    if(not *f)
  //    {
  //      std::cerr << "*** ERROR in Refiner3d::adaptMesh(): volume_pointer NULL\n";
  //      assert(0);
  //      exit(1);
  //    }
  //    _markCellToRefine( *( *f ) );
  //  }
  //  _makeRegular();
  //  _refine();
  //  _updateCopyFaceAndEdge();
  //  _constructAdaptInfo();
  //  _getMesh()->updateHangingInfo();
  //  _getMesh()->reInitFadaMesh();

}


/*--------------------------------------------------------------------------*/
void Refiner3d::_refine()
{
  const FadaMesh::CurvedBoundaryInformation* curvedboundaryinformation = _getCurvedBoundaries();
  _getCellMapOk() = false;
  _getNumberingOk() = false;
  // put nodes on boundary
  if(curvedboundaryinformation)
  {
  for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
  {
    int color = ( *f )->boundaryid();
    if( curvedboundaryinformation->boundaryColorIsCurved(color) )
    {
      const FadaMesh::CurvedBoundaryDescriptionInterface* BDI =  curvedboundaryinformation->get(color);
      for(int ii = 0; ii < (*f)->NumberOfNodes(); ii++)
      {
        BDI->newton( ( *f )->node(ii)->getNode() );
      }
    }
  }
  }
  //edge refinement

  for(edge_leafpointer e = _getEdges().begin_leaf(); e != _getEdges().end_leaf(); e++)
  {
    if( !( ( *e )->to_refine() ) )
    {
      continue;
    }
    _edgerefiner.refine( e, _getLastNodeId(), _getLastEdgeId() );
    std::vector<Node*>& newnodes = _edgerefiner.getNewNodes();
    std::vector<Edge*>& newedges = _edgerefiner.getNewEdges();
    //    int color = ( *e )->boundaryid();
    //    if( _getCurvedBoundaries().IsCurved(color) )
    //    {
    //      const FadaMesh::CurvedBoundaryDescriptionInterface* BDI = _getCurvedBoundaries().get(color);
    //      for(int ii = 0; ii < newnodes.size(); ii++)
    //      {
    //        BDI->newton( newnodes[ii]->getNode() );
    //      }
    //    }
    for(std::vector<Node*>::iterator it = newnodes.begin(); it != newnodes.end(); it++)
    {
      _getNodes().insert(*it);
    }
    for(std::vector<Edge*>::iterator it = newedges.begin(); it != newedges.end(); it++)
    {
      _getEdges().append_child(e, *it);
    }
  }
  // face refinement
  {
    face_leafpointer it_begin = _getFaces().begin_leaf();
    face_leafpointer it_end = _getFaces().end_leaf();
    for(face_leafpointer f = it_begin; f != it_end; f++)
    {
      if( !( ( *f )->to_refine() ) )
      {
        continue;
      }
      if( !( _facerefiner->refine( *f, _getEdges(), _getLastEdgeId() ) ) )
      {
        continue;
      }
      std::vector<Node*>& newnodes = _facerefiner->getNewNodes();
      std::vector<FaceInterface*>& newfaces = _facerefiner->getNewFaces();

      for(std::vector<Node*>::iterator it = newnodes.begin(); it != newnodes.end(); it++)
      {
        ( *it )->id() = ++_getLastNodeId();
        _getNodes().insert(*it);
      }
      for(std::vector<FaceInterface*>::iterator it = newfaces.begin(); it != newfaces.end(); it++)
      {
        face_pointer newf = _getFaces().append_child(f, *it);
        (*newf)->boundaryid()=(*f)->boundaryid();
        ( *it )->id() = ++_getLastFaceId();
        ( *it )->depth() = ( *f )->depth()+1;
        for(int ii = 0; ii < ( *newf )->NumberOfEdges(); ii++)
        {
          edge_pointer enewf = ( *newf )->edge(ii);
          ( *enewf )->face( ( *enewf )->nfaces ) = newf;
          ( *enewf )->nfaces++;

        }

      }
    }
  }
  // volume refinement
  {
    //    volume_leafpointer it_begin = _getVolumes().begin_leaf();
    //    volume_leafpointer it_end = _getVolumes().end_leaf();
    //    for(volume_leafpointer v = it_begin; v != it_end; v++)
    for ( std::map<int, volume_pointer>::const_iterator it=_getCellMap3d().begin();it!=_getCellMap3d().end(); it++ )
    {
      volume_pointer v=(*it).second;
      if( !( ( *v )->to_refine() ) )
      {
        continue;
      }
      if( !( _volumerefiner->refine( *v, _getFaces(), _getLastFaceId(), _getEdges(), _getLastEdgeId() ) ) )
      {
        continue;
      }

      std::vector<VolumeInterface*>& newvolumes = _volumerefiner->getNewVolumes();

      std::vector<Node*>& newnodes = _volumerefiner->getNewNodes();
      for(std::vector<Node*>::iterator it = newnodes.begin(); it != newnodes.end(); it++)
      {
        ( *it )->id() = ++_getLastNodeId();
        _getNodes().insert(*it);
      }
      for(std::vector<VolumeInterface*>::iterator child_it = newvolumes.begin(); child_it != newvolumes.end(); child_it++)
      {
        volume_pointer newv = _getVolumes().append_child(v, *child_it);

        ( *newv )->id() = ++_getLastVolumeId();
        for(int ii = 0; ii < ( *newv )->NumberOfFaces(); ii++)
        {
          face_pointer fnewv = ( *newv )->face(ii);
          ( *fnewv )->volume( ( *fnewv )->nvolumes ) = newv;
          ( *fnewv )->nvolumes++;
        }
      }
      //      std::cerr<<"------------------------------------------------------------------------------------"<<'\n';
      //        for(std::vector<VolumeInterface*>::iterator it = newvolumes.begin(); it != newvolumes.end(); it++)
      //        {
      //          VolumeInterface* newv= *it;
      //          std::cerr<<"SOUS-Volume: "<<(newv)->id()<<'\n';
      //          std::cerr<<"  Nodes: ";
      //          for(int ii = 0; ii < 8; ii++)
      //          {
      //            std::cerr<<'\t'<<(( newv )->node(ii))->id()<<" "<<(( newv )->node(ii))->x()<<
      //                       " "<<(( newv )->node(ii))->y()<<"  "<<(( newv )->node(ii))->z()<<'\n';
      //          }
      //  //        std::cerr<<'\n';
      //  //        std::cerr<<"  Edges : ";
      //  //        for(int ii = 0; ii < 12; ii++)
      //  //        {
      //  //          std::cerr<<'\t'<<(*(( *newv )->edge(ii)))->id()<<" ";
      //  //        }
      //  //        std::cerr<<'\n';
      //  //        std::cerr<<"  Faces : ";
      //  //        for(int ii = 0; ii < 6; ii++)
      //  //        {
      //  //          std::cerr<<'\t'<<(*(( *newv )->face(ii)))->id()<<" ";
      //  //        }
      //  //        std::cerr<<'\n';
      //        }
      //        std::cerr<<"------------------------------------------------------------------------------------"<<'\n';

    }
  }

}

/*--------------------------------------------------------------------------*/
void Refiner3d::_constructAdaptInfo()
{
  _getMesh()->createGeometryObject("RefineInfo");
  _getMesh()->constructNumbering();
  int n = _getCellMap3d().size();
  Alat::Vector<Alat::IntVector> SPC(n), SPF(n), SPN(n), SPE(n);
  for(int i = 0; i < n; i++)
  {
    volume_pointer p = _getCellMap3d()[i];
    Alat::IntVector& cells = SPC[i];
    Alat::IntVector& faces = SPF[i];
    Alat::IntVector& nodes = SPN[i];
    Alat::IntVector& edges = SPE[i];
    int nchild = _getVolumes().number_of_children(p);
    if(nchild == 0)
    {
      assert( getVolumeId2Id().find( ( *p )->id() ) != getVolumeId2Id().end() );
      cells.set_size(1);
      cells[0] = getVolumeId2Id()[( *p )->id()];
      nodes.set_size( ( *p )->NumberOfNodes() );
      for(int in = 0; in < ( *p )->NumberOfNodes(); in++)
      {
        nodes[in] = _getNodeId2Id()[( *p )->node(in)->id()];
      }
      edges.set_size( ( *p )->NumberOfEdges() );
      for(int is = 0; is < ( *p )->NumberOfEdges(); is++)
      {
        assert( _getEdgeId2Id().find( ( *( *p )->edge(is) )->id() ) != _getEdgeId2Id().end() );
        edges[is] = _getEdgeId2Id()[( *( *p )->edge(is) )->id()];
      }
      faces.set_size( ( *p )->NumberOfFaces() );
      for(int ii = 0; ii < ( *p )->NumberOfFaces(); ii++)
      {
        assert( _getFaceId2Id().find( ( *( *p )->face(ii) )->id() ) != _getFaceId2Id().end() );
        faces[ii] =_getFaceId2Id()[( *( *p )->face(ii) )->id()];
      }
    }
    else
    {
      cells.set_size(nchild);
      int countnode=0, countedge=0, countface=0;
      Alat::IntMap newnodstoid, newedgetoid, newfactoid;
      for(int ic = 0; ic < nchild; ic++)
      {
        volume_pointer v = _getVolumes().child(p, ic);
        assert( getVolumeId2Id().find( ( *v )->id() ) != getVolumeId2Id().end() );
        cells[ic] = getVolumeId2Id()[( *v )->id()];
        for(int in = 0; in < ( *v )->NumberOfNodes(); in++)
        {
          int icand = _getNodeId2Id()[( ( *v )->node(in) )->id()];
          if(newnodstoid.find(icand)==newnodstoid.end())
          {
            newnodstoid[icand] = countnode++;
          }
        }
        for(int is = 0; is < ( *v )->NumberOfEdges(); is++)
        {
          assert( _getEdgeId2Id().find( ( *( *v)->edge(is) )->id() ) != _getEdgeId2Id().end() );
          int icand = _getEdgeId2Id()[( *( *v )->edge(is) )->id()];
          if(newedgetoid.find(icand)==newedgetoid.end())
          {
            newedgetoid[icand] = countedge++;
          }
        }
        for(int is = 0; is < ( *v )->NumberOfFaces(); is++)
        {
          assert( _getFaceId2Id().find( ( *( *v)->face(is) )->id() ) != _getFaceId2Id().end() );
          int icand = _getFaceId2Id()[( *( *v )->face(is) )->id()];
          if(newfactoid.find(icand)==newfactoid.end())
          {
            newfactoid[icand] = countface++;
          }
        }
      }
      nodes.set_size(countnode);
      edges.set_size(countedge);
      faces.set_size(countface);
      for(Alat::IntMap::const_iterator p = newnodstoid.begin(); p!=newnodstoid.end(); p++)
      {
        nodes[p->second] = p->first;
      }
      for(Alat::IntMap::const_iterator p = newedgetoid.begin(); p!=newedgetoid.end(); p++)
      {
        edges[p->second] = p->first;
      }
      for(Alat::IntMap::const_iterator p = newfactoid.begin(); p!=newfactoid.end(); p++)
      {
        faces[p->second] = p->first;
      }
    }
  }


  FadaMesh::RefineInfo* _refineinfo = dynamic_cast<FadaMesh::RefineInfo*>( _getMesh()->getGeometryObject("RefineInfo") );
  Alat::SparsityPattern& coarsenodeids = _refineinfo->getCoarseNodes();
  Alat::SparsityPattern& coarsecellids = _refineinfo->getCoarseCells();
  Alat::SparsityPattern& coarseedgeids = _refineinfo->getCoarseEdges();
  Alat::SparsityPattern& coarsesideids = _refineinfo->getCoarseSides();


  Alat::IntVector& nodeids = _refineinfo->getNodeIds();
   nodeids.set_size(_getNodeId2Id().size());
   for(Alat::IntMap::const_iterator p = _getNodeId2Id().begin(); p!= _getNodeId2Id().end(); p++)
   {
     nodeids[p->first] = p->second;
   }


  coarsenodeids.set_size(SPN);
  coarsecellids.set_size(SPC);
  coarseedgeids.set_size(SPE);
  coarsesideids.set_size(SPF);
}

/*--------------------------------------------------------------------------*/
void Refiner3d::_markCellToRefine(const volume_pointer v)
{
  int nf = (*v)->NumberOfFaces();
  int ne = (*v)->NumberOfEdges();
  (*v)->to_refine() = 1;
  for(int i = 0; i < nf; i++)
  {
    ( *( (*v)->face(i) ) )->to_refine() = 1;
  }
  for(int i = 0; i < ne; i++)
  {
    ( *( (*v)->edge(i) ) )->nref = 2;
    ( *( (*v)->edge(i) ) )->to_refine() = 1;
  }
}

/*--------------------------------------------------------------------------*/
void Refiner3d::_makeRegular()
{
  assert(0);
  //  std::map<int, VolumeSet> _nodeid2cell;
  //  _nodeid2cell.clear();
  //  for(volume_leafpointer vp = _getVolumes().begin_leaf(); vp != _getVolumes().end_leaf(); vp++)
  //  {
  //    for(int ii = 0; ii < ( *vp )->NumberOfNodes(); ii++)
  //    {
  //      _nodeid2cell[( ( *vp )->node(ii) )->id()].insert(vp);
  //    }
  //  }
  //  while(1)
  //  {
  //    int additional_refine = 0;
  //    for(std::map<int, VolumeSet>::iterator np = _nodeid2cell.begin(); np != _nodeid2cell.end(); np++)
  //    {
  //      const VolumeSet & volumes_of_node = np->second;
  //      int nlevelsignore = INT_MAX;
  //      int maxlevel = INT_MIN;
  //      for(VolumeSet::const_iterator q = volumes_of_node.begin(); q != volumes_of_node.end(); q++)
  //      {
  //        int newdepth = _getVolumes().depth(*q);
  //        if( ( *( *q ) )->to_refine() == 1 )
  //        {
  //          newdepth += 1;
  //        }
  //        nlevelsignore = std::min(nlevelsignore, newdepth);
  //        maxlevel = std::max(maxlevel, newdepth);
  //      }
  //      if(maxlevel > nlevelsignore+2)
  //      {
  //        for(VolumeSet::const_iterator q = volumes_of_node.begin(); q != volumes_of_node.end(); q++)
  //        {
  //          int newdepth = _getVolumes().depth(*q);
  //          std::cerr<<"--- "<<newdepth<<" "<<( *( *q ) )->to_refine()<<std::endl;
  //        }
  //        assert(0);
  //      }
  //      else if(maxlevel == nlevelsignore+2)
  //      {
  //        for(VolumeSet::const_iterator q = volumes_of_node.begin(); q != volumes_of_node.end(); q++)
  //        {
  //          int newdepth = _getVolumes().depth(*q);
  //          if( ( *( *q ) )->to_refine() == 1 )
  //          {
  //            newdepth += 1;
  //          }

  //          if(newdepth == nlevelsignore)
  //          {
  //            if( ( *( *q ) )->to_refine() != 0 )
  //            {
  //              for(VolumeSet::const_iterator q = volumes_of_node.begin(); q != volumes_of_node.end(); q++)
  //              {
  //                int newdepth = _getVolumes().depth(*q);
  //                std::cerr<<"--- "<<newdepth<<" "<<( *( *q ) )->to_refine()<<std::endl;
  //              }
  //              assert(0);
  //            }
  //            additional_refine++;
  //            _markCellToRefine( *( *( *q ) ) );
  //          }
  //        }
  //      }
  //    }
  //    if(additional_refine == 0)
  //    {
  //      break;
  //    }
  //  }
}
