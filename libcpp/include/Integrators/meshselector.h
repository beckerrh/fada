#ifndef __Integrators_MeshSelector_h
#define __Integrators_MeshSelector_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshCompositionInterface;
}
namespace Fada
{
  class ModelInterface;
}
namespace Integrators
{
  class MeshSelector : public Alat::InterfaceBase
  {
private:
protected:
  std::string getInterfaceName() const
  {
    return "MeshSelector";
  }
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
