#ifndef __Fada_Integrator_h
#define __Fada_Integrator_h

#include  "Alat/set.h"
#include  "Alat/stringdoublemap.h"
#include  "Alat/systemassemblematrix.h"
#include  "datamap.h"
#include  "Fada/termanddomain.h"
#include  "integratorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class BoundaryManager;
  class DomainSolverInterface;
  class ModelInterface;
  class ProblemData;
  class SolverInterface;
  template<class T>
  class Vector;

  class Integrator : public virtual IntegratorInterface
  {
protected:
    const Fada::ModelInterface* _model;
    const Fada::DomainSolverInterface* _solver;
    VarInfoIntegrator* _varinfo;
    const Fada::ProblemData* _problemdata;
    bool _basicinitcalled;
    int _dimension;
    const Fada::BoundaryManager* _boundarymanager;
    mutable double _time, _dt, _dtold, _masscoefimplicit, _trparameter;
    std::string _timescheme;
    DomainsOfTerm _domainsofterm;
    Alat::Set<Fada::TermAndDomain> _nointegration;
    Alat::StringDoubleMap _parameters;
    Alat::Map<std::string, std::string> _strings;
    Fada::DataMap _data;
    FadaEnums::looptype _looptype;
    const FemManagerInterface* _femmanager;
    const FemManagerInterface* _femmanagerR;
    mutable const Fada::FemFunctionsMap* _femfctmapL, * _femfctmapR;
    mutable Alat::SystemAssembleMatrix* _AlocLL, * _AlocLR, * _AlocRL, * _AlocRR;
    mutable Alat::SystemAssembleVector* _flocL, * _flocR;

    void addDomainsOfTerm(FadaEnums::term t, FadaEnums::domain d);
    void setNoIntegration(FadaEnums::term t, FadaEnums::domain d);
    bool noIntegration(Fada::TermAndDomain term_and_domain) const;
    bool adjointMode() const;
    const Fada::BoundaryManager* getBoundaryManager() const;
    int getDimension() const;
    const DomainsOfTerm& getDomainsOfTerm() const;
    DomainsOfTerm& getDomainsOfTerm();
    double getMassCoefImplicit() const;
    double getDt() const;
    double getDtOld() const;
    double getParameter(const std::string& parametername) const;
    double getParameter(const std::string& parametername, double d) const;
    std::string getStringParameter(const std::string& parametername) const;
    std::string getStringParameter(const std::string& parametername, std::string d) const;
    double getTime() const;
    bool tangentMode() const;
    void _basicInit(const Fada::DomainSolverInterface* solver);
    const FemInterface* fem(std::string ivar) const;
    const FemInterface* femL(std::string ivar) const;
    const FemInterface* femR(std::string ivar) const;

public:
    ~Integrator();
    Integrator();
    Integrator( const Integrator& integrator);
    Integrator& operator=( const Integrator& integrator);
    std::string getName() const;

    const Fada::VarInfoIntegrator* getVarInfo() const;
    const Fada::DomainSolverInterface* getSolver() const;
    const Fada::ModelInterface* getModel() const;
    void basicInit(const Fada::DomainSolverInterface* solver);
    void defineVariables(const std::string& output, const std::string& input, const std::string& data);

    int getOrderForIntegrationFormula(int orderin, int orderout) const;
    const Fada::DataInterface* getData(const std::string& name) const;
    const Fada::DataMap* getData() const;
    const Fada::DataInterface* getDataIfDefined(const std::string& name) const;
    std::string getDataName(int index) const;
    const Alat::Set<FadaEnums::domain>& getDomainsOfTerm(FadaEnums::term t) const;
    Alat::Set<FadaEnums::domain>& getDomainsOfTerm(FadaEnums::term t);
    const Fada::ProblemData* getProblemData() const;
    const Fada::DataInterface* getProblemData(const std::string& varname, const std::string& dataname) const;
    void setProblemData(const ProblemData* problemdata);

    bool hasKey(const std::string& name) const;
    void setData(const std::string& dataname, const DataInterface* data);
    void setIntegrationFormulaName(FadaEnums::term t, FadaEnums::domain d) const;
    void setParameter(const std::string& parametername, double d);
    void setParameter(const std::string& parametername, std::string d);
    void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit);
    void setTimeScheme(std::string time_discretization);
    bool worksOn(FadaEnums::term t) const;
    bool worksOn(FadaEnums::domain d) const;
    bool worksOnDomainOfTerm(FadaEnums::term t, FadaEnums::domain d) const;
    void setLavrentievParameter(double parameter) const;
    void setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const;
    void setFemFunctions(const Fada::FemFunctionsMap& femfctmapL, const Fada::FemFunctionsMap& femfctmapR) const;
    void setAssembleData(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const;
    void setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
