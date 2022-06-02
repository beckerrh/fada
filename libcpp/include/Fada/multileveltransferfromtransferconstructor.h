#ifndef __Fada_MultiLevelTransferFromTransferConstructor_h
#define __Fada_MultiLevelTransferFromTransferConstructor_h

#include  "Fada/multileveltransfersinglefeminterface.h"
#include  "Alat/intvector.h"
#include  "Alat/variablematrix.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class DofInformation;
  class MultiLevelTransferFromTransferConstructor : public Fada::MultiLevelTransferSingleFemInterface
  {
  protected:
    Alat::Vector<Alat::VariableMatrix> _matrixtransfer;
    Alat::Vector<Alat::VariableMatrix> _matrixproject;
    void _constructWithTransferConstructor(const Fada::DofInformation* dofinfo, Alat::VariableMatrix& matrixtransfer, Alat::VariableMatrix& matrixproject, const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh);

  public:
    ~MultiLevelTransferFromTransferConstructor();
    MultiLevelTransferFromTransferConstructor();
    MultiLevelTransferFromTransferConstructor( const MultiLevelTransferFromTransferConstructor& multileveltransferfromtransferconstructor);
    MultiLevelTransferFromTransferConstructor& operator=( const MultiLevelTransferFromTransferConstructor& multileveltransferfromtransferconstructor);
    std::string getName() const;
    MultiLevelTransferFromTransferConstructor* clone() const;

    void basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem);
    void prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d = 1.0) const;
    void restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const;
    void project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const;
  };
}

/*--------------------------------------------------------------------------*/
#endif
