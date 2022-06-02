#ifndef __FadaMesh_MeshSelectorInterface_h
#define __FadaMesh_MeshSelectorInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshSelectorInterface : public Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const
    {
      return "MeshSelectorInterface";
    }

public:
    ~MeshSelectorInterface();
    MeshSelectorInterface();
    MeshSelectorInterface( const MeshSelectorInterface& meshselectorinterface);
    MeshSelectorInterface& operator=( const MeshSelectorInterface& meshselectorinterface);
    std::string getName() const
    {
      return "MeshSelectorInterface";
    }
  };
}

/*--------------------------------------------------------------------------*/

#endif
