#ifndef __Fada_DofManagerAllVariables_h
#define __Fada_DofManagerAllVariables_h

#include  "Alat/map.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DofInformationInterface;
  class FemManagerInterface;
  class MeshInterface;
  class DomainSolverInterface;
  class VariableManager;

  class DofManagerAllVariables
  {
private:
    Alat::Vector<DofInformationInterface*> _dofs;
    Alat::StringIntMap _varname2index;
    const FemManagerInterface* _femmanager;
    const VariableManager* _variablemanager;

public:
    ~DofManagerAllVariables();
    DofManagerAllVariables();
    DofManagerAllVariables( const DofManagerAllVariables& dofmanagerallvariables);
    DofManagerAllVariables& operator=( const DofManagerAllVariables& dofmanagerallvariables);
    std::string getName() const;
    DofManagerAllVariables* clone() const;

    void basicInit(const DomainSolverInterface* solver, const FadaMesh::MeshInterface* mesh);
    int getNDofInformations() const;
    const DofInformationInterface* getDofInformationOfIndex(int ivar) const;
    const DofInformationInterface* getDofInformationOfVariable(std::string varname) const;
    void setMeshLevel(int level) const;
    const Alat::StringIntMap& getVarnameToIndex() const;
  };
  std::ostream& operator<<(std::ostream& os, const DofManagerAllVariables& dofmanagerallvariables);
}

/*--------------------------------------------------------------------------*/

#endif
