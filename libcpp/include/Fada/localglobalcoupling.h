#ifndef __Fada_LocalGlobalCoupling_h
#define __Fada_LocalGlobalCoupling_h

#include  "localglobal.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
}

namespace Fada
{
  class AssembleVector;
  class DofManagerAllVariables;
  class LocalVectors;
  class MatrixAllVariables;
  class SystemAssembleMatrix;
  class SystemAssembleVector;
  class SystemVectorInterface;
  class VariableInterface;
  class VariableManager;
  class VectorInterface;

  class LocalGlobalCoupling
  {
protected:

public:
    ~LocalGlobalCoupling();
    LocalGlobalCoupling();
    LocalGlobalCoupling( const LocalGlobalCoupling& localglobal);
    LocalGlobalCoupling& operator=( const LocalGlobalCoupling& localglobal);
    std::string getName() const;
    LocalGlobalCoupling* clone() const;

    LocalGlobal _left, _right;

    void basicInit(const DofManagerAllVariables* dofmanagerallvariables_left, const VariableManager* variable_manager_left, const DofManagerAllVariables* dofmanagerallvariables_right, const VariableManager* variable_manager_right, const Alat::StringDoubleVectorMap& scalings_left, const Alat::StringDoubleVectorMap& scalings_right);
    void initLocalVectors(Fada::LocalVectors& localvectorsleft, Fada::LocalVectors& localvectorsright, const LocalGlobal::VectorMap& vectormapL, const LocalGlobal::VectorMap& vectormapR) const;
    void extractAll(Fada::LocalVectors& localvectors_left, Fada::LocalVectors& localvectorsR, const LocalGlobal::VectorMap& vectormapL, const Alat::SystemVectorInterface* datavectorL, const LocalGlobal::VectorMap& vectormapR, const Alat::SystemVectorInterface* datavectorR, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright) const;

    void initLocalOutput(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const;
    void initLocalPostProcess(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const;
    void initLocalMatrices(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const;

    void assembleVector(Alat::SystemVectorInterface* favL, Alat::SystemVectorInterface* favR, const Alat::SystemAssembleVector& flocL, const Alat::SystemAssembleVector& flocR, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR, double d) const;

    void assembleVectorPostProcess(Alat::SystemVectorInterface* vL, Alat::SystemVectorInterface* vR, const Alat::SystemAssembleVector& flocL, const Alat::SystemAssembleVector& flocR, int iKL, int iKR, const Fada::VariableManager* variablemanagerL, const Fada::VariableManager* variablemanagerR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR, double d) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
