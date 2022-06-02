#ifndef __Fada_DofInformationWithoutFem_h
#define __Fada_DofInformationWithoutFem_h

#include  "dofinformationinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
class DofInformationWithoutFem : public DofInformationInterface
{
public:
  ~DofInformationWithoutFem();
  DofInformationWithoutFem();
  DofInformationWithoutFem( const DofInformationWithoutFem& dofinformationwithoutfem);
  DofInformationWithoutFem& operator=( const DofInformationWithoutFem& dofinformationwithoutfem);
  std::string getName() const;
  DofInformationWithoutFem* clone() const;

  const LocalDofInformation* getLocalDofInformation() const;
  void setMeshLevel(int level);
  void basicInit(Fada::FemInterface* fem, const FadaMesh::MeshInterface* mesh);
  int ndof() const;
  int ndof(const FadaMesh::MeshInterface* refinedmesh) const;
  int ndofpercell() const;
  int ndoflocal() const;
  int ndoflocalpatch() const;
  void getCellIndices(int iK, Alat::IntVector& indices) const;
  void getCellIndicesInterpolateRefined(int iK, Alat::IntVector& indices, const FadaMesh::MeshInterface* refinedmesh, const FadaMesh::RefineInfo* refinfo) const;
  void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const;
  Fada::FemInterface* getFem() const;
  void interpolate(int iK, Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const;
  const Alat::DoubleMatrix& getInterpolationMatrix() const;
  const Alat::DoubleMatrix& getProjectionMatrix() const;
};
}

/*--------------------------------------------------------------------------*/

#endif
