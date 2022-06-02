#include  "Alat/doublevector.h"
#include  "FadaMesh/basicadaptivemesh3d.h"
#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/hangingsideinfo.h"
#include  <algorithm>
#include  <fstream>
#include  <iterator>

using namespace FadaMesh;
using namespace FadaMesh;
using namespace std;


/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::basicInit(const std::string& type)
{
  _cell_map_ok=false;
  _numbering_ok=false;
}
/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE >
BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE >::BasicAdaptiveMesh3d()
  :AdaptiveMeshInterface(),FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE >(){}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::constructCellMap()
{
  if (_cell_map_ok) return;
  // Construction du map permettant de passer d'un numéro de feuille à la face correspondante
  _cellmap_fadalightmesh.clear();
  int count = 0;
  for(volume_leafpointer p =   _Volumes.begin_leaf(); p != _Volumes.end_leaf(); p++)
  {
    _cellmap_fadalightmesh[count++] = p;
  }
  _cell_map_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::constructNumbering()
{
  if (_numbering_ok) return;
  _volumeid2id.clear();
  _faceid2id.clear();
  _edgeid2id.clear();
  _nodeid2id.clear();
  //  _copy_edge.clear();
  int count_node=0;
  // on numérote les noeuds (dans le cas d'une procédure de déraffinement un noeud peut avoir une
  // identité supérieure au nombre de noeuds)
  for(NodeSet::iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
  {
    _nodeid2id[( *p )->id()] = count_node++;
  }
  // on numérote les feuilles de l'arbre des edges
  int count_edge = 0;
  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    _edgeid2id[( *p )->id()] = count_edge++;
  }
  // on numérote les feuilles de l'arbre des faces
  int count_face = 0;
  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    _faceid2id[( *p )->id()] = count_face++;
    //maillage adaptatif
    //    for(int ii = 0; ii < (*p)->NumberOfEdges(); ii++)
    //    {
    //      edge_pointer E = (*p)->edge(ii);
    //      if ( (*E)->Copy())
    //      {
    //        _copy_edge.insert(E);
    //        _edgeid2id[(*E)->id()]=count_edge++;
    //      }
    //    }
  }
  // on numérote les feuilles de l'arbre des volumes
  int count_volume = 0;
  for(volume_leafpointer p = _Volumes.begin_leaf(); p != _Volumes.end_leaf(); p++)
  {
    _volumeid2id[( *p )->id()] = count_volume++;


    //maillage adaptatif
    //    for(int ii = 0; ii < (*p)->NumberOfFaces(); ii++)
    //    {
    //      face_pointer F = (*p)->face(ii);
    //      if ( (*F)->Copy())
    //      {
    //        _copy_face.insert(F);
    //        _edgeid2id[(*F)->id()]=count_face++;
    //      }
    //    }
  }
  _numbering_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::writeAdaptiveMesh(std::string name, std::string datatype)
{
  // on met la mumérotation à jour

  constructCellMap();
  constructNumbering();

  // Ecriture des arbres constituants le maillage adaptatif
  if(datatype == "ascii")
  {
    std::cout<<"AdaptiveMesh::Write() writing  (fadalightmeshadaptive) "<<name<<" : "<<datatype<<"\n";
  }
  //! create directory
  string dirname = name+".fadalightmeshadaptive";
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
  {
      Alat::IntVector nodedata(2*_Nodes.size());
      Alat::DoubleVector nodecoor(3*_Nodes.size());
      int icount=0, xcount=0;
      for(set<Node*>::const_iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
      {
        nodedata[icount++]=(*p)->id();
        nodedata[icount++]=(*p)->oldid();
        nodecoor[xcount++]=(*p)->getNode().x();
        nodecoor[xcount++]=(*p)->getNode().y();
        nodecoor[xcount++]=(*p)->getNode().z();
      }
      nodedata.saveFada(file);
      nodecoor.saveFada(file);
  }
  file.close();
  //! write volumes
  filename = dirname+"/volumes";
  file.open( filename.c_str() );
  assert( file.is_open() );
  {
    Alat::IntVector volumedata(28*_Volumes.size());
    int icount=0;
    for(volume_pointer p = _Volumes.begin(); p != _Volumes.end(); p++)
    {
      volumedata[icount++]= (*p )->id();
      volumedata[icount++]= (*p )->oldid();
      for(int i=0;i<6;i++) volumedata[icount++]=(*((*p )->face(i)))->id();
      for(int i=0;i<12;i++) volumedata[icount++]=(*((*p )->edge(i)))->id();
      for(int i=0;i<8;i++) volumedata[icount++]=((*p )->node(i)->id());
    }
    volumedata.saveFada(file);
  }
  file.close();
  //! write faces
  filename = dirname+"/faces";
  file.open( filename.c_str() );
  assert( file.is_open() );
  {
    Alat::IntVector facedata(13*_Faces.size());
    int icount=0;
    for(face_pointer p = _Faces.begin(); p != _Faces.end(); p++)
    {
      facedata[icount++]= (*p )->id();
      facedata[icount++]= (*p )->oldid();
      facedata[icount++]= (*p )->boundaryid();
      for(int i=0;i<4;i++) facedata[icount++]=(*(*p )->edge(i))->id();
      for(int i=0;i<4;i++) facedata[icount++]=(*p )->node(i)->id();
      for(int i=0;i<2;i++) {if((*p )->volume(i)!=NULL) facedata[icount++]=(*(*p )->volume(i))->id(); else  facedata[icount++]=-1;}
    }
    facedata.saveFada(file);
  }

  file.close();
  //! write edges
  filename = dirname+"/edges";
  file.open( filename.c_str() );
  assert( file.is_open() );
  {
    Alat::IntVector edgedata(9*_Edges.size());
    int icount=0;
    for(edge_pointer p = _Edges.begin(); p != _Edges.end(); p++)
    {
      edgedata[icount++]= (*p )->id();
      edgedata[icount++]= (*p )->oldid();
      edgedata[icount++]= (*p )->nref;
      edgedata[icount++]= (*p )->boundaryid();
      edgedata[icount++]= (*p )->copy();
      for(int i=0;i<2;i++) edgedata[icount++]=(*p )->node(i)->id();
      for(int i=0;i<2;i++) edgedata[icount++]=-1;
    }
    edgedata.saveFada(file);
  }
  file.close();
  //!write volume_tree
  filename = dirname+"/volumeparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap volumeparents;
  for(volume_pointer p = _Volumes.begin(); p != _Volumes.end(); p++)
  {
    int ip =  ( *p )->id();
    if( _Volumes.depth(p) )
    {
      volumeparents[ip] = ( *_Volumes.parent(p) )->id();
    }
    else
    {
      volumeparents[ip] = -1;
    }
  }
  volumeparents.write(file, datatype);
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
  for(map<int, volume_pointer>::iterator p =  _cellmap_fadalightmesh.begin(); p !=  _cellmap_fadalightmesh.end(); p++)
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
  //!write _volumeid2id
  filename = dirname+"/volumeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _volumeid2id.write(file, datatype);
  file.close();
  //!write _curvedboundaries
  // filename = dirname+"/CurvedBoundaryInformation";
  // _curvedboundaries.write(filename, datatype);
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::readAdaptiveMesh(std::string name, std::string lastnamein)
{
  //! init _last_id_
  _last_node_id=-1;
  _last_edge_id=-1;
  _last_face_id=-1;
  _last_volume_id=-1;

  //! create directory name
  std::string inadaptive = name+".fadalightmesh/"+lastnamein;
  string dirname = inadaptive+".fadalightmeshadaptive";

  int n;
  string filename, datatype, volumename;
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
  _Edges.clear();
  _Faces.clear();
  _Volumes.clear();
  _copy_edge.clear();
  // read nodes
  filename = dirname+"/nodes";
  file.open( filename.c_str() );
  assert( file.is_open() );
   std::map<int, Node*> id2node;
  {
      Alat::IntVector nodedata;
      Alat::DoubleVector nodecoor;
      nodedata.loadFada(file);
      nodecoor.loadFada(file);
      int n=nodedata.size()/2;
      int icount=0, xcount=0;
      for(int i=0; i<n;i++)
      {
        Node* N = new Node;
        N->id()=nodedata[icount++];
        N->oldid()=nodedata[icount++];
        N->getNode().x()=nodecoor[xcount++];
        N->getNode().y()=nodecoor[xcount++];
        N->getNode().z()=nodecoor[xcount++];
        _last_node_id=std::max(N->id(),_last_node_id);
        _Nodes.insert(N);
        id2node[N->id()] = N;
      }
  }
  file.close();

  map<int, volume_pointer> id2volume;
  map<int, face_pointer> id2face;
  map<int, edge_pointer> id2edge;

  //  //! read edgeparents
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
    (*E).id()=p->first;
  }
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
    F = new Quad();
    if(p->second == -1)
    {
      id2face[p->first] = _Faces.insert(_Faces.end(), F);
    }
    else
    {
      map<int, face_pointer>::const_iterator q = id2face.find(p->second);
      assert( q != id2face.end() );
      id2face[p->first] = _Faces.append_child(q->second, F);
      assert(*(id2face[p->first]));
    }
  }

  //! read volumeparents
  filename = dirname+"/volumeparent";
  file.open( filename.c_str() );
  assert( file.is_open() );
  Alat::IntMap volumeparents;
  volumeparents.read(file);
  file.close();
  for(Alat::IntMap::const_iterator p = volumeparents.begin(); p != volumeparents.end(); p++)
  {
    VolumeInterface* V;
    V = new Hex;
    V->id()=p->first;
    if(p->second == -1)
    {
      id2volume[p->first] = _Volumes.insert(_Volumes.end(), V);
    }
    else
    {
      map<int, volume_pointer>::const_iterator q = id2volume.find(p->second);
      assert( q != id2volume.end() );
      id2volume[p->first] = _Volumes.append_child(q->second, V);
    }
  }
  //! read edges
  filename = dirname+"/edges";
  file.open( filename.c_str() );
  {
    Alat::IntVector edgedata;
    edgedata.loadFada(file);
    assert( file.is_open() );
    n=edgedata.size()/9;
    int ide;
    int nodenumber;
    int icount=0;
    for(int i = 0; i < n; i++)
    {
      ide=edgedata[icount++];
      assert(id2edge.find(ide)!=id2edge.end());
      edge_pointer& E = id2edge[ide];
      ( *E )->id() = ide;
      ( *E )->oldid() = edgedata[icount++];
      ( *E )->nref= edgedata[icount++];
      ( *E )->boundaryid()= edgedata[icount++];
      ( *E )->copy()= edgedata[icount++];
      nodenumber=edgedata[icount++];
      ( *E )->node(0)=id2node[nodenumber];
      nodenumber=edgedata[icount++];
      ( *E )->node(1)=id2node[nodenumber];
      icount++;icount++;
    }
    file.close();
  }
  for(edge_pointer p = _Edges.begin(); p != _Edges.end(); p++)
  {
    int ip =  ( *p )->id();
    _last_edge_id=std::max(ip,_last_edge_id);
  }

  //! read faces
  filename = dirname+"/faces";
  file.open( filename.c_str() );
  assert( file.is_open() );
  {
    Alat::IntVector facedata;
    facedata.loadFada(file);
    file.close();
    int icount=0;
    int idf, idfold, boundaryid;
    int ii,iv,vn;
    n=facedata.size()/13;
    for(int i = 0; i < n; i++)
    {
      idf=facedata[icount++];
      face_pointer& C = id2face[idf];
      Quad* Q = dynamic_cast<Quad*>( *C );
      idfold=facedata[icount++];
      boundaryid=facedata[icount++];
      Q->id() = idf;
      Q->oldid() = idfold;
      Q->boundaryid()=boundaryid;
      for(int iie=0;iie<4;iie++)
      {
        ii=facedata[icount++];
        assert(id2edge.find(ii)!=id2edge.end());
        Q->edge(iie) = id2edge.find(ii)->second;
        assert(*Q->edge(iie));
      }
      for(int iin=0;iin<4;iin++)
      {
        ii=facedata[icount++];
        assert(id2node.find(ii)!=id2node.end());
        Q->node(iin) = id2node.find(ii)->second;
        assert(Q->node(iin));
      }
      iv=0;
      vn=facedata[icount++];
      if(vn>=0) {Q->volume(iv) = id2volume.find(vn)->second;iv++;}
      vn=facedata[icount++];
      if(vn>=0) {Q->volume(iv) = id2volume.find(vn)->second;}
    }
  }

  file.close();
  for(face_pointer p = _Faces.begin(); p != _Faces.end(); p++)
  {
    // mise à jour de _last_face_id
    int ip =  ( *p )->id();
    _last_face_id=std::max(ip,_last_face_id);
  }
  //! read volumes

  filename = dirname+"/volumes";
  file.open( filename.c_str() );
  assert( file.is_open() );
  {
    Alat::IntVector volumedata;
    volumedata.loadFada(file);
    n=volumedata.size()/28;
    int icount=0;
    int idv,ii;
    for(int i = 0; i < n; i++)
    {
      idv=volumedata[icount++];
      volume_pointer& C = id2volume[idv];
      Hex* H = dynamic_cast<Hex*>( *C );
      H->id()=idv;
      H->oldid()=volumedata[icount++];
      for(int i=0;i<6;i++)
      {
        ii=volumedata[icount++];
        H->face(i) = id2face.find(ii)->second;
        assert(*(H->face(i)));
      }
      for(int i=0;i<12;i++)
      {
        ii=volumedata[icount++];
        H->edge(i)= id2edge.find(ii)->second;
        assert(*(H->edge(i)));
      }
      for(int i=0;i<8;i++)
      {
        ii=volumedata[icount++];
        H->node(i) = id2node.find(ii)->second;
        assert(H->node(i));
      }
    }
  }

  file.close();

  for(volume_pointer p = _Volumes.begin(); p != _Volumes.end(); p++)
  {
    int ip =  ( *p )->id();
    _last_volume_id=std::max(ip,_last_volume_id);
  }
  //  //!read _curvedboundaries
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
    _cellmap_fadalightmesh[p->first] = id2volume[p->second];
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
  //!read _volumeid2id
  filename = dirname+"/volumeid2id";
  file.open( filename.c_str() );
  assert( file.is_open() );
  _volumeid2id.read(file);
  file.close();


  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::readFadaMesh(name);
  _cell_map_ok=true;
  _numbering_ok=true;
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::readFadaMeshAndInitTrees(std::string name)
{
  // lecture du FadaMesh
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::readFadaMesh(name);
  //lecture des CurvedBoundaryInfo
  //  if(FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::GeometryObjectExists("CurvedBoundaryInformation") )
  //  {
  //    const FadaMesh::GeometryObject* GO = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getGeometryObject("CurvedBoundaryInformation");
  //    const FadaMesh::CurvedBoundaryInformation* BD = dynamic_cast<const FadaMesh::CurvedBoundaryInformation*>( GO );
  //    assert(BD);
  //    _curvedboundaries.set_size(*BD);
  //  }
  //map provisoires pour connectivités
  map<int, Node*> id2node;
  map<int, edge_pointer> id2edge;
  map<int, face_pointer> id2face;
  //construction des nodes
  for(int iN = 0; iN < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNNodes(); iN++)
  {
    const Alat::Node& v = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNode(iN);
    Node* np = new Node( iN, v );
    _Nodes.insert(np);
    id2node[iN] = np;
  }
  //construction des volumes et du cellmap
  for(int iK = 0; iK < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNCells(); iK++)
  {
    VolumeInterface* V = _newVolume(iK);
    for(int ii = 0; ii < getNNodesPerCell(); ii++)
    {
      V->node(ii) = id2node[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNodeIdOfCell(iK, ii)];
    }
    _cellmap_fadalightmesh[iK] = _Volumes.insert(_Volumes.end(), V);
  }
  //construction des faces
  for(int iS = 0; iS < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNSides(); iS++)
  {
    FaceInterface* F = _newFace(iS);
    for(int ii = 0; ii < getNNodesPerSide(); ii++)
    {
      F->node(ii) = id2node[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNodeIdOfSide(iS, ii)];
    }
    id2face[iS] = _Faces.insert(_Faces.end(), F);
  }

  //construction des edges
  for(int iE = 0; iE < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNEdges(); iE++)
  {
    Edge* E = new Edge;
    E->id() = iE;
    for(int ii = 0; ii < getNNodesPerEdge(); ii++)
    {
      E->node(ii) = id2node[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNodeIdOfEdge(iE, ii)];
    }
    id2edge[iE] = _Edges.insert(_Edges.end(), E);
  }

  // remplissage des informations manquantes :
  // les edges et faces composant chaque volume:
  for(int iK = 0; iK < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNCells(); iK++)
  {
    VolumeInterface* V = *_cellmap_fadalightmesh[iK];
    for(int ii = 0; ii <getNSidesPerCell(); ii++)
    {
      V->face(ii) = id2face[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSideIdOfCell(iK, ii)];
    }
    for(int ii = 0; ii <getNEdgesPerCell(); ii++)
    {
      V->edge(ii) = id2edge[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgeIdOfCell(iK, ii)];
    }
  }
  // les edges composant chaque face et les volumes de part et d'autre
  for(int iS = 0; iS < FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNSides(); iS++)
  {
    FaceInterface* F = *id2face[iS];
    for(int ii = 0; ii < 2; ii++)
    {
      int iK = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellIdOfSide(iS, ii);
      if(iK >= 0)
      {
        F->volume(ii) = _cellmap_fadalightmesh[iK];
      }
    }
    for(int ii = 0; ii <getNEdgesPerSide(); ii++)
    {
      F->edge(ii) = id2edge[FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgeIdOfSide(iS, ii)];
    }
  }

  // les boundary info pour chaque face
  const FadaMesh::BoundaryInfo* BI = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getBoundaryInfo();
  const Alat::IntVector colors = BI->getColors();
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& faces = BI->getSidesOfColor(color);
    for(int i = 0; i < faces.size(); i++)
    {
      int iS = faces[i];
      FaceInterface* F = *id2face[iS];
      F->boundaryid() = color;
    }
  }
  // on met à jour les numérotations
  _last_node_id=FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNNodes()-1;
  _last_edge_id=FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNEdges()-1;
  _last_face_id=FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNSides()-1;
  _last_volume_id=FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getNCells()-1;

  constructNumbering();
  constructCellMap();
}

/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::writeFadaMesh(std::string name, std::string datatype)
{
  // Reinitialisation du FadaMesh à partir des feuilles de l'arbre
  reInitFadaMesh();
  // Mise à jour des information sur les bords courbes
  // FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::reInit();
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCurvedBoundaryInformation()->constructBoundaryInformation(this);
  // Ecriture du FadaMesh
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::writeFadaMesh(name,datatype);
}



/*--------------------------------------------------------------------------*/
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::reInitFadaMesh()
{
  // on remet à jour le maillage FadaMesh à partir des arbres
  // si besoin on met à jour la numérotation
  constructNumbering();
  constructCellMap();
  int _n_leaf_faces= _faceid2id.size();
  int _n_leaf_edges= _edgeid2id.size();
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCells().set_size( _volumeid2id.size() );
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSidesOfCells().set_size( _volumeid2id.size() );
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSides().set_size(_n_leaf_faces);
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides().set_size(_n_leaf_faces);
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgesOfSides().set_size(_n_leaf_faces);
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdges().set_size(_n_leaf_edges);
  FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgesOfCells().set_size( _volumeid2id.size());
  // remplissage du tableau des nodes
  Alat::Vector<Alat::Node>& nodes = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getAllNodes();
  nodes.set_size( _Nodes.size() );
  for(set<Node*>::iterator p = _Nodes.begin(); p != _Nodes.end(); p++)
  {
    int id = _nodeid2id[(*p)->id()];
    nodes[id] = ( *p )->getNode();
  }
  // les edges
  for(edge_leafpointer p = _Edges.begin_leaf(); p != _Edges.end_leaf(); p++)
  {
    Edge* E = *p;
    int id = _edgeid2id[E->id()];
    for(int ii = 0; ii < getNNodesPerEdge(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdges()[id][ii] = _nodeid2id[E->node(ii)->id()];
    }
  }
  // Les cells avec les connectivités SidesOfCells et EdgesOfCells

  for(volume_leafpointer p = _Volumes.begin_leaf(); p != _Volumes.end_leaf(); p++)
  {
    VolumeInterface* V = *p;
    int id = _volumeid2id[V->id()];
    for(int ii = 0; ii < getNNodesPerCell(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCells()[id][ii] = _nodeid2id[V->node(ii)->id()];
    }

    for(int ii = 0; ii < getNSidesPerCell(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSidesOfCells()[id][ii] = _faceid2id[((*V->face(ii)) )->id()];
    }

    for(int ii = 0; ii < getNEdgesPerCell(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgesOfCells()[id][ii] = _edgeid2id[((*V->edge(ii)) )->id()];
    }
  }
  // les sides avec les connectivités CellsOfSides et EdgesOfSides



  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    FaceInterface* F = *p;
    int id = _faceid2id[F->id()];
    for(int ii = 0; ii < getNNodesPerSide(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSides()[id][ii] = _nodeid2id[F->node(ii)->id()];
    }
    for(int ii = 0; ii < getNEdgesPerSide(); ii++)
    {
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgesOfSides()[id][ii] = _edgeid2id[ (*(F->edge(ii)) )->id()];
    }

    for(int ii = 0; ii < 2; ii++)
    {
      if(F->volume(ii) != NULL)
      {
        VolumeInterface* V = *F->volume(ii);
        FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = _volumeid2id[V->id()];
      }
      else
      {
        FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = -1;// il s'agit d'un side sur le bord
      }
    }
    if(FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][0] < 0)
    {
      int temp = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][0];
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][0] = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][1];
      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][1] = temp;
    }

  }

  //  A voir pour adaptation!
  // les faces recopiées font parties des feuilles, on les traite également
  //    for(std::set<face_pointer>::iterator it = _copy_face.begin(); it != _copy_face.end(); it++)
  //    {
  //      FaceInterface* F = *( *it );
  //      int id = _faceid2id[F->id()];
  //      for(int ii = 0; ii < getNNodesPerSide(); ii++)
  //      {
  //        FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getSides()[id][ii] = _nodeid2id[F->node(ii)->id()];
  //      }
  //      for(int ii = 0; ii < getNEdgesPerSide(); ii++)
  //      {
  //        FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdgesOfSides()[id][ii] = _edgeid2id[( F->edge(ii) )->id()];
  //      }
  //      for(int ii = 0; ii < 2; ii++)
  //      {
  //        if(F->volume(ii) != NULL)
  //        {
  //          VolumeInterface* V = *F->volume(ii);
  //          if( _volumeid2id.find( V->id() ) != _volumeid2id.end() )
  //          {
  //            FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = _volumeid2id[F->id()];
  //          }
  //          else
  //          {
  //            FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = -2;// il s'agit du grand side d'un HangingSide
  //          }
  //        }
  //        else
  //        {
  //          if(F->boundaryid() == -1)
  //          {
  //            FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = -2;// il s'agit du grand side d'un HangingSide
  //          }
  //          else
  //          {
  //            FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCellsOfSides()[id][ii] = -1;// il s'agit d'un side sur le bord
  //          }
  //        }
  //      }
  //    }


  // A voir pour adaptation!!
  // les edges recopiés font partie des feuilles, on les traite également
  //  for(std::set<edge_pointer>::iterator it = _copy_edge.begin(); it != _copy_edge.end(); it++)
  //  {
  //    Edge* E = *( *it );
  //    int id = _edgeid2id[E->id()];
  //    for(int ii = 0; ii < getNNodesPerEdge(); ii++)
  //    {
  //      FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getEdges()[id][ii] = _nodeid2id[E->node(ii)->id()];
  //    }
  //  }
  //boundary info
  FadaMesh::BoundaryInfo* BI = FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getBoundaryInfo();
  std::map<int, int> size_of_color;

  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    FaceInterface*  F = *p;
    int color = F->boundaryid();
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
  for(face_leafpointer p = _Faces.begin_leaf(); p != _Faces.end_leaf(); p++)
  {
    FaceInterface* F = *p;
    int color = F->boundaryid();
    if(color != -1)
    {
      BI->getSidesOfColor(color)[size_of_color[color]] = _faceid2id[F->id()];
      if (F->volume(0)==NULL)
      {
        std::cerr<<"F nodes"<<'\n';
        std::cerr<<(F->node(0))->getNode()<<" "<<(F->node(1))->getNode()<<" "<<(F->node(2))->getNode()<<" "<<(F->node(3))->getNode()<<'\n';
      }
      assert(F->volume(0) != NULL);
      VolumeInterface* V = *F->volume(0);
      assert(V);
      BI->getCellsOfColor(color)[size_of_color[color]] = _volumeid2id[V->id()];
      for(int ii = 0; ii < getNSidesPerCell(); ii++)
      {
        if(*V->face(ii) == F)
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
template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
void BasicAdaptiveMesh3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::updateHangingInfo()
{
  return;
  // // Mise à jour des informations sur les HangingNodes et Side

  //// mise à jour des numérotations et du cell map
  //  constructNumbering();
  //  constructCellMap();
  //  typedef std::map<face_pointer, std::pair<VolumeInterface*, int>, FaceCompare> NewHangingInfo;
  //  NewHangingInfo _hangingfaces;
  //  for(volume_leafpointer v = _Volumes.begin_leaf(); v != _Volumes.end_leaf(); v++)
  //  {
  //    VolumeInterface* V = *v;
  //    for(int ii = 0; ii < V->NumberOfFaces(); ii++)
  //    {
  //      face_pointer F = V->face(ii);
  //      // seuls les faces recopiés sont des hanging faces
  //      if ((*F)->Copy()) {
  //        _hangingfaces.insert( make_pair( F, make_pair(V, ii) ) );
  //      }
  //    }
  //  }
  //  int n_hanging=_hangingfaces.size();


  //  ****
  //  ICI
  //  ****


  //  int n_local_data_hnode=3; //cas 2D on stocke, le cell_id, le localcell_id, et le noeud_id
  //  int n_local_data_hside=4; //cas 2D on stocke, le cell_id, le localcell_id, et les deux side_id
  //  createGeometryObject("HangingNodeInfo");
  //  createGeometryObject("HangingSideInfo");
  //// on récupère les objets géométriques pour pouvoir les remplir
  //  FadaMesh::HangingNodeInfo* hnodes= dynamic_cast<FadaMesh::HangingNodeInfo*>(FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getGeometryObject("HangingNodeInfo"));
  //  FadaMesh::HangingSideInfo* hsides= dynamic_cast<FadaMesh::HangingSideInfo*>(FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getGeometryObject("HangingSideInfo"));
  //  hnodes->set_size(n_hanging, n_local_data_hnode);
  //  hsides->set_size(n_hanging, n_local_data_hside);
  //  if (n_hanging==0) return;
  //  int count=0;
  //// remplissage des objets géométriques
  //  for(map<edge_pointer, std::pair<FaceInterface*, int> >::iterator p = _hangingedges.begin(); p != _hangingedges.end(); p++)
  //  {
  //    edge_pointer E = p->first;
  //    FaceInterface* F = p->second.first;
  //    int icell= _faceid2id[F->id()];
  //    int local_side_id = p->second.second;
  //    hsides->getCellNumber(count)=icell;
  //    hnodes->getCellNumber(count)=icell;
  //    hsides->getLocalSide(count)=local_side_id;
  //    hnodes->getLocalSide(count)=local_side_id;
  //    assert(_Edges.number_of_children(E)==2);
  //    edge_pointer EC = _Edges.child(E, 0);
  //    int node_id= _nodeid2id[( *EC )->node(1)->id()];
  //    hnodes->getHangingNodes(count,0)= node_id;

  //    for(int ii = 0; ii < _Edges.number_of_children(E); ii++)
  //    {
  //      edge_pointer EC = _Edges.child(E, ii);
  //      int edge_id= _edgeid2id[( *EC )->id()];
  //      hsides->getHangingSides(count,ii)=edge_id;
  //    }
  //    count++;
  //  }
}
/*---------------------------------------------------------*/
// hexahedral mesh
template class FadaMesh::BasicAdaptiveMesh3d<8,6,12,4>;

// tetrahedral mesh
template class FadaMesh::BasicAdaptiveMesh3d<4,4,6,3>;
