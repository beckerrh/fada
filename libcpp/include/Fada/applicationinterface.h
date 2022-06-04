#ifndef __Fada_ApplicationInterface_h
#define __Fada_ApplicationInterface_h

#include  "fadaenums.h"
#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class StringDoubleVectorMap;
  class StringSet;
  class ParameterFile;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class ApplicationInterface;
  class DataInterface;
  class DataFunctionVector;
  class FunctionalMap;
  class IntegratorManager;
  class ModelInterface;
  class ProblemData;

  class ApplicationInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~ApplicationInterface();
    ApplicationInterface();
    ApplicationInterface( const ApplicationInterface& applicationinterface);
    ApplicationInterface& operator=( const ApplicationInterface& applicationinterface);
    virtual ApplicationInterface* clone() const = 0;
    std::string getName() const;

    virtual void basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model, const Alat::StringSet& possibleboundaryconditions) = 0;
    virtual std::string getBoundaryData(const std::string& bdtype) const = 0;

    virtual void setVariablesScales(Alat::StringDoubleVectorMap& varscales) const = 0;
    virtual Fada::DataInterface* defineData(std::string name) const;
    virtual void defineProblemData(Fada::ProblemData* problemdata) const = 0;
    virtual bool hasExactSolutions() const = 0;
    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    virtual void defineIntegrators(Fada::IntegratorManager* integratormanager) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
