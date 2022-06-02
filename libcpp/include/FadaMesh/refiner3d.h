#ifndef __FadaMesh_Refiner3d_h
#define __FadaMesh_Refiner3d_h

#include "basicadaptor.h"
#include  "hexrefiner.h"
#include  "quadrefiner.h"
/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Refiner3d: public BasicAdaptor
  {
protected:
/// construction des informations sur l'adaptation
/// on precise dans un objet de type Fada::RefineInfo pour chaque maille raffinée
/// les nouveaux volumes, faces, edges et nodes produits par le raffinement
/// Ces informations sont nécessaires pour effectuer des interpolations
   void _constructAdaptInfo();
/// fonction permettant de rendre le maillage régulier
   void _makeRegular();
   void _updateCopyEdge();
   void _updateCopyFace();
/// fonction de marquage des mailles à raffiner
   void _markCellToRefine(const volume_pointer V);
/// fonction de raffinement
   void _refine();
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

public:
   /// constructeur : on spécialise ici l'outil de refinement des faces en fonction
   /// du type de maillage
    Refiner3d(AdaptiveMeshInterface* mesh): BasicAdaptor(mesh)
    {
        if(_adaptive_mesh->getName() == "FadaMesh::HexahedralMesh")
        {
           _volumerefiner = new HexRefiner;
           _facerefiner = new QuadRefiner;
        }
        else
        {
           std::cerr<<"Type of refiner unknown: "<<_adaptive_mesh->getName()<<'\n';
           assert(0);
        }
    }
    std::string getName() const
    {
      return "Refiner3d";
    }
    /// fonction de raffinement global de maillage
    void globalRefine(int nref);
    /// fonction de d'adaptation du maillage (rafinement ici des mailles marquées)
    void adaptMesh(std::string filename);
  };
}

/*--------------------------------------------------------------------------*/

#endif
