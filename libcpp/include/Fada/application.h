#ifndef __Fada_Application_h
#define __Fada_Application_h

#include  "applicationinterface.h"
#include  "fadalightenums.h"
#include  "parametermap.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
  class StringDoubleVectorMap;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class Application : public virtual ApplicationInterface
  {
protected:
    int _dimension;
    const Alat::ParameterFile* _parameterfile;
    Fada::ParameterMap<double> _parameters;
    Fada::ParameterMap<std::string> _stringparameters;
    Alat::Map<std::string, std::string> _bdcolors;
    FadaEnums::looptype _looptype;
    const Fada::ModelInterface* _model;

    const Alat::Map<std::string, std::string>& getBoundaryConditions() const;
    void clearBoundaryCondition();
    const Fada::ParameterMap<double>& getParameters() const;
    Fada::ParameterMap<double>& getParameters();
    double& getParameter(const std::string& key);
    void addParameter(std::string name, double d = 0.0);
    void addStringParameter(std::string name, std::string s = "none");

public:
    ~Application();
    Application();
    Application( const Application& application);
    Application& operator=( const Application& application);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    bool hasExactSolutions() const;
    void setVariablesScales(Alat::StringDoubleVectorMap& varscales) const;

    void basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model, const Alat::StringSet& possibleboundaryconditions);
    double getParameter(const std::string& key) const;
    const std::string& getStringParameter(const std::string& key) const;
    int getDimension() const;
    const Alat::ParameterFile* getParameterFile() const;
    std::string& getBoundaryData(const std::string& bdtype);
    std::string getBoundaryData(const std::string& bdtype) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
