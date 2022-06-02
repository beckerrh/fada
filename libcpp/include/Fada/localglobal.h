#ifndef __Fada_LocalGlobal_h
#define __Fada_LocalGlobal_h

#include  "Alat/stringdoublemap.h"
#include  "Alat/stringdoublevectormap.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class AssembleVector;
  class SystemAssembleMatrix;
  class SystemAssembleVector;
  class IntVector;
}
namespace Alat
{
  class SystemVectorInterface;
}
namespace Alat
{
  class MatrixAllVariables;
}
namespace Fada
{
  class DofManagerAllVariables;
  class LocalVectors;
  class VariableInterface;
  class VariableManager;

  class LocalGlobal
  {
public:
    typedef Alat::Map<std::string, const Alat::SystemVectorInterface*> VectorMap;

    ~LocalGlobal();
    LocalGlobal();
    LocalGlobal( const LocalGlobal& localglobal);
    LocalGlobal& operator=( const LocalGlobal& localglobal);
    std::string getName() const;
    LocalGlobal* clone() const;

    const VariableManager* _variablemanager;
    const DofManagerAllVariables* _dofmanagerallvariables;
    Alat::StringDoubleVectorMap _scalings;

    void setVariablesScales(const Alat::StringDoubleVectorMap& variablescales);
    void basicInit(const DofManagerAllVariables* dofmanagerallvariables, const VariableManager* variable_manager, const Alat::StringDoubleVectorMap& scalings);
    void initLocalVectors(Fada::LocalVectors& localvectors, const VectorMap& vectormap) const;
    void extractAll(Fada::LocalVectors& localvectors, int iK, const VectorMap& vectormap, const Alat::SystemVectorInterface* datavector) const;
    void extractAll(Fada::LocalVectors& localvectors, const VectorMap& vectormap, const Alat::SystemVectorInterface* datavector, const Alat::IntVector& indices) const;
    void initLocalOutput(Alat::SystemAssembleVector& floc) const;
    void initLocalPostProcess(Alat::SystemAssembleVector& floc) const;
    void initLocalMatrix(Alat::SystemAssembleMatrix& Aloc) const;
    void assembleMatrixAllVariables(int iK, Alat::MatrixAllVariables* matrix, const Alat::SystemAssembleMatrix& Aloc, const Alat::StringSet& variables) const;
    void assembleMatrixAllVariables(int iKL, int iKR, Alat::MatrixAllVariables* matrix, const Alat::SystemAssembleMatrix& AlocLL, const Alat::SystemAssembleMatrix& AlocLR, const Alat::SystemAssembleMatrix& AlocRL, const Alat::SystemAssembleMatrix& AlocRR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR) const;
    void assembleVector(Alat::SystemVectorInterface* fav, const Alat::SystemAssembleVector& floc, int iK, const Alat::StringSet& variables, double d) const;
    void assembleVector(Alat::SystemVectorInterface* fav, const Alat::SystemAssembleVector& floc, const Alat::IntVector& indicesnodes, const Alat::StringSet& variables, double d) const;
    void assembleVectorPostProcess(Alat::SystemVectorInterface* fav, const Alat::SystemAssembleVector& floc, int iK, const Alat::StringSet& variables, double d) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
