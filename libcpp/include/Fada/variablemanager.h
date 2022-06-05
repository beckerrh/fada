#ifndef __Fada_VariableManager_h
#define __Fada_VariableManager_h

#include  "Alat/doublevector.h"
#include  "Alat/intvector.h"
#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/stringvector.h"
#include  "Alat/map.h"
#include  "integratorinterface.h"
#include  "variableinterface.h"
#include  "variablesmap.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class StringSet;
}
namespace Fada
{
  class DomainSolver;
  class IntegratorManager;
  class MeshInterface;
  class ModelInterface;

  class VariableManager
  {
public:
    typedef VariablesMap::const_iterator const_iterator;
    typedef Alat::Vector<VariableInterface*> VariablesVector;
    typedef Alat::Map<std::string, const IntegratorInterface*> IntegratorOfName;

protected:
    VariablesMap _datavariables;
    VariablesMap _postprocessvariables;
    VariablesMap _unknownvariables;

    Alat::StringVector _index_to_unknown, _index_to_postprocess, _index_to_data;
    Alat::StringIntMap _unknown_to_index, _postprocess_to_index, _data_to_index;
    Alat::DoubleVector _variablescale;

    void _insertVariableNames(IntegratorOfName& integratorofundefineddata, IntegratorOfName& integratorofundefinedpostprocess, const Alat::StringVector& output, const Alat::StringVector& input, const Alat::StringVector& data, const IntegratorInterface* integrator) const;

    void addPostProcessVariable(VariableInterface* variable);
    void addDataVariable(VariableInterface* variable);

public:
    ~VariableManager();
    VariableManager();
    VariableManager( const VariableManager& variablemanager);
    VariableManager& operator=( const VariableManager& variablemanager);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;

    double getScale(int ivar) const;

    int getNUnknownVariables() const;
    int getNPostprocessVariables() const;
    int getNDataVariables() const;

    Alat::StringSet getDataNames() const;
    Alat::StringSet getPostProcessNames() const;
    Alat::StringSet getUnknownsNames() const;
    const VariablesMap& getData() const;
    const VariablesMap& getPostProcess() const;
    const VariablesMap& getUnknowns() const;
    const VariableInterface* getData(std::string name) const;
    const VariableInterface* getPostProcess(std::string name) const;
    const VariableInterface* getUnknown(std::string name) const;
    const VariableInterface* getOutputVariable(std::string name) const;
    const VariableInterface* getInputVariable(std::string name) const;
    const VariableInterface* getVariable(std::string name) const;
    // const VariableInterface* getUnknown(int ivar) const;
    // const VariableInterface* getData(int ivar) const;
    // const VariableInterface* getPostProcess(int ivar) const;
    bool variableIsUnknown(std::string name) const;
    bool variableIsData(std::string name) const;
    bool variableIsPostProcess(std::string name) const;

    void addUnknownVariable(VariableInterface* variable);
    void addAdditionalPostProcessVariable(VariableInterface* variable);

    void defineDataAndPostProcessVariables(const IntegratorManager* integratormanager, const ModelInterface* model, const FadaMesh::MeshInterface* mesh, const DomainSolver* solver);
  };
}

/*--------------------------------------------------------------------------*/

#endif
