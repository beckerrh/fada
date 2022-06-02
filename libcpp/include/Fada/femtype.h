#ifndef  __Fada_femType_h
#define  __Fada_femType_h

#include <string>

/*---------------------------------------------*/    
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  bool femType(std::string& femout, const std::string& femin, const FadaMesh::MeshInterface* mesh);
}

#endif
