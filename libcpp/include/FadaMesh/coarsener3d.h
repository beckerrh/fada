#ifndef __FadaMesh_Coarsener3d_h
#define __FadaMesh_Coarsener3d_h

#include "basicadaptor.h"
/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Coarsener3d: public BasicAdaptor
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
   void _markCellToCoarsen(volume_pointer f);
/// fonction de déraffinement
   void _coarsen();
/// map des objets de maillage à éliminer pour déraffiner le maillage
   typedef std::map<volume_pointer, std::vector<int>, VolumeCompare> CoarseMap;
   CoarseMap _nodecoarse, _edgecoarse, _facecoarse, _volumecoarse ;
   
   

public:
   /// constructeur : on spécialise ici l'outil de refinement des faces en fonction
   /// du type de maillage
    Coarsener3d(AdaptiveMeshInterface* mesh): BasicAdaptor(mesh){}
    std::string getName() const
    {
      return "Coarsener3d";
    }
    const volume_pointer _neighbour(const volume_pointer V, const face_pointer F)
    {
      if( ( *F )->volume(0) != V )
      {
        return ( *F )->volume(0);
      }
      else
      {
        return ( *F )->volume(1);
      }
    }
    /// fonction de d'adaptation du maillage (dérafinement ici des mailles marquées)
    void adaptMesh(std::string filename);
    void globalCoarsen();
    CoarseMap& getVolumeCoarse(){return _volumecoarse;}
    CoarseMap& getFaceCoarse(){return _facecoarse;}
    CoarseMap& getEdgeCoarse(){return _edgecoarse;}
    CoarseMap& getNodeCoarse(){return _nodecoarse;}
    AdaptiveMeshInterface*  getMesh(){return _adaptive_mesh;}
    
  };
}

/*--------------------------------------------------------------------------*/

#endif
