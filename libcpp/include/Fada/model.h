#ifndef __Fada_Model_h
#define __Fada_Model_h

#include  "Alat/map.h"
#include  "applicationinterface.h"
#include  "boundarymanager.h"
#include  "functionalmap.h"
#include  "FadaMesh/meshinterface.h"
#include  "modelinterface.h"
#include  "Alat/parameterfile.h"
#include  "parametermap.h"
#include  "parametertranslator.h"
#include  "problemdata.h"
#include  "Alat/stringdoublevectormap.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Model : public virtual ModelInterface
  {
protected:
    ApplicationInterface* _application;
    mutable const FadaMesh::MeshInterface*  _mesh;
    const Alat::ParameterFile* _parameterfile;
    std::string _rundir, _applicationname, _scaling;
    Fada::ParameterMap < std::string > _stringparameters;
    Fada::ParameterMap < std::string > _stringparametersofloop;
    Fada::ParameterMap < double > _physicalparameters, _numericalparameters;
    Fada::ProblemData _problemdata;
    mutable Fada::ParameterTranslator _dico;
    Fada::DataMap _data;
    Fada::FunctionalMap _functional_map;
    bool _basicinitcalled;
    int _modelid;
    FadaEnums::looptype _looptype;
    Alat::StringDoubleVectorMap _variablesscales;

    void addString(std::string key, std::string value = "");
    void addStringOfLoop(std::string key, std::string value = "");
    std::string determineFemType(std::string varname) const;
    void setParameters();
    void setDataForProblemData(const std::string& varname, const std::string& problemdataname, const std::string& dataname, const Fada::DataInterface* data);
    void setParameterForProblemData(const std::string& varname, const std::string& problemdataname, const std::string& dataname, double d);
    void setParameterForProblemDataIfDefined(const std::string& varname, const std::string& problemdataname, const std::string& dataname, double d);

public:
    ~Model();
    Model();
    Model(const Model &model);
    Model& operator = ( const Model &model );
    ModelInterface* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh, FadaEnums::looptype looptype);
    void constructApplication();
    std::string getName() const;
    void setMesh(const FadaMesh::MeshInterface* mesh) const;
    const FadaMesh::MeshInterface* getMesh() const;
    const Alat::ParameterFile* getParameterFile() const;
    void setRunDir(std::string rundir);
    std::string getRunDir() const;
    const ApplicationInterface* getApplication() const;
    const Alat::StringDoubleVectorMap& getVariablesScales() const;
    Alat::StringDoubleVectorMap& getVariablesScales();
    void setVariablesScales(const Alat::StringDoubleVectorMap& maxvalues);

    const Fada::ParameterMap < std::string >& getStringParamaters() const;
    Fada::ParameterMap < std::string > &getStringParamaters();
    const Fada::ParameterMap < std::string >& getStringParamatersOfLoop() const;
    Fada::ParameterMap < std::string > &getStringParamatersOfLoop();
    const Fada::ParameterMap < double >& getPhysicalParameters() const;
    Fada::ParameterMap < double > &getPhysicalParameters();
    const Fada::ParameterMap < double >& getNumericalParameters() const;
    Fada::ParameterMap < double > &getNumericalParameters();
    const std::string& getStringParameter(const std::string& key) const;
    const std::string& getStringParameterOfLoop(const std::string& key) const;
    void addPhysicalParameter(std::string name, double d = 0.0);
    void addNumericalParameter(std::string name, double d = 0.0);
    double getPhysicalParameter(const std::string& parametername) const;
    double getNumericalParameter(const std::string& parametername) const;
    bool physicalParameterhasKey(std::string name) const;
    bool numericalParameterhasKey(std::string name) const;
    const Fada::ProblemData* getProblemData() const;
    Fada::ProblemData* getProblemData();
    Fada::DataInterface* getProblemData(const std::string& varname, const std::string& dataname);
    const Fada::DataInterface* getProblemData(const std::string& varname, const std::string& dataname) const;
    const Fada::DataInterface* getData(const std::string& name) const;

    bool isDataDefined(const std::string& name) const;
    FadaEnums::looptype getLoopType() const;
    Fada::ParameterTranslator&  getDictionary() const;

    const Fada::DataInterface*& setData(const std::string& name);
    void deleteData(const std::string& name);

    Fada::FunctionalInterface* getFunctional(const std::string& name);
    const Fada::FunctionalInterface* getFunctional(const std::string& name) const;

    FunctionalMap* getFunctionals();
    const FunctionalMap* getFunctionals() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
