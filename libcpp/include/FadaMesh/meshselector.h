#ifndef __Integrators_MeshSelector_h
#define __Integrators_MeshSelector_h

#include  "Alat/interfacebase.h"
#include  "FadaMesh/fadameshenums.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ModelInterface;
}
namespace FadaMesh
{
  class MeshInterface;

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
    FadaMesh::MeshInterface* newMesh(FadaMeshEnums::meshtype& meshtype, std::string meshnamebase) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
