#ifndef __FadaMesh_ConstructAdaptiveMeshAndAdaptor_h
#define __FadaMesh_ConstructAdaptiveMeshAndAdaptor_h

#include  <string>
#include  "adaptivemeshinterface.h"
#include  "meshadaptorinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class ConstructAdaptiveMeshAndAdaptor
  {
  public:

    ConstructAdaptiveMeshAndAdaptor(AdaptiveMeshInterface*& M, MeshAdaptorInterface*& AM, const std::string& meshname, const std::string& adaption_type="refine");
    std::string getName() const {return "ConstructAdaptiveMeshAndAdaptor";}
  };
}

/*--------------------------------------------------------------------------*/

#endif
