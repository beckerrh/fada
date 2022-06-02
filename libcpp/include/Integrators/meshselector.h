#ifndef __Integrators_MeshSelector_h
#define __Integrators_MeshSelector_h

#include  "FadaMesh/meshselectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshCompositionInterface;
}
namespace Fada
{
  class ModelManagerInterface;
}
namespace Integrators
{
  class MeshSelector : public FadaMesh::MeshSelectorInterface
  {
private:
protected:
public:
    ~MeshSelector();
    MeshSelector();
    MeshSelector( const MeshSelector& meshselector);
    MeshSelector& operator=( const MeshSelector& meshselector);
    std::string getName() const;
    FadaMesh::MeshCompositionInterface* newMeshComposition(std::string meshtype, std::string meshnamebase) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
