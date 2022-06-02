#ifndef __FadaMesh_MeshCompositionInterface_h
#define __FadaMesh_MeshCompositionInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class CouplingMeshInterface;
  class MeshInterface;
  class ModelManagerInterface;
  
  class MeshCompositionInterface : public Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const;

public:
    ~MeshCompositionInterface();
    MeshCompositionInterface();
    MeshCompositionInterface( const MeshCompositionInterface& meshcompositioninterface);
    MeshCompositionInterface& operator=( const MeshCompositionInterface& meshcompositioninterface);
    std::string getName() const;
    MeshCompositionInterface* clone() const;

    virtual int getDimension() const=0;
    virtual std::string getInfo() const = 0;
    virtual void writeMeshInfo(std::string filename) const;
    // virtual void initCouplingGrids(const Alat::DoubleVectorModelManagerInterface* _modelmanager);
    virtual void constructFadaMesh(const std::string& meshname);
    virtual void read(const std::string& basefilename) = 0;
    virtual void write(const std::string& basefilename, std::string datatype = "binary") const = 0;
    virtual int getNDomains() const=0;
    virtual int getNCouplingMeshes() const=0;
    virtual const FadaMesh::MeshInterface* getMesh(int i) const=0;
    virtual void writeH5(std::string filename) const=0;
    virtual const FadaMesh::CouplingMeshInterface* getCouplingMesh(int i) const=0;
    virtual const FadaMesh::MeshInterface* getMacroMesh() const;
    virtual int getNCells() const=0;

  };
}

/*--------------------------------------------------------------------------*/

#endif
