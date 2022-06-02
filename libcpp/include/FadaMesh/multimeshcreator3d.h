#ifndef __FadaMesh_MultiMeshCreator3d_h
#define __FadaMesh_MultiMeshCreator3d_h

#include  "coarsener3d.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
  class MultiMeshCreator3d
  {  
protected:

   void _coarsen();
   Coarsener3d* _coarsener;
   /// fonction d'accés au pointer sur le maillage adaptatif
   AdaptiveMeshInterface* _getMesh(){return _coarsener->getMesh();}
   /// fonction d'accés à l'arbre des faces
   tree<VolumeInterface*> & _getVolumes(){return _coarsener->getVolumes();}
   void _constructRefineInfo();
public:
    MultiMeshCreator3d(Coarsener3d* c): _coarsener(c){}
    
    std::string getName() const
    {
      return "MultiMeshCreator3d";
    }
   void createMultiMesh(std::string dirname, std::string datatype, int nlevels, int ncells);
   
   
  };
}

/*--------------------------------------------------------------------------*/

#endif
