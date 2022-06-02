#ifndef __FadaMesh_SingleMeshComposition_h
#define __FadaMesh_SingleMeshComposition_h

#include  "meshcompositioninterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshInterface;
  class SingleMeshComposition : public FadaMesh::MeshCompositionInterface
  {
private:
    FadaMesh::MeshInterface* _mesh;

protected:
public:
    ~SingleMeshComposition();
    SingleMeshComposition();
    SingleMeshComposition( const SingleMeshComposition& singlemeshcomposition);
    SingleMeshComposition& operator=( const SingleMeshComposition& singlemeshcomposition);
    std::string getName() const;
    SingleMeshComposition* clone() const;

    int getDimension() const;
    std::string getInfo() const;
    void read(const std::string& basefilename);
    void write(const std::string& basefilename, std::string datatype = "binary") const;
    void constructFadaMesh(const std::string& meshname);
    int getNDomains() const;
    int getNCouplingMeshes() const;
    const FadaMesh::MeshInterface* getMesh(int i) const;
    const FadaMesh::CouplingMeshInterface* getCouplingMesh(int i) const;
    void writeMeshInfo(std::string filename) const;
    void writeH5(std::string filename) const;
    int getNCells() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
