#ifndef __Fada_MultiLevelTransferAllVariablesInterface_h
#define __Fada_MultiLevelTransferAllVariablesInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class SystemVectorInterface;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class DofManagerAllVariables;
  class DomainSolverInterface;
  class VariableManager;

  class MultiLevelTransferAllVariablesInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~MultiLevelTransferAllVariablesInterface();
    MultiLevelTransferAllVariablesInterface();
    MultiLevelTransferAllVariablesInterface( const MultiLevelTransferAllVariablesInterface& multigridtransferinterface);
    MultiLevelTransferAllVariablesInterface& operator=( const MultiLevelTransferAllVariablesInterface& multigridtransferinterface);
    std::string getName() const;
    MultiLevelTransferAllVariablesInterface* clone() const;

    virtual void basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables);

    virtual void prolongate(int level, Alat::SystemVectorInterface* ufine, const Alat::SystemVectorInterface* ucoarse, double d = 1.0) const=0;
    virtual void restrict(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine ) const=0;
    virtual void project(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine) const=0;
    virtual void interpolate(int level, Alat::SystemVectorInterface* ufine, const Alat::SystemVectorInterface* ucoarse) const;
    
    virtual void constructTransfer(int level, const Alat::SystemMatrixInterface* matrixfine, const Alat::SystemMatrixInterface* matrixcoarse);
    
    virtual bool isConstant() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
