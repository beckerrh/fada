#ifndef __Fada_integratorinterface_h
#define __Fada_integratorinterface_h

#include  "fadaenums.h"
#include  "Alat/interfacebase.h"
#include  "localglobal.h"
#include  "Alat/enums.h"
#include  "Fada/femfunctionsmap.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class AssembleVector;
  class DoubleMatrix;
  class IntVector;
  template <class K, class V>
  class Map;
  template <class T>
  class Set;
  template <class T>
  class Vector;
  class StringVector;
  class Node;
  class SystemAssembleMatrix;
  class SystemAssembleVector;
}
namespace Alat
{
  class SystemVector;
  class VectorInterface;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class DataInterface;
  class DofInformationInterface;
  class DofManagerAllVariables;
  class DomainSolver;
  class FemFunctions;
  class FemManagerData;
  class FemManagerInterface;
  class FemInterface;
  class LocalVectors;
  class ProblemData;
  class DomainSolverInterface;
  class TermAndDomain;
  class TimeData;
  class VariableManager;
  class VariableInterface;
  class VarInfoIntegrator;

  class IntegratorInterface : public Alat::InterfaceBase
  {
public:
    typedef Alat::Map<FadaEnums::term, Alat::Set<FadaEnums::domain> > DomainsOfTerm;

protected:
    std::string getInterfaceName() const;

public:
    ~IntegratorInterface();
    IntegratorInterface();
    IntegratorInterface( const IntegratorInterface& IntegratorInterface);
    IntegratorInterface& operator=( const IntegratorInterface& IntegratorInterface);
    // virtual IntegratorInterface* clone() const = 0;

    virtual void setParameter(const std::string& parametername, double d) = 0;
    virtual void setParameter(const std::string& parametername, std::string d) = 0;
    virtual void setData(const std::string& dataname, const DataInterface* data) = 0;
    virtual const Fada::DataInterface* getData(const std::string& name) const = 0;
    virtual void initData();
    virtual Fada::VariableInterface* constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const;
    virtual Fada::VariableInterface* constructDataVariable(std::string name, const Fada::VariableManager& variablemanager, const FadaMesh::MeshInterface* mesh, const DomainSolver* solver) const;
    virtual void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit) = 0;
    virtual void setTimeScheme(std::string time_discretization) = 0;
    virtual const Fada::DataInterface* getProblemData(const std::string& varname, const std::string& dataname) const = 0;
    virtual void setProblemData(const ProblemData* problemdata) = 0;
    virtual int getOrderForIntegrationFormula(int orderin, int orderout) const = 0;
    virtual void setDomainsOfTerms() = 0;
    virtual const DomainsOfTerm& getDomainsOfTerm() const = 0;

    virtual bool noIntegration(Fada::TermAndDomain term_and_domain) const=0;
    virtual void basicInit(const Fada::DomainSolverInterface* solver);
    virtual bool worksOn(FadaEnums::term t) const = 0;
    virtual bool worksOn(FadaEnums::domain d) const = 0;
    virtual bool worksOnDomainOfTerm(FadaEnums::term t, FadaEnums::domain d) const = 0;

    virtual void postProcessCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
    virtual void postProcessCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const;

    virtual void rhsCouplingSide(AlatEnums::residualstatus& status, double weight, double measureSmallSide) const;

    virtual void rhsTimeCouplingSide(AlatEnums::residualstatus& status, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR, double d) const;

    virtual void formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
    virtual void formCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const;

    virtual void linearizationCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
    virtual void linearizationCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const;

    virtual void matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const;
    virtual void matrixCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const;

    virtual void sortCriterion(AlatEnums::residualstatus& status, double&  value, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const;

    virtual void beforeIntegration(const Fada::LocalVectors& localvectorsleft, const Fada::LocalVectors& localvectorsright) const;
    virtual void beforeIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, const Fada::LocalGlobal& localglobal, Fada::LocalVectors& localvectors, Fada::LocalVectors& localvectorsR, const Fada::LocalGlobal::VectorMap& vectormap, const Alat::SystemVector* datavector) const;
    virtual void afterIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, Alat::SystemVector* ouput) const;
    virtual void beforeIntegration(const Fada::LocalVectors& localvectors) const;
    virtual bool needsVeryOld() const;
    virtual void setLavrentievParameter(double parameter) const;
    virtual void setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const=0;
    virtual void setFemFunctions(const Fada::FemFunctionsMap& femfctmapL, const Fada::FemFunctionsMap& femfctmapR) const=0;
    virtual void setAssembleData(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const=0;
    virtual void setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const=0;




    virtual const Fada::VarInfoIntegrator* getVarInfo() const = 0;
    virtual void defineVariables(const std::string& output, const std::string& input, const std::string& data) = 0;

    virtual void postProcessCell(AlatEnums::residualstatus& status, double weight) const;
    virtual void postProcessBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    virtual void postProcessCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& flocL, const Fada::LocalVectors& localvectors) const;
    virtual void postProcessBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, const Fada::LocalVectors& localvectors, int color) const;

    virtual void rhsCell(AlatEnums::residualstatus& status, double weight) const;
    virtual void rhsCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc) const;
    virtual void rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const;
    virtual void rhsBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, int color) const;

    virtual void rhsTimeCell(AlatEnums::residualstatus& status, double weight, double d) const;
    virtual void rhsTimeCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, double d) const;
    virtual void rhsTimeBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, Alat::SystemAssembleVector& floc, double weight, int color, const Fada::FemFunctionsMap& femfcts, double d) const;
    virtual void rhsTimeBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color, double d) const;

    virtual void formCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    virtual void formCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors) const;
    virtual void formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    virtual void formBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color) const;

    virtual void linearizationCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    virtual void linearizationCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors) const;
    virtual void linearizationBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    virtual void linearizationBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color) const;

    virtual void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    virtual void matrixCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleMatrix& Aloc, const Fada::LocalVectors& localvectors) const;
    virtual void matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const;
    virtual void matrixBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleMatrix& Aloc, const Fada::LocalVectors& localvectors, int color) const;

    virtual void computeNormSquaredCell(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, double weight, const Fada::FemFunctionsMap& femfcts) const;
    // virtual void computeNormSquaredCellNoIntegration(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, int iK, const Fada::FemManagerData& femd, const Fada::LocalVectors& localvectors) const;

  };
}

/*--------------------------------------------------------------------------*/

#endif
