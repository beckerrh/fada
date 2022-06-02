#ifndef __FadaMesh_Coarsener2d_h
#define __FadaMesh_Coarsener2d_h

#include "basicadaptor.h"
/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Coarsener2d: public BasicAdaptor
  {  
protected:
/// construction des informations sur l'adaptation
/// on precise dans un objet de type Fada::RefineInfo pour chaque maille raffinée
/// les nouveaux faces, edges et nodes produits par le raffinement
/// Ces informations sont nécessaires pour effectuer des interpolations
   void _constructAdaptInfo();
/// fonction permettant de rendre le maillage régulier
   void _makeRegular();
/// fonction de marquage des mailles à raffiner
   void _markCellToCoarsen(face_pointer f);
/// fonction de déraffinement
   void _coarsen();
   void _updateCopyEdge();
   void _updateIds();
/// map des objets de maillage à éliminer pour déraffiner le maillage
   typedef std::map<face_pointer, std::vector<int>, FaceCompare> CoarseMap;
   // std::map<face_pointer, Alat::IntVector, FaceCompare> _edgecoarse, _facecoarse, _nodecoarse;
   CoarseMap _edgecoarse, _facecoarse, _nodecoarse;

public:
   /// constructeur : on spécialise ici l'outil de refinement des faces en fonction
   /// du type de maillage
    Coarsener2d(AdaptiveMeshInterface* mesh): BasicAdaptor(mesh){}
    std::string getName() const
    {
      return "Coarsener2d";
    }
    const face_pointer _neighbour(const face_pointer F, const edge_pointer E)
    {
      if( ( *E )->face(0) != F )
      {
        return ( *E )->face(0);
      }
      else
      {
        return ( *E )->face(1);
      }
    }
    /// fonction de d'adaptation du maillage (dérafinement ici des mailles marquées)
    void adaptMesh(std::string filename);
    void globalCoarsen();
    CoarseMap& getEdgeCoarse(){return _edgecoarse;}   
    CoarseMap& getFaceCoarse(){return _facecoarse;}   
    CoarseMap& getNodeCoarse(){return _nodecoarse;}     
    AdaptiveMeshInterface*  getMesh(){return _adaptive_mesh;}    
  };
}

/*--------------------------------------------------------------------------*/

#endif
