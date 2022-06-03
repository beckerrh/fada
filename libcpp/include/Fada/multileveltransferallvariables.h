#ifndef __Fada_MultiLevelTransferAllVariables_h
#define __Fada_MultiLevelTransferAllVariables_h

#include  "multileveltransferallvariablesinterface.h"
#include  "multileveltransfersinglefeminterface.h"
#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
class MultiLevelTransferAllVariables : public MultiLevelTransferAllVariablesInterface
{
protected:
  Alat::Map<std::string, std::string> _varnametofem;
  Alat::Map<std::string, Fada::MultiLevelTransferSingleFemInterface*> _mltransfersinglefem;

public:
  ~MultiLevelTransferAllVariables();
  MultiLevelTransferAllVariables();
  MultiLevelTransferAllVariables( const MultiLevelTransferAllVariables& multileveltransferallvariables);
  MultiLevelTransferAllVariables& operator=( const MultiLevelTransferAllVariables& multileveltransferallvariables);
  std::string getName() const;
  MultiLevelTransferAllVariables* clone() const;

  void basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables);
  void prolongate(int level, Alat::SystemVector* ufine, const Alat::SystemVector* ucoarse, double d = 1.0) const;
  void restrict(int level, Alat::SystemVector* ucoarse, const Alat::SystemVector* ufine ) const;
  void project(int level, Alat::SystemVector* ucoarse, const Alat::SystemVector* ufine) const;
};
}

/*--------------------------------------------------------------------------*/
#endif
