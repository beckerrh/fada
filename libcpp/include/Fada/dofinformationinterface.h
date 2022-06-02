#ifndef __Fada_DofInformationInterface_h
#define __Fada_DofInformationInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class AssembleVector;
  class DoubleMatrix;
  class IntVector;
}
namespace FadaMesh
{
  class MeshInterface;
  class RefineInfo;
}
namespace Fada
{
  class FemInterface;
  class LocalDofInformation;

  class DofInformationInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~DofInformationInterface();
    DofInformationInterface();
    DofInformationInterface( const DofInformationInterface& dofinformationinterface);
    DofInformationInterface& operator=( const DofInformationInterface& dofinformationinterface);
    std::string getName() const;
    DofInformationInterface* clone() const;

    virtual const LocalDofInformation* getLocalDofInformation() const = 0;
    virtual void setMeshLevel(int level) = 0;
    virtual void basicInit(Fada::FemInterface* fem, const FadaMesh::MeshInterface* mesh) = 0;
    virtual int ndof() const = 0;
    virtual int ndof(const FadaMesh::MeshInterface* refinedmesh) const = 0;
    virtual int ndofpercell() const = 0;
    virtual int ndoflocal() const = 0;
    virtual int ndoflocalpatch() const = 0;
    virtual void getCellIndices(int iK, Alat::IntVector& indices) const = 0;
    virtual void getCellIndicesInterpolate(int iK, Alat::IntVector& indices) const;
    virtual void getCellIndicesInterpolateRefined(int iK, Alat::IntVector& indices, const FadaMesh::MeshInterface* refinedmesh, const FadaMesh::RefineInfo* refinfo) const = 0;
    virtual void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const = 0;
    virtual Fada::FemInterface* getFem() const = 0;
    virtual void interpolate(int iK, Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const = 0;
    virtual const Alat::DoubleMatrix& getInterpolationMatrix() const = 0;
    virtual const Alat::DoubleMatrix& getProjectionMatrix() const = 0;

    virtual void setDofIdsOnSide(int iS, Alat::IntVector& ids) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
