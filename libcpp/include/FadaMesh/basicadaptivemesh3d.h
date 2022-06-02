#ifndef __FadaMesh_BasicAdaptiveMesh3d_h
#define __FadaMesh_BasicAdaptiveMesh3d_h

#include  "Alat/vector.h"
#include  "curvedboundaryinformation.h"
#include  "fadalightmeshbase3d.h"
#include  "adaptivemeshinterface.h"
#include  "hex.h"
#include  "edge.h"
#include  "Alat/node.h"
#include  "quad.h"
#include  "setdefs.h"
#include  "tri.h"
#include  "typedefs.h"
#include  <map>

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  template<int NODESPERCELL,int SIDESPERCELL,int EDGESPERCELL,int NODESPERSIDE>
  class BasicAdaptiveMesh3d : virtual public AdaptiveMeshInterface, virtual public FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>
  {
protected:
    /// arbre des cells
    tree<VolumeInterface*> _Volumes;
    /// arbre des faces
    tree<FaceInterface*> _Faces;
    /// arbre des Edges
    tree<Edge*> _Edges;
    /// ensemble des noeuds
    NodeSet _Nodes;
    /// information pour les bords courbes
    // FadaMesh::CurvedBoundaryInformation _curvedboundaries;
    /// map affectant à chaque feuille de l'arbre des volumes un numéro de feuille
    Alat::IntMap _volumeid2id;
    /// map affectant à chaque feuille de l'arbre des faces un numéro de feuille
    Alat::IntMap _faceid2id;
    /// map affectant à chaque feuille de l'arbre des edges un numéro de feuille
    Alat::IntMap _edgeid2id;
    /// map permettant de renuméroter de manière continue, les nodes du maillages
    /// important lorsque des noeuds ont disparu lors d'une procédure de déraffinement
    Alat::IntMap _nodeid2id;
    /// map donnant un pointer sur un volume en fonction du numéro de cell dans
    /// le maillage FadaMesh
    std::map<int, volume_pointer> _cellmap_fadalightmesh;
    /// le maillage FadaMesh
    std::map<int, face_pointer> _dummycellmap;

    /// dernière identité de node utilisée
    int _last_node_id;
    /// dernière identité de edge utilisée
    int _last_edge_id;
    /// dernière identité de face utilisée
    int _last_face_id;
    /// dernière identité de cell utilisée
    int _last_volume_id;
    /// booleéns indiquant si les map de numérotation et le cell_map sont à jour
    bool _cell_map_ok, _numbering_ok;

    /// Ensemble des hanging edges
    EdgeSet _copy_edge;
    /// Ensemble des hanging faces
    EdgeSet _copy_Face;

protected:
    /// fonction renvoyant la cellule voisine de V par rapport à la face F
    const volume_pointer _neighbour(const volume_pointer V, const face_pointer F)
    {
      if( ( *F )->volume(0) != V )
      {
        return ( (*F) )->volume(0);
      }
      else
      {
        return ( (*F) )->volume(1);
      }
    }

    /// fonction de création d'un volume
    /// utile dans le cas de raffinement mais aussi lors de la création de l'arbre des volumes
    VolumeInterface* _newVolume(int iK)
    {
      if(getName() =="FadaMesh::HexahedralMesh")
      {
        return new Hex(iK);
      }
      assert(0);
    }
    FaceInterface* _newFace(int iS)
    {
      if(getName() =="FadaMesh::HexahedralMesh")
      {
        return new Quad(iS);
      }
      assert(0);
    }

public:
    /// constructeur sans arguments
    BasicAdaptiveMesh3d();

    /// Initialisation
    void basicInit(const std::string& t);
    /// Nom du maillage en fonction du paramètre NODESPERCELLS
    std::string getName() const
    {
      if(NODESPERCELL == 8)
      {
        return "FadaMesh::HexahedralMesh";
      }
      else
      {
        assert(0);
      }
      return "BasicAdaptiveMesh3d";
    }

    /// Procédure de lecture du maillage adaptatif (arbre et numérotations)
    void readAdaptiveMesh(std::string name, std::string last_in);
    /// Procédure d'écriture du maillage adaptatif (arbre et numérotations)
    void writeAdaptiveMesh(std::string name, std::string datatype = "binary");
    /// Lecture d'un maillage de type FadaMesh et initialisation des arbres
    void readFadaMeshAndInitTrees(std::string name);
    /// Lecture écriture du maillage au format FadaMesh
    void writeFadaMesh(std::string name, std::string datatype);
    /// Mise à jour des map de numérotation
    void constructNumbering();
    /// Mise à jour du cell_map
    void constructCellMap();
    /// Mise à jour des informations sur les hanging nodes et sides
    void updateHangingInfo();
    /// Mise à jour du maillage FadaMesh
    void reInitFadaMesh();

    /// Nombre de nodes par cell
    int getNNodesPerCell()
    {
      return NODESPERCELL;
    }

    /// Nombre de sides par cells
    int getNSidesPerCell()
    {
      return SIDESPERCELL;
    }

    /// Nombre de edges par cells
    int getNEdgesPerCell()
    {
      return EDGESPERCELL;
    }

    /// Nombre de nodes par side
    int getNNodesPerSide()
    {
      return NODESPERSIDE;
    }
    /// Nombre de nodes par edge
    int getNNodesPerEdge()
    {
      return 2;
    }
    /// Nombre de nodes par edge
    int getNEdgesPerSide()
    {
      return NODESPERSIDE;
    }


    /// Renvoie l'ensemble des nodes
    NodeSet& getNodes()
    {
      return _Nodes;
    }

    /// Renvoie l'arbre des edges
    tree<Edge*>& getEdges()
    {
      return _Edges;
    }

    /// Renvoie l'arbre des faces
    tree<FaceInterface*>& getFaces()
    {
      return _Faces;
    }

    /// Renvoie l'arbre des cells
    tree<VolumeInterface*>& getVolumes()
    {
      return _Volumes;
    }

    /// Renvoie les informations sur les bords courbes
    const FadaMesh::CurvedBoundaryInformation* getCurvedBoundaries()
    {
      return FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getCurvedBoundaryInformation();
      // return _curvedboundaries;
    }

    /// renvoie la dernière identité de node utilisée
    int& getLastNodeId()
    {
      return _last_node_id;
    }

    /// renvoie la dernière identité d'edge utilisée
    int& getLastEdgeId()
    {
      return _last_edge_id;
    }

    /// renvoie la dernière identité de face utilisée
    int& getLastFaceId()
    {
      return _last_face_id;
    }

    /// renvoie la dernière identité de face utilisée
    int& getLastVolumeId()
    {
      return _last_volume_id;
    }

    /// renvoie le map de renumérotation des nodes
    Alat::IntMap& getNodeId2Id()
    {
      return _nodeid2id;
    }

    /// renvoie le map de renumérotation des edges actifs (feuilles)
    Alat::IntMap& getEdgeId2Id()
    {
      return _edgeid2id;
    }

    /// renvoie le map de renumérotation des faces actives (feuilles)
    Alat::IntMap& getFaceId2Id()
    {
      return _faceid2id;
    }

    /// renvoie le map de renumérotation des faces actives (feuilles)
    Alat::IntMap& getVolumeId2Id()
    {
      return _volumeid2id;
    }

    /// renvoie le cell_map
    std::map<int, volume_pointer>&  getCellMap3d()
    {
      return _cellmap_fadalightmesh;
    }
    /// renvoie le dummy cell_map pour compatibilité
    std::map<int, face_pointer>&  getCellMap2d()
    {
      return _dummycellmap;
    }
    // void setQuadToTri(bool quadtotri)
    // {
    //   quadtotri=false;
    // }
    //
    // void setQuadToTriFileName(std::string quadtotriname)
    // {
    //   return;
    // }
    //
    // bool quadToTri()
    // {
    //   return false;
    // }
    //
    // std::string getQuadToTriFileName()
    // {
    //   return "none";
    // }

    /// écriture au format vtk pour visualisation
    void writeVtk(std::string name)
    {
      FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::writeVtk(name);
    }

    // /// écriture au format Ensight pour visualisation
    // void writeEnsightGeometry(std::string name)
    // {
    //   FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::writeEnsightGeometry(name);
    // }

    /// renvoie l'indicateur de mise à jour du cell_map
    bool&  getCellMapOk()
    {
      return _cell_map_ok;
    }

    /// renvoie l'indicateur de mise à jour des numérotations
    bool&  getNumberingOk()
    {
      return _numbering_ok;
    }

    /// Récriture de la fonction pour permettre son appel dans les outils d'adaptation
    FadaMesh::GeometryObject* getGeometryObject(std::string name)
    {
      return FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::getGeometryObject(name);
    }

    /// Récriture de la fonction pour permettre son appel dans les outils d'adaptation
    bool geometryObjectExists(std::string name) const
    {
      return FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::geometryObjectExists(name);
    }

    /// Récriture de la fonction pour permettre son appel dans les outils d'adaptation
    void createGeometryObject(std::string name)
    {
      FadaMesh::FadaMeshBase3d<NODESPERCELL,SIDESPERCELL,EDGESPERCELL,NODESPERSIDE>::createGeometryObject(name);
    }

  };
  typedef BasicAdaptiveMesh3d<8,6,12,4> HexahedralAdaptiveMesh;
}

/*--------------------------------------------------------------------------*/

#endif
