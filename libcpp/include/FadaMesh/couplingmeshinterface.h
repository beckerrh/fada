#ifndef __FadaMesh_CouplingMeshInterface_h
#define __FadaMesh_CouplingMeshInterface_h

#include  "meshinterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class CouplingMeshInterface : public MeshInterface
  {
private:
protected:
public:
    ~CouplingMeshInterface();
    CouplingMeshInterface();
    CouplingMeshInterface( const CouplingMeshInterface& couplingmeshinterface);
    CouplingMeshInterface& operator=( const CouplingMeshInterface& couplingmeshinterface);
    std::string getName() const;
    CouplingMeshInterface* clone() const;

    // virtual void setNLevels(int nlevels) const = 0;
    virtual void computeCouplingSideInformation(int nlevels) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
