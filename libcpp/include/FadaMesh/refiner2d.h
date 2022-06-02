#ifndef __FadaMesh_Refiner2d_h
#define __FadaMesh_Refiner2d_h

#include "basicadaptor.h"
#include  "trirefiner.h"
#include  "quadrefiner.h"
/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Refiner2d: public BasicAdaptor
  {

protected:
/// construction des informations sur l'adaptation
/// on precise dans un objet de type Fada::RefineInfo pour chaque maille raffinée
/// les nouveaux faces, edges et nodes produits par le raffinement
/// Ces informations sont nécessaires pour effectuer des interpolations
   void _constructAdaptInfo();
/// fonction permettant de rendre le maillage régulier
   void _makeRegular();
   void _updateCopyEdge();
/// fonction de marquage des mailles à raffiner
   void _markCellToRefine(const face_pointer F);
/// fonction de raffinement
   void _refine();
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
/// fonction de raffinement en quadtotri
   // void _refineQuadToTri();
public:
   /// constructeur : on spécialise ici l'outil de refinement des faces en fonction
   /// du type de maillage
    Refiner2d(AdaptiveMeshInterface* mesh): BasicAdaptor(mesh)
    {
        if(_adaptive_mesh->getName() == "FadaMesh::QuadrilateralMesh")
        {
           _facerefiner = new QuadRefiner;
        }
        else if(_adaptive_mesh->getName() == "FadaMesh::TriangleMesh")
        {
           _facerefiner = new TriRefiner;
        }
        else
        {
           std::cerr<<"Type of refiner unknown: "<<_adaptive_mesh->getName()<<'\n';
           assert(0);
        }
    }
    std::string getName() const
    {
      return "Refiner2d";
    }
    /// fonction de raffinement global de maillage
    void globalRefine(int nref);
    /// fonction de d'adaptation du maillage (rafinement ici des mailles marquées)
    void adaptMesh(std::string filename);
  };
}

/*--------------------------------------------------------------------------*/

#endif
