#ifndef __FadaMesh_MultiMeshCreator_h
#define __FadaMesh_MultiMeshCreator_h

#include "coarsener2d.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
  class MultiMeshCreator
  {  
protected:
   void _coarsen();
   Coarsener2d* _coarsener;
   /// fonction d'accés au pointer sur le maillage adaptatif
   AdaptiveMeshInterface* _getMesh();
   /// fonction d'accés à l'arbre des faces
   tree<FaceInterface*> & _getFaces();
   void _constructRefineInfo();
   
public:
  ~MultiMeshCreator();
  MultiMeshCreator(Coarsener2d* coarsener);
  MultiMeshCreator(const MultiMeshCreator& multimeshcreator);
  MultiMeshCreator& operator= (const MultiMeshCreator & multimeshcreator);    
  std::string getName() const;
  void createMultiMesh(std::string dirname, std::string datatype, int nlevels, int ncells);   
  };
}

/*--------------------------------------------------------------------------*/

#endif
