#ifndef __Fada_DofInformation_h
#define __Fada_DofInformation_h

#include  "Alat/doublematrix.h"
#include  "Alat/intvector.h"
#include  "Fada/dofinformationinterface.h"
#include  "Fada/indicesinpatch.h"
#include  "Fada/localdofinformation.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemInterface;
  class IntVector;
  class MeshInterface;
  class VariableInterface;
}

namespace Fada
{
  class DofInformation : public Fada::DofInformationInterface
  {
protected:
    const FadaMesh::MeshInterface* _mesh;
    mutable Fada::FemInterface* _fem;
    // int _ndofnodes, _ndofedges, _ndofsides, _ndofcells;
    int _ndoflocal, _ndoflocalpatch;
    Alat::IntVector _ndofs, _ndofspercell, _offsetcells, _offsetsides, _offsetesges, _offsetnodes;
    Fada::IndicesInPatch _indicesinpatch;
    int _level;
    Alat::DoubleMatrix _interpolationmatrix, _projectionmatrix;
    Fada::LocalDofInformation _localdofinformation;

public:
    ~DofInformation();
    DofInformation();
    DofInformation( const DofInformation& dofinformation);
    DofInformation& operator=( const DofInformation& dofinformation);
    std::string getName() const;
    DofInformation* clone() const;

    const Fada::LocalDofInformation* getLocalDofInformation() const;
    void setMeshLevel(int level);
    void basicInit(Fada::FemInterface* fem, const FadaMesh::MeshInterface* mesh);
    int ndof() const;
    int ndof(const FadaMesh::MeshInterface* refinedmesh) const;
    int ndofpercell() const;
    int ndoflocal() const;
    int ndoflocalpatch() const;
    void getCellIndices(int iK, Alat::IntVector& indices) const;
    void getCellIndicesInterpolate(int iK, Alat::IntVector& indices) const;
    void getCellIndicesInterpolateRefined(int iK, Alat::IntVector& indices, const FadaMesh::MeshInterface* refinedmesh, const FadaMesh::RefineInfo* refinfo) const;
    void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const;
    Fada::FemInterface* getFem() const;
    void interpolate(int iK, Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const;
    void project(int iK, Alat::AssembleVector& uH, const Alat::AssembleVector& uh) const;
    const Alat::DoubleMatrix& getInterpolationMatrix() const;
    const Alat::DoubleMatrix& getProjectionMatrix() const;

    void setLocalDofIdsOnSide(int iS, Alat::IntVector& ids) const;
    void setDofIdsOnSide(int iS, Alat::IntVector& ids) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
