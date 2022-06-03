#ifndef __Fada_ModelInterface_h
#define __Fada_ModelInterface_h

#include  "Alat/map.h"
#include  "Alat/stringset.h"
#include  "Alat/vector.h"
#include  "fadalightenums.h"
#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
  class ParameterFile;
  class StringDoubleVectorMap;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class ApplicationInterface;
  class DataInterface;
  class DomainIntegrationLoopInterface;
  class IntegratorManager;
  class ProblemData;
  class VariableInterface;
  class VariableManager;
  class VariableInterface;

  class ModelInterface : public virtual Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const;

public:
    ~ModelInterface();
    ModelInterface();
    ModelInterface( const ModelInterface& modelinterface);
    ModelInterface& operator=( const ModelInterface& modelinterface);

    typedef Alat::Map<std::string, Alat::StringSet> BoundaryConditionToVariables;
    virtual void definePossibleBoundaryConditions(BoundaryConditionToVariables& bdryconditions) const = 0;
    virtual const Alat::StringDoubleVectorMap& getVariablesScales() const = 0;
    virtual Alat::StringDoubleVectorMap& getVariablesScales() = 0;
    virtual void setVariablesScales(const Alat::StringDoubleVectorMap& maxvalues) = 0;

    virtual void init();
    virtual void basicInit(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh, FadaEnums::looptype looptype) = 0;
    virtual int numberOfBoundaryConditionsPerColor() const = 0;
    virtual void defineVariables(Fada::VariableManager* variable_manager) const = 0;
    virtual void setRunDir(std::string rundir) = 0;
    virtual Fada::ModelInterface* clone() const = 0;
    virtual void defineIntegrators(Fada::IntegratorManager* integratormanager) const = 0;
    virtual void addStringsAndParameters(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh){}
    virtual const Fada::ApplicationInterface* getApplication() const = 0;
    virtual Fada::ApplicationInterface* newApplication(std::string applicationname) const = 0;
    virtual void defineParametersForProblemData() = 0;
    virtual void defineApplicationData(){}
    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    virtual const Fada::DataInterface* getProblemData(const std::string& varname, const std::string& dataname) const = 0;
    virtual ProblemData* getProblemData()  = 0;
    virtual const ProblemData* getProblemData() const = 0;
    virtual double getPhysicalParameter(const std::string& parametername) const = 0;
    virtual double getNumericalParameter(const std::string& parametername) const = 0;
    virtual const std::string& getStringParameter(const std::string& key) const = 0;
    virtual FadaEnums::looptype getLoopType() const = 0;
    virtual const FadaMesh::MeshInterface* getMesh() const = 0;
    virtual const Fada::DataInterface* getData(const std::string& name) const=0;
    virtual std::string matrixcoupling(const std::string& variableout, const std::string& variablsin) const=0;

    virtual Fada::DomainIntegrationLoopInterface* newDiscretization() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
