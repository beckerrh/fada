#ifndef __Fada_UnstructuredMultiLevelMatrixTransfer_h
#define __Fada_UnstructuredMultiLevelMatrixTransfer_h

#include  "Fada/multilevelmatrix.h"
#include  "Fada/multileveltransferallvariablesinterface.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class UnstructuredMultiLevelMatrixTransfer : public Fada::MultiLevelTransferAllVariablesInterface
  {
protected:
    Fada::MultiLevelMatrix _matrix;
    int _ncompsum;
    Alat::IntVector _offsets, _ncomps;
    Alat::Vector<Alat::MatrixAllVariables> _mat;

public:
    ~UnstructuredMultiLevelMatrixTransfer();
    UnstructuredMultiLevelMatrixTransfer();
    UnstructuredMultiLevelMatrixTransfer( const UnstructuredMultiLevelMatrixTransfer& multilevelmatrixtransfer);
    UnstructuredMultiLevelMatrixTransfer& operator=( const UnstructuredMultiLevelMatrixTransfer& multilevelmatrixtransfer);
    std::string getName() const;
    UnstructuredMultiLevelMatrixTransfer* clone() const;

     void basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables);

     void prolongate(int level, Alat::SystemVector* ufine, const Alat::SystemVector* ucoarse, double d = 1.0) const;
     void restrict(int level, Alat::SystemVector* ucoarse, const Alat::SystemVector* ufine ) const;
     void project(int level, Alat::SystemVector* ucoarse, const Alat::SystemVector* ufine) const;
     void interpolate(int level, Alat::SystemVector* ufine, const Alat::SystemVector* ucoarse) const;
     void constructTransfer(int level, const Alat::SystemMatrixInterface* matrixfine, const Alat::SystemMatrixInterface* matrixcoarse);

    bool isConstant() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
