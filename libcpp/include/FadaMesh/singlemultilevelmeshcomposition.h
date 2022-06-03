#ifndef __FadaMesh_SingleMultiLevelMeshComposition_h
#define __FadaMesh_SingleMultiLevelMeshComposition_h

#include  "meshcompositioninterface.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class MeshInterface;
  class SingleMultiLevelMeshComposition : public FadaMesh::MeshCompositionInterface
  {
private:
    FadaMesh::MeshInterface* _mesh;

protected:
public:
    ~SingleMultiLevelMeshComposition();
    SingleMultiLevelMeshComposition();
    SingleMultiLevelMeshComposition( const SingleMultiLevelMeshComposition& singlemeshcomposition);
    SingleMultiLevelMeshComposition& operator=( const SingleMultiLevelMeshComposition& singlemeshcomposition);
    std::string getName() const;
    SingleMultiLevelMeshComposition* clone() const;

    int getDimension() const;
    std::string getInfo() const;
    void read(const std::string& basefilename);
    void write(const std::string& basefilename, std::string datatype = "binary") const;
    void constructFadaMesh(const std::string& meshname);
    int getNDomains() const;
    int getNCouplingMeshes() const;
    const FadaMesh::MeshInterface* getMesh(int i) const;
    void writeMeshInfo(std::string filename) const;
    void writeH5(std::string filename) const;
    int getNCells() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
