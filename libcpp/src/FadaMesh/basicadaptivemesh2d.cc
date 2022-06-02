#include  "FadaMesh/basicadaptivemesh2d.h"
#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/hangingsideinfo.h"
#include  <algorithm>
#include  <fstream>
#include  <iterator>

using namespace FadaMesh;
using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/
template <int NODESPERCELL>
// BasicAdaptiveMesh2d<NODESPERCELL>::BasicAdaptiveMesh2d():_quad_to_tri(false), _quad_to_tri_pointer(NULL),
//   _quad_to_tri_name("none"),AdaptiveMeshInterface(),FadaMesh::FadaMeshBase2d<NODESPERCELL>(){}
  BasicAdaptiveMesh2d<NODESPERCELL>::BasicAdaptiveMesh2d(): AdaptiveMeshInterface(),FadaMesh::FadaMeshBase2d<NODESPERCELL>(){}


/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::BasicInit(const std::string& type)
{
   _cell_map_ok=false;
   _numbering_ok=false;
}


/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::constructCellMap()
{
  if (_cell_map_ok) return;
  // Construction du map permettant de passer d'un numéro de feuille à la face correspondante
  _cellmap_fadalightmesh.clear();
  int count = 0;
  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    _cellmap_fadalightmesh[count++] = p;
  }
  _cell_map_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::constructNumbering()
{
  if (_numbering_ok) return;
  _edgeid2id.clear();
  _faceid2id.clear();
  _nodeid2id.clear();
  _copy_edge.clear();
  int count;
  count = 0;
  // on numérote les noeuds (dans le cas d'une procédure de déraffinement un noeud peut avoir une
  // identité supérieure au nombre de noeuds)
  for(NodeSet::iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
  {
    _nodeid2id[( *p )->id()] = count++;
  }
  // on numérote les feuilles de l'arbre des edges
  int count_edge = 0;
  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    _edgeid2id[( *p )->id()] = count_edge++;
  }
  // on numérote les feuilles de l'arbre des faces
  count = 0;
  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    _faceid2id[( *p )->id()] = count++;
    for(int ii = 0; ii < (*p)->NumberOfEdges(); ii++)
    {
      edge_pointer E = (*p)->edge(ii);
      if ( (*E)->copy()>0)
      {
        _copy_edge.insert(E);
        _edgeid2id[(*E)->id()]=count_edge++;
      }
    }
  }
  _numbering_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::writeAdaptiveMesh(std::string name, std::string datatype)
{
// on met la mumérotation à jour

  constructCellMap();
  constructNumbering();

// Ecriture des arbres constituants le maillage adaptatif
  if(datatype == "ascii")
  {
    std::cout<<"AdaptiveMesh::write() writing  (fadalightmeshadaptive) "<<name<<" : "<<datatype<<"\n";
  }
  //! create directory

  string dirname= name+".fadalightmeshadaptive";
  string cmd = "rm -rf "+dirname;
  int error = system( cmd.c_str() );
  assert(!error);
  cmd = "mkdir "+dirname;
  error = system( cmd.c_str() );
  assert(!error);
  string filename;
  std::ofstream file;
  file.precision(12);
  //! write type
  filename = dirname+"/type";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file<<getName()<<" "<<datatype<<"\n";
  file.close();
  //! write nodes
  filename = dirname+"/nodes";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file<<_Nodes.size()<<" "<<datatype<<"\n";
  for(set<Node*>::const_iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
  {
    ( *p )->write(file, datatype);
  }
  file.close();
  //! write faces
  filename = dirname+"/faces";
  file.open( filename.c_str() );
  assert( file.is_open() );
  std::string facename("none");
  face_pointer ps = _Faces.begin();
  if( ps != _Faces.end() )
  {
    facename = ( *ps )->getName();
  }
  file<<_Faces.size()<<" "<<datatype<<" "<<facename<<"\n";
  for(face_pointer p = _Faces.begin(); p != _Faces.end(); p++)
  {
    ( *p )->write(file, datatype);
  }
  file.close();
  //! write edges

  filename = dirname+"/edges";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file<<_Edges.size()<<" "<<datatype<<"\n";
  for(edge_pointer p = _Edges.begin(); p != _Edges.end(); p++)
  {
    ( *p )->write(file, datatype);
  }
  file.close();
  //!write face_tree
  filename = dirname+"/faceparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap faceparents;
  for(face_pointer p = _Faces.begin(); p != _Faces.end(); p++)
  {
    int ip =  ( *p )->id();
    if( _Faces.depth(p) )
    {
      faceparents[ip] = ( *_Faces.parent(p) )->id();
    }
    else
    {
      faceparents[ip] = -1;
    }
  }
  faceparents.write(file, datatype);
  file.close();
  //!write edgeparents
  filename = dirname+"/edgeparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap edgeparents;
  for(edge_pointer p = _Edges.begin(); p != _Edges.end(); p++)
  {
    int ip =  ( *p )->id();
    if( _Edges.depth(p) )
    {
      edgeparents[ip] = ( *_Edges.parent(p) )->id();
    }
    else
    {
      edgeparents[ip] = -1;
    }
  }
  edgeparents.write(file, datatype);
  file.close();
  //!write cell_map
  filename = dirname+"/cell_map";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap cellmap;
  for(map<int, face_pointer>::iterator p = _cellmap_fadalightmesh.begin(); p != _cellmap_fadalightmesh.end(); p++)
  {
    int ip =  ( *( ( *p ).second ) )->id();
    cellmap[p->first] = ip;
  }
  cellmap.write(file, datatype);
  file.close();

  //!write _nodeid2id
  filename = dirname+"/nodeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _nodeid2id.write(file, datatype);
  file.close();
  //!write _edgeid2id
  filename = dirname+"/edgeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _edgeid2id.write(file, datatype);
  file.close();
  //!write _faceid2id
  filename = dirname+"/faceid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _faceid2id.write(file, datatype);
  file.close();
  //!write _curvedboundaries
  // filename = dirname+"/CurvedBoundaryInformation";
  // _curvedboundaries.write(filename, datatype);
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::readAdaptiveMesh(std::string name, std::string lastnamein)
{
  //! init _last_id_
   _last_node_id=-1;
   _last_face_id=-1;
   _last_edge_id=-1;
  //! create directory name
  std::string inadaptive=name+".fadalightmesh/"+lastnamein;
  string dirname = inadaptive+".fadalightmeshadaptive";

  int n;
  string filename, datatype, facename;
  std::ifstream file;
  //! read type
  filename = dirname+"/type";
  file.open( filename.c_str() );
  if( not file.is_open() )
  {
    std::cerr<<"*** ERROR in  AdaptiveMesh::read() cannot open file \""<<filename<<"\"\n";
    assert(0);
    exit(1);
  }
  assert( file.is_open() );
  file>>datatype;
  file.close();
  if(datatype == "ascii")
  {
    std::cout<<"AdaptiveMesh::read() reading (fadalightmeshadaptive) "<<name<<"\n";
  }
  // clear set and map
  _Nodes.clear();
  _Faces.clear();
  _Edges.clear();
  _copy_edge.clear();
  //! read nodes
  filename = dirname+"/nodes";
  file.open( filename.c_str() );
  assert( file.is_open() );
  int nsize;
  file>>nsize;
  file>>datatype;
  std::map<int, Node*> id2node;
  for(int i = 0; i < nsize; i++)
  {
    Node* N = new Node(file);
    _last_node_id=std::max(N->id(),_last_node_id);
    _Nodes.insert(N);
    id2node[N->id()] = N;
  }

  file.close();

  map<int, face_pointer> id2face;
  map<int, edge_pointer> id2edge;

  //! read edgeparents
  filename = dirname+"/edgeparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap edgeparents;
  edgeparents.read(file);
  file.close();
  for(Alat::IntMap::const_iterator p = edgeparents.begin(); p != edgeparents.end(); p++)
  {
    Edge* E = new Edge;
    if(p->second < 0)
    {
      id2edge[p->first] = _Edges.insert(_Edges.end(), E);
    }
    else
    {
      map<int, edge_pointer>::const_iterator q = id2edge.find(p->second);
      assert( q != id2edge.end() );
      id2edge[p->first] = _Edges.append_child(q->second, E);
    }
  }
  //! read faces type
  filename = dirname+"/faces";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file>>n>>datatype>>facename;
  file.close();

  //! read faceparents
  filename = dirname+"/faceparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap faceparents;
  faceparents.read(file);
  file.close();
  for(Alat::IntMap::const_iterator p = faceparents.begin(); p != faceparents.end(); p++)
  {
    FaceInterface* F;
    if(facename == "Quad")
    {
      F = new Quad;
    }
    else if(facename == "Tri")
    {
      F = new Tri;
    }
    else
    {
      assert(0);
    }
    if(p->second == -1)
    {
      id2face[p->first] = _Faces.insert(_Faces.end(), F);
    }
    else
    {
      map<int, face_pointer>::const_iterator q = id2face.find(p->second);
      assert( q != id2face.end() );
      id2face[p->first] = _Faces.append_child(q->second, F);
    }
  }


  //! read edges
  filename = dirname+"/edges";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file>>n>>datatype;
  int ide, ideold;
  for(int i = 0; i < n; i++)
  {
    file>>ide>>ideold;
    edge_pointer& E = id2edge[ide];
    ( *E )->id() = ide;
    ( *E )->oldid() = ideold;
    ( *E )->set_size(file, id2node, id2face);
    int copy = ( *E )->copy();
    if(copy>0)
    {
      _copy_edge.insert(E);
    }
  }
  file.close();
  for(edge_pointer p = _Edges.begin(); p != _Edges.end(); p++)
  {
    int ip =  ( *p )->id();
    _last_edge_id=std::max(ip,_last_edge_id);
  }
  //! read faces
  filename = dirname+"/faces";
  file.open( filename.c_str() );
  assert( file.is_open() );
  file>>n>>datatype>>facename;
  if(facename == "Quad")
  {
    int idf, idfold,boundaryid;
    for(int i = 0; i < n; i++)
    {
      file>>idf>>idfold>>boundaryid;
      face_pointer& C = id2face[idf];
      Quad* Q = dynamic_cast<Quad*>( *C );
      assert(Q);
      Q->id() = idf;
      Q->oldid() = idfold;
      Q->boundaryid()=boundaryid;
      Q->set_size(file, id2node, id2edge);
    }
  }
  else if(facename == "Tri")
  {
    int idf, idfold;
    for(int i = 0; i < n; i++)
    {
      file>>idf>>idfold;
      face_pointer& C = id2face[idf];
      Tri* T = dynamic_cast<Tri*>( *C );
      assert(T);
      T->id() = idf;
      T->oldid() = idfold;
      T->set_size(file, id2node, id2edge);
    }
  }
  else
  {
    assert(0);
  }
  file.close();
  for(face_pointer p = _Faces.begin(); p != _Faces.end(); p++)
  {
    int ip =  ( *p )->id();
    _last_face_id=std::max(ip,_last_face_id);
  }
  //!read _curvedboundaries
  // filename = dirname+"/CurvedBoundaryInformation";
  // _curvedboundaries.read(filename);

  //!read cell_map
  filename = dirname+"/cell_map";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap cellmap;
  cellmap.read(file);
  for(map<int, int>::iterator p = cellmap.begin(); p != cellmap.end(); p++)
  {
    // std::cerr << "_cellmap_fadalightmesh   " << p->first << " " << p->second << " " << *id2face[p->second] << "\n";
    _cellmap_fadalightmesh[p->first] = id2face[p->second];
  }
  file.close();
  //!read _nodeid2id
  filename = dirname+"/nodeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _nodeid2id.read(file);
  file.close();
  //!read _edgeid2id
  filename = dirname+"/edgeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _edgeid2id.read(file);
  file.close();
  //!read _faceid2id
  filename = dirname+"/faceid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _faceid2id.read(file);
  file.close();
  FadaMeshBase2d<NODESPERCELL>::readFadaMesh(name);
  _cell_map_ok=true;
  _numbering_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::readFadaMeshAndInitTrees(std::string name)
{
// lecture du FadaMesh
  FadaMeshBase2d<NODESPERCELL>::readFadaMesh(name);

//lecture des CurvedBoundaryInfo
  // if(FadaMeshBase2d<NODESPERCELL>::geometryObjectExists("CurvedBoundaryInformation") )
  // {
  //   const FadaMesh::GeometryObject* GO = FadaMeshBase2d<NODESPERCELL>::getGeometryObject("CurvedBoundaryInformation");
  //   const FadaMesh::CurvedBoundaryInformation* BD = dynamic_cast<const FadaMesh::CurvedBoundaryInformation*>( GO );
  //   assert(BD);
  //   _curvedboundaries.set_size(*BD);
  // }
//map provisoires pour connectivités
  map<int, Node*> id2node;
  map<int, edge_pointer> id2edge;
//construction des nodes
  for(int iN = 0; iN < FadaMeshBase2d<NODESPERCELL>::getNNodes(); iN++)
  {
    const Alat::Node& v = FadaMeshBase2d<NODESPERCELL>::getNode(iN);
    Node* np = new Node( iN, v );
    _Nodes.insert(np);
    id2node[iN] = np;
  }
//construction des faces et du cellmap
  for(int iK = 0; iK < FadaMeshBase2d<NODESPERCELL>::getNCells(); iK++)
  {
    FaceInterface* F = _newFace(iK);
    F->id() = iK;
    for(int ii = 0; ii < getNNodesPerCell(); ii++)
    {
      F->node(ii) = id2node[FadaMeshBase2d<NODESPERCELL>::getNodeIdOfCell(iK, ii)];
    }
    _cellmap_fadalightmesh[iK] = _Faces.insert(_Faces.end(), F);
  }

//construction des edges
  for(int iS = 0; iS < FadaMeshBase2d<NODESPERCELL>::getNSides(); iS++)
  {
    Edge* E = new Edge;
    E->id() = iS;
    for(int ii = 0; ii < getNNodesPerSide(); ii++)
    {
      E->node(ii) = id2node[FadaMeshBase2d<NODESPERCELL>::getNodeIdOfSide(iS, ii)];
    }
    // il faut encore donner les edges, le bord
    id2edge[iS] = _Edges.insert(_Edges.end(), E);
  }

// remplissage des informations manquantes :
// les edges composant chaque face
  for(int iK = 0; iK < FadaMeshBase2d<NODESPERCELL>::getNCells(); iK++)
  {
    FaceInterface* F = *_cellmap_fadalightmesh[iK];
    for(int ii = 0; ii <getNSidesPerCell(); ii++)
    {
      F->edge(ii) = id2edge[FadaMeshBase2d<NODESPERCELL>::getSideIdOfCell(iK, ii)];
    }
  }
// Les faces de part et d'autre de chaque edge
  for(int iS = 0; iS < FadaMeshBase2d<NODESPERCELL>::getNSides(); iS++)
  {
    Edge* E = *id2edge[iS];
    for(int ii = 0; ii < 2; ii++)
    {
      int iK = FadaMeshBase2d<NODESPERCELL>::getCellIdOfSide(iS, ii);
      if(iK >= 0)
      {
        E->face(ii) = _cellmap_fadalightmesh[iK];
      }
    }
  }
// les boundary info pour chaque edge
  const FadaMesh::BoundaryInfo* BI = FadaMeshBase2d<NODESPERCELL>::getBoundaryInfo();
  const Alat::IntVector colors = BI->getColors();
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& edges = BI->getSidesOfColor(color);
    for(int i = 0; i < edges.size(); i++)
    {
      int iS = edges[i];
      Edge* E = *id2edge[iS];
      E->boundaryid() = color;
    }
  }
// on met à jour les numérotations
  _last_node_id=FadaMeshBase2d<NODESPERCELL>::getNNodes()-1;
  _last_edge_id=FadaMeshBase2d<NODESPERCELL>::getNSides()-1;
  _last_face_id=FadaMeshBase2d<NODESPERCELL>::getNCells()-1;
  constructNumbering();
  constructCellMap();
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::writeFadaMesh(std::string name, std::string datatype)
{
  // Reinitialisation du FadaMesh à partir des feuilles de l'arbre
    reInitFadaMesh();
  // Mise à jour des information sur les bords courbes
    // FadaMeshBase2d<NODESPERCELL>::reInit();
    FadaMeshBase2d<NODESPERCELL>::getCurvedBoundaryInformation()->constructBoundaryInformation(this);
    FadaMeshBase2d<NODESPERCELL>::writeFadaMesh(name,datatype);
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::reInitFadaMesh()
{
// on remet à jour le maillage FadaMesh à partir des arbres
// si besoin on met à jour la numérotation
  constructNumbering();
  constructCellMap();

// remplissage du tableau des nodes
  Alat::Vector<Alat::Node>& nodes = FadaMeshBase2d<NODESPERCELL>::getAllNodes();
  nodes.set_size( _Nodes.size() );
  for(set<Node*>::iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
  {
    int id = _nodeid2id[(*p)->id()];
    nodes[id] = ( *p )->getNode();
  }
// Les cells avec la connectivité SidesOfCells
  FadaMeshBase2d<NODESPERCELL>::getCells().set_size( _faceid2id.size() );
  FadaMeshBase2d<NODESPERCELL>::getSidesOfCells().set_size( _faceid2id.size() );
  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    FaceInterface* F = *p;
    int id = _faceid2id[F->id()];
    for(int ii = 0; ii < getNNodesPerCell(); ii++)
    {
      FadaMeshBase2d<NODESPERCELL>::getCells()[id][ii] = _nodeid2id[F->node(ii)->id()];
    }
    for(int ii = 0; ii < getNSidesPerCell(); ii++)
    {
      FadaMeshBase2d<NODESPERCELL>::getSidesOfCells()[id][ii] = _edgeid2id[( *F->edge(ii) )->id()];
    }
  }
// les sides avec la connectivité CellsOfSides
  int _n_leaf_edges= _edgeid2id.size();
  FadaMeshBase2d<NODESPERCELL>::getSides().set_size(_n_leaf_edges);
  FadaMeshBase2d<NODESPERCELL>::getCellsOfSides().set_size(_n_leaf_edges);
  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    Edge* E = *p;
    int id = _edgeid2id[E->id()];
    for(int ii = 0; ii < getNNodesPerSide(); ii++)
    {
      FadaMeshBase2d<NODESPERCELL>::getSides()[id][ii] = _nodeid2id[E->node(ii)->id()];
    }

    for(int ii = 0; ii < 2; ii++)
    {
      if(E->face(ii) != NULL)
      {
        FaceInterface* F = *E->face(ii);
        if( _faceid2id.find( F->id() ) != _faceid2id.end() )
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = _faceid2id[F->id()];
        }
        else
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -3; // il s'agit d'un petit side d'un HangingSide
        }
      }
      else
      {
        if(E->boundaryid() == -1)
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -3;// il s'agit d'un petit side d'un HangingSide
        }
        else
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -1;// il s'agit d'un side sur le bord
        }
      }
    }
    if(FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0] < 0)
    {
      int temp = FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0];
      FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0] = FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][1];
      FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][1] = temp;
    }
  }
// les edges recopiés font partie des feuilles, on les traite également
  for(std::set<edge_pointer>::iterator it = _copy_edge.begin(); it != _copy_edge.end(); it++)
  {
    Edge* E = *( *it );
    int id = _edgeid2id[E->id()];
    for(int ii = 0; ii < getNNodesPerSide(); ii++)
    {
      FadaMeshBase2d<NODESPERCELL>::getSides()[id][ii] = _nodeid2id[E->node(ii)->id()];
    }
    for(int ii = 0; ii < 2; ii++)
    {
      if(E->face(ii) != NULL)
      {
        FaceInterface* F = *E->face(ii);
        if( _faceid2id.find( F->id() ) != _faceid2id.end() )
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = _faceid2id[F->id()];
        }
        else
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -2;// il s'agit du grand side d'un HangingSide
        }
      }
      else
      {
        if(E->boundaryid() == -1)
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -2;// il s'agit du grand side d'un HangingSide
        }
        else
        {
          FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][ii] = -1;// il s'agit d'un side sur le bord
        }
      }
    }
    // on permute si besoin pour faire en sorte que le premier indice corresponde
    // vraiment à une face (indice >0)
    if(FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0] < 0)
    {
      int temp = FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0];
      FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][0] = FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][1];
      FadaMeshBase2d<NODESPERCELL>::getCellsOfSides()[id][1] = temp;
    }
  }
  //boundary info
  FadaMesh::BoundaryInfo* BI = FadaMeshBase2d<NODESPERCELL>::getBoundaryInfo();
  std::map<int, int> size_of_color;

  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    Edge*  E = *p;
    int color = E->boundaryid();
    if(color != -1)
    {
      if( size_of_color.find(color) == size_of_color.end() )
      {
        size_of_color[color] = 1;
      }
      else
      {
        size_of_color[color]++;
      }
    }
  }
  BI->set_size(size_of_color);
  for(std::map<int, int>::iterator p = size_of_color.begin(); p != size_of_color.end(); p++)
  {
    p->second = 0;
  }
  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    Edge* E = *p;
    int color = E->boundaryid();
    if(color != -1)
    {
      BI->getSidesOfColor(color)[size_of_color[color]] = _edgeid2id[E->id()];
      assert(E->face(0) != NULL);
      FaceInterface* F = *E->face(0);
      assert(F);
      BI->getCellsOfColor(color)[size_of_color[color]] = _faceid2id[F->id()];
      for(int ii = 0; ii < getNSidesPerCell(); ii++)
      {
        if(*F->edge(ii) == E)
        {
          BI->getSidesIdOfCellsOfColor(color)[size_of_color[color]] = ii;
          break;
        }
      }
      size_of_color[color]++;
    }
  }

}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL>
void BasicAdaptiveMesh2d<NODESPERCELL>::updateHangingInfo()
{
 // Mise à jour des informations sur les HangingNodes et Side

// mise à jour des numérotations et du cell map
  constructNumbering();
  constructCellMap();
  typedef std::map<edge_pointer, std::pair<FaceInterface*, int>, EdgeCompare> NewHangingInfo;
  NewHangingInfo _hangingedges;
  for(face_leafpointer f = _Faces.begin_leaf(); f != _Faces.end_leaf(); f++)
  {
    FaceInterface* F = *f;
    for(int ii = 0; ii < F->NumberOfEdges(); ii++)
    {
      edge_pointer E = F->edge(ii);
      // seuls les egdes recopiés sont des hanging edges
      if ((*E)->copy()>0) {
        _hangingedges.insert( make_pair( E, make_pair(F, ii) ) );
      }
    }
  }
  int n_hanging=_hangingedges.size();

  int n_local_data_hnode=3; //cas 2D on stocke, le cell_id, le localcell_id, et le noeud_id
  int n_local_data_hside=4; //cas 2D on stocke, le cell_id, le localcell_id, et les deux side_id
  createGeometryObject("HangingNodeInfo");
  createGeometryObject("HangingSideInfo");
// on récupère les objets géométriques pour pouvoir les remplir
  FadaMesh::HangingNodeInfo* hnodes= dynamic_cast<FadaMesh::HangingNodeInfo*>(FadaMeshBase2d<NODESPERCELL>::getGeometryObject("HangingNodeInfo"));
  FadaMesh::HangingSideInfo* hsides= dynamic_cast<FadaMesh::HangingSideInfo*>(FadaMeshBase2d<NODESPERCELL>::getGeometryObject("HangingSideInfo"));
  hnodes->set_size(n_hanging, n_local_data_hnode);
  hsides->set_size(n_hanging, n_local_data_hside);
  if (n_hanging==0) return;
  int count=0;
// remplissage des objets géométriques
  for(map<edge_pointer, std::pair<FaceInterface*, int> >::iterator p = _hangingedges.begin(); p != _hangingedges.end(); p++)
  {
    edge_pointer E = p->first;
    FaceInterface* F = p->second.first;
    int icell= _faceid2id[F->id()];
    int local_side_id = p->second.second;
    hsides->getCellNumber(count)=icell;
    hnodes->getCellNumber(count)=icell;
    hsides->getLocalSide(count)=local_side_id;
    hnodes->getLocalSide(count)=local_side_id;
    assert(_Edges.number_of_children(E)==2);
    edge_pointer EC = _Edges.child(E, 0);
    int node_id= _nodeid2id[( *EC )->node(1)->id()];
    hnodes->getHangingNodes(count,0)= node_id;
    for(int ii = 0; ii < _Edges.number_of_children(E); ii++)
    {
      edge_pointer EC = _Edges.child(E, ii);
      int edge_id= _edgeid2id[( *EC )->id()];
      hsides->getHangingSides(count,ii)=edge_id;
    }
    count++;
  }
}
/*---------------------------------------------------------*/

// triangle mesh
template class FadaMesh::BasicAdaptiveMesh2d<3>;

// quadrilateral mesh
template class FadaMesh::BasicAdaptiveMesh2d<4>;
