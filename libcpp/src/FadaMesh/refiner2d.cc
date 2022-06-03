#include "FadaMesh/basicadaptivemesh2d.h"
#include "FadaMesh/refiner2d.h"
// #include "FadaMesh/quadtotri.h"
#include "FadaMesh/refineinfo.h"
#include  <limits>

using namespace FadaMesh;
using namespace std;


/*--------------------------------------------------------------------------*/
void Refiner2d::globalRefine(int nref)
{
  //  std::cerr<<"Refiner2d::globalRefine "<<nref<<'\n';
  //  if(nref == 0)
  //  {
  //    return;
  //  }
  for(int i = 0; i < nref; i++)
  {
    int comp=0;
    for(face_leafpointer f = _getFaces().begin_leaf(); f != _getFaces().end_leaf(); f++)
    {
      _markCellToRefine( f  );
    }
    _refine();
    _constructAdaptInfo();
    _getMesh()->reInitFadaMesh();
  }
  // if( _getMesh()->quadToTri() )
  // {
  //   _refineQuadToTri();
  // }
}

/*--------------------------------------------------------------------------*/
void Refiner2d::_updateCopyEdge()
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
        if( ( *E )->copy()>0)
        {
          std::cerr<<"updatCopy ref"<<( *E )->boundaryid()<<'\n';
        }
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
void Refiner2d::adaptMesh(std::string filename)
{
  Alat::IntVector marked_fadalightmesh_cells;
  std::ifstream file( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr<<"*** ERROR BasicRefiner2d::refine() cannot open file \""<<filename<<"\"\n";
    exit(5);
  }

  marked_fadalightmesh_cells.loadFada(file);
  file.close();

  for(int i = 0; i < marked_fadalightmesh_cells.size(); i++)
  {
    int imarked = marked_fadalightmesh_cells[i];
    face_pointer f = _getCellMap2d()[imarked];
    if(not *f)
    {
      std::cerr << "*** ERROR in Refiner2d::adaptMesh(): face_pointer NULL\n";
      assert(0);
      exit(1);
    }
    _markCellToRefine( f );
  }
  _makeRegular();
  _refine();
  _updateCopyEdge();
  _constructAdaptInfo();
  _getMesh()->updateHangingInfo();
  _getMesh()->reInitFadaMesh();
}

/*--------------------------------------------------------------------------*/
void Refiner2d::_refine()
{
  _getCellMapOk() = false;
  _getNumberingOk() = false;
  // put nodes on boundary
  // if(curvedboundaryinformation)
  // {
  // for(edge_leafpointer e = _getEdges().begin_leaf(); e != _getEdges().end_leaf(); e++)
  // {
  //   int color = ( *e )->boundaryid();
  //   if( curvedboundaryinformation->boundaryColorIsCurved(color) )
  //   {
  //     const FadaMesh::CurvedBoundaryDescriptionInterface* BDI =  curvedboundaryinformation->get(color);
  //     for(int ii = 0; ii < 2; ii++)
  //     {
  //       BDI->newton( ( *e )->node(ii)->getNode() );
  //     }
  //   }
  // }
  // }
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
    // if(curvedboundaryinformation)
    // {
    // int color = ( *e )->boundaryid();
    // if( curvedboundaryinformation->boundaryColorIsCurved(color) )
    // {
    //   const FadaMesh::CurvedBoundaryDescriptionInterface* BDI = curvedboundaryinformation->get(color);
    //   for(int ii = 0; ii < newnodes.size(); ii++)
    //   {
    //     BDI->newton( newnodes[ii]->getNode() );
    //   }
    // }
    // }
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
    // for(int i=0;i<newnodes.size();i++)
    // {
    //   std::cerr << "???? newnodes " << newnodes[i]->getNode() << "\n";
    // }

    for(std::vector<Node*>::iterator it = newnodes.begin(); it != newnodes.end(); it++)
    {
      ( *it )->id() = ++_getLastNodeId();
      _getNodes().insert(*it);
    }
    for(std::vector<FaceInterface*>::iterator it = newfaces.begin(); it != newfaces.end(); it++)
    {
      face_pointer newf = _getFaces().append_child(f, *it);
      for(int ii = 0; ii < ( *newf )->NumberOfEdges(); ii++)
      {
        edge_pointer enewf = ( *newf )->edge(ii);
        ( *enewf )->face( ( *enewf )->nfaces ) = newf;
        ( *enewf )->nfaces++;
      }
      ( *it )->id() = ++_getLastFaceId();
      ( *it )->depth() = ( *f )->depth()+1;
    }
  }
}

/*--------------------------------------------------------------------------*/
void Refiner2d::_constructAdaptInfo()
{
  _getMesh()->createGeometryObject("RefineInfo");
  _getMesh()->constructNumbering();
  int n = _getCellMap2d().size();
  Alat::Vector<Alat::IntVector> SPC(n), SPN(n), SPE(n);
  for(int i = 0; i < n; i++)
  {
    face_pointer p = _getCellMap2d()[i];
    Alat::IntVector& cells = SPC[i];
    Alat::IntVector& nodes = SPN[i];
    Alat::IntVector& edges = SPE[i];
    int nchild = _getFaces().number_of_children(p);
    if(nchild == 0)
    {
      assert( _getFaceId2Id().find( ( *p )->id() ) != _getFaceId2Id().end() );
      cells.set_size(1);
      cells[0] = _getFaceId2Id()[( *p )->id()];
      int pnnodes = ( *p )->NumberOfNodes();
      assert(pnnodes==4);
      nodes.set_size( pnnodes );
      for(int in = 0; in < pnnodes; in++)
      {
        nodes[in] = _getNodeId2Id()[( *p )->node(in)->id()];
      }
      int pnedges = ( *p )->NumberOfEdges();
      assert(pnedges==4);
      edges.set_size( pnedges );
      for(int is = 0; is < pnedges; is++)
      {
        assert( _getEdgeId2Id().find( ( *( *p )->edge(is) )->id() ) != _getEdgeId2Id().end() );
        edges[is] = _getEdgeId2Id()[( *( *p )->edge(is) )->id()];
      }
    }
    else
    {
      cells.set_size(nchild);
      int countnode=0, countedge=0;
      Alat::IntMap newnodstoid, newedgetoid;
      for(int ic = 0; ic < nchild; ic++)
      {
        face_pointer f = _getFaces().child(p, ic);
        assert( _getFaceId2Id().find( ( *f )->id() ) != _getFaceId2Id().end() );
        cells[ic] = _getFaceId2Id()[( *f )->id()];
        for(int in = 0; in < ( *f )->NumberOfNodes(); in++)
        {
          int icand = _getNodeId2Id()[( ( *f )->node(in) )->id()];
          if(newnodstoid.find(icand)==newnodstoid.end())
          {
            newnodstoid[icand] = countnode++;
          }
          // if( std::find( nodes.begin(), nodes.end(), _getNodeId2Id()[( *f )->node(in)->id()] ) == nodes.end() )
          // {
          //   countnode++;
          // }
        }
        for(int is = 0; is < ( *f )->NumberOfEdges(); is++)
        {
          assert( _getEdgeId2Id().find( ( *( *f )->edge(is) )->id() ) != _getEdgeId2Id().end() );
          int icand = _getEdgeId2Id()[( *( *f )->edge(is) )->id()];
          if(newedgetoid.find(icand)==newedgetoid.end())
          {
            newedgetoid[icand] = countedge++;
          }
          // if( std::find(edges.begin(), edges.end(), icand) == edges.end() )
          // {
          //   countedge++;
          // }
        }
      }
      // std::cerr << "Refiner2d::_constructAdaptInfo() countnode="<<countnode<<" countedge="<<countedge<<"\n";
      nodes.set_size(countnode);
      edges.set_size(countedge);
      for(Alat::IntMap::const_iterator p = newnodstoid.begin(); p!=newnodstoid.end(); p++)
      {
        nodes[p->second] = p->first;
      }
      for(Alat::IntMap::const_iterator p = newedgetoid.begin(); p!=newedgetoid.end(); p++)
      {
        edges[p->second] = p->first;
      }
      // countnode = countedge = 0;
      // for(int ic = 0; ic < nchild; ic++)
      // {
      //   face_pointer f = _getFaces().child(p, ic);
      //   for(int in = 0; in < ( *f )->NumberOfNodes(); in++)
      //   {
      //     int icand = _getNodeId2Id()[( ( *f )->node(in) )->id()];
      //     if( std::find( nodes.begin(), nodes.end(), icand ) == nodes.end() )
      //     {
      //       nodes[countnode++] = icand;
      //       // nodes.push_back( _getNodeId2Id()[( *f )->node(in)->id()] );
      //     }
      //   }
      //   for(int is = 0; is < ( *f )->NumberOfEdges(); is++)
      //   {
      //     int icand = _getEdgeId2Id()[( *( *f )->edge(is) )->id()];
      //     if( std::find(edges.begin(), edges.end(), icand) == edges.end() )
      //     {
      //       edges[countedge++] = icand;
      //       // edges.push_back(icand);
      //     }
      //   }
      // }
    }
    // std::cerr << "nodes " << nodes << "\n";
    // SPC[i] = cells;
    // SPN[i] = nodes;
    // SPE[i] = edges;
  }
  FadaMesh::RefineInfo* _refineinfo = dynamic_cast<FadaMesh::RefineInfo*>( _getMesh()->getGeometryObject("RefineInfo") );
  Alat::SparsityPattern& coarsenodeids = _refineinfo->getCoarseNodes();
  Alat::SparsityPattern& coarsecellids = _refineinfo->getCoarseCells();
  Alat::SparsityPattern& coarsesideids = _refineinfo->getCoarseSides();



  // std::cerr << "##################### _getNodeId2Id()\n" << _getNodeId2Id() << "\n";
  //
  Alat::IntVector& nodeids = _refineinfo->getNodeIds();
  nodeids.set_size(_getNodeId2Id().size());
  for(Alat::IntMap::const_iterator p = _getNodeId2Id().begin(); p!= _getNodeId2Id().end(); p++)
  {
    nodeids[p->first] = p->second;
  }


  coarsenodeids.set_size(SPN);
  coarsecellids.set_size(SPC);
  coarsesideids.set_size(SPE);
  //
  // std::cerr << "#####################\n";
  // coarsenodeids.print(std::cerr);



  // coarsenodeids.write("toto");
  //
  // Alat::SparsityPattern coarsenodeids2;
  // coarsenodeids2.read("toto");
  // std::cerr << "#####-----------######\n";
  // coarsenodeids2.print(std::cerr);
  //

  _refineinfo->refinfoinfonode[0] = 6;
  _refineinfo->refinfoinfonode[1] = 1;
  _refineinfo->refinfoinfonode[2] = 4;
  _refineinfo->refinfoinfonode[3] = 8;
  _refineinfo->refinfoinfoside[0] = 0;
  _refineinfo->refinfoinfoside[1] = 2;
  _refineinfo->refinfoinfoside[2] = 5;
  _refineinfo->refinfoinfoside[3] = 7;
  _refineinfo->refinfoinfocell[0] = 1;
  _refineinfo->refinfoinfocell[1] = 2;
  _refineinfo->refinfoinfocell[2] = 0;
  _refineinfo->refinfoinfocell[3] = 3;

}

/*--------------------------------------------------------------------------*/
void Refiner2d::_markCellToRefine(const face_pointer f)
{
  int ne = ( *f )->NumberOfEdges();
  ( *f )->to_refine() = 1;
  if(_getMesh()->getName() == "FadaMesh::TriangleMesh")
  {
    face_pointer pf = _getFaces().parent(f);
    if(pf != NULL)
    {
      int nchild = _getFaces().number_of_children(pf);
      if(nchild == 4)
      {
        if( f == ( _getFaces().child(pf, 3) ) )
        {
          for(int i = 0; i < 3; i++)
          {
            face_pointer childpf = _getFaces().child(pf, i);
            ( *childpf )->to_refine() = 1;
            int nepf = ( *childpf )->NumberOfEdges();
            for(int i = 0; i < nepf; i++)
            {
              ( *( ( *childpf )->edge(i) ) )->nref = 2;
              ( *( ( *childpf )->edge(i) ) )->to_refine() = 1;
            }
          }
        }
      }
    }
  }
  for(int i = 0; i < ne; i++)
  {
    ( *( ( *f )->edge(i) ) )->nref = 2;
    ( *( ( *f )->edge(i) ) )->to_refine() = 1;
  }
}

/*--------------------------------------------------------------------------*/
void Refiner2d::_makeRegular()
{
  std::map<int, FaceSet> _nodeid2cell;
  _nodeid2cell.clear();
  for(face_leafpointer fp = _getFaces().begin_leaf(); fp != _getFaces().end_leaf(); fp++)
  {
    for(int ii = 0; ii < ( *fp )->NumberOfNodes(); ii++)
    {
      _nodeid2cell[( ( *fp )->node(ii) )->id()].insert(fp);
    }
  }

  while(1)
  {
    int additional_refine = 0;
    for(std::map<int, FaceSet>::iterator np = _nodeid2cell.begin(); np != _nodeid2cell.end(); np++)
    {
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
      if(maxlevel > nlevelsignore+2)
      {
        for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
        {
          int newdepth = _getFaces().depth(*q);
          std::cerr<<"--- "<<newdepth<<" "<<( *( *q ) )->to_refine()<<std::endl;
        }
        assert(0);
      }
      else if(maxlevel == nlevelsignore+2)
      {
        for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
        {
          int newdepth = _getFaces().depth(*q);
          if( ( *( *q ) )->to_refine() == 1 )
          {
            newdepth += 1;
          }

          if(newdepth == nlevelsignore)
          {
            if( ( *( *q ) )->to_refine() != 0 )
            {
              for(FaceSet::const_iterator q = faces_of_node.begin(); q != faces_of_node.end(); q++)
              {
                int newdepth = _getFaces().depth(*q);
                std::cerr<<"--- "<<newdepth<<" "<<( *( *q ) )->to_refine()<<std::endl;
              }
              assert(0);
            }
            additional_refine++;
            _markCellToRefine( ( *q ) );
          }
        }
      }
    }
    if(additional_refine == 0)
    {
      break;
    }
  }
}
