#ifndef __Fada_SolverManagerInterface_h
#define __Fada_SolverManagerInterface_h

#include  "fadalightenums.h"
#include  "Alat/enums.h"
#include  "Alat/interfacebase.h"
#include  "Alat/pair.h"
#include  "Alat/set.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template<int N, class T>
  class FixArray;
  class IntVector;
  template<class K, class T>
  class Map;
  class StringDoubleMap;
  class StringVector;
  template<class T>
  class Vector;
  class GhostVector;
  class GhostLinearSolver;
  class GhostMatrix;
  class IoManager;
  class ParameterFile;
  class SparsityPattern;
}
namespace Alat
{
  class LinearSolverInterface;
}
namespace FadaMesh
{
  class CouplingMeshInterface;
  class MeshInterface;
  class MeshCompositionInterface;
  class VectorInterface;
}
namespace Fada
{
  class Chronometer;
  class DofManager;
  class DomainSolverInterface;
  class FormIntegrationLoopInterface;
  class IntegrationLoopInterface;
  class IntegratorManager;
  class IntegratorInterface;
  class LinearInfo;
  class ManagerInterface;
  class ModelManagerInterface;
  class NonLinearInfo;
  class TimeData;
  class VariableInterface;
  class VariableManager;

  class SolverManagerInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;
    virtual Alat::LinearSolverInterface* newLinearSolver(const Alat::GhostLinearSolver& linearsolver) = 0;
    virtual void setDomainsOfCoupling(Alat::Vector<Alat::FixArray<2, int> >& domainsofcoupling, Alat::SparsityPattern& _domainneighbours, Alat::Map<Alat::IntPair, int>& _couplingofdomains) const = 0;

public:
    ~SolverManagerInterface();
    SolverManagerInterface();
    SolverManagerInterface( const SolverManagerInterface& solvermanagerinterface);
    SolverManagerInterface& operator=( const SolverManagerInterface& solvermanagerinterface);

    virtual void basicInit(Fada::ModelManagerInterface* modelmanager, const FadaMesh::MeshCompositionInterface* meshcomposition, const Alat::IoManager& io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile = NULL) = 0;
    virtual void reInit() = 0;
    virtual void reInitMatrixAndLinearSolver() = 0;
    virtual int getNDomainSolvers() const = 0;
    // virtual int getNCouplingSolvers() const = 0;
    virtual void printChronometer(std::string filename) const = 0;
    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    virtual void registerVector(const Alat::GhostVector& v) = 0;
    virtual void registerMatrix( Alat::GhostMatrix& matrix) = 0;
    virtual void registerLinearSolver(const Alat::GhostLinearSolver& v) = 0;
    virtual void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit) = 0;
    virtual void setTimeScheme(std::string time_discretization) = 0;
    virtual double getTime() const = 0;
    virtual double getDeltaT() const = 0;

    virtual int getNlevels() const = 0;

    virtual const FadaMesh::MeshCompositionInterface* getMeshComposition() const = 0;

    virtual void readUnknownVariables(Alat::GhostVector& v, int number = -1) = 0;
    virtual void writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& gv, int number = -1) const = 0;
    virtual void writeVariablesInfo() const = 0;
    virtual void writePostProcessVariables(const Alat::GhostVector& v, int number = -1) const = 0;
    virtual void writePostProcessVariablesDynamic(const Alat::GhostVector& v, int number) const = 0;

    virtual void initSolution(Alat::GhostVector& u) = 0;
    virtual int linearSolve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, const Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& x, const Alat::GhostVector& b) const = 0;
    virtual void constructLinearSolver(Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& u);
    virtual void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, const Alat::GhostVector& u) = 0;
    virtual void constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& f) const = 0;
    virtual void constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const = 0;
    virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& du) const = 0;
    virtual void postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level=0) const = 0;

    virtual void vectorZero(Alat::GhostVector& f) const = 0;
    virtual void vectorAdd(Alat::GhostVector& y, double s, const Alat::GhostVector& x) const = 0;
    virtual void vectorEqual(Alat::GhostVector& y, const Alat::GhostVector& x) const = 0;
    virtual double vectorNorm(const Alat::GhostVector& y) const = 0;
    virtual double scalarProduct(const Alat::GhostVector& x, const Alat::GhostVector& y) const = 0;
    virtual std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const = 0;
    virtual void vectorNormPerVariable(Alat::StringDoubleMap& rnorm, const Alat::GhostVector& r) const = 0;

    virtual void matrixZero(Alat::GhostMatrix& A) const = 0;
    virtual const Alat::IoManager* getIoManager() const = 0;
    virtual void setLavrentievParameter(double parameter) const;
    virtual const Alat::ParameterFile* getParameterFile() const = 0;

    virtual DomainSolverInterface* newDomainSolver(const FadaMesh::MeshInterface* mesh) const = 0;
    virtual const DomainSolverInterface* getDomainSolver(int i) const = 0;
    virtual DomainSolverInterface* getDomainSolver(int i) = 0;
    virtual void matrixVectorProductCoupling(int i, int level, const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const = 0;
    virtual void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const = 0;
    virtual void reInitVectorForInterpolation(Alat::GhostVector& u) const=0;
    virtual void interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const = 0;
    virtual void setMeshDecomposition(const FadaMesh::MeshCompositionInterface* meshcomposition) = 0;
    virtual void reInitForInterpolation(const FadaMesh::MeshCompositionInterface* meshcomposition);

    virtual void integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const;
    virtual int getLeftSolverIndexOfCouplingSolver(int i) const = 0;
    virtual int getRightSolverIndexOfCouplingSolver(int i) const = 0;
    virtual const Alat::Map<std::string, Alat::IntSet>& getDomainsOfVar() const = 0;
    virtual const Alat::IntSet& getDomainsOfVar(std::string varname) const = 0;
    virtual const Alat::Map<std::string, Alat::IntSet>& getCouplingsOfVar() const = 0;
    virtual const Alat::IntSet& getCouplingsOfVar(std::string varname) const = 0;
    virtual void addLinearDomainSolvers(const Alat::GhostLinearSolver& linearsolver) = 0;
    virtual void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const;

    virtual void strongDirichletVectorSolution(Alat::GhostVector& u) const = 0;
    virtual void newtonProject(Alat::GhostVector& gu) const = 0;
    virtual void beforeTimestep(Alat::GhostVector& gu) const = 0;
    virtual void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const = 0;

    virtual const Alat::LinearSolverInterface* getLinearSolver(const Alat::GhostLinearSolver& v) const = 0;
    virtual Alat::LinearSolverInterface* getLinearSolver(const Alat::GhostLinearSolver& v) = 0;
    virtual const Alat::IntVector& getDomainsPermutation(int iteration) const = 0;
    virtual double computeTimeEstimator(const Alat::GhostVector& u, const Alat::GhostVector& prev, Alat::GhostVector& h1, Alat::GhostVector& h2) const = 0;
    virtual void smoothInterface(int level, int idomain, Alat::GhostVector& u) const = 0;

    virtual void constructVectorOfDomains(const Alat::GhostVector& gu) const;
    virtual void initSolverChronometer() const = 0;
    virtual void printSolverChronometer(std::ostream& os) const = 0;
    virtual void restartLinearSolver(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& gu) = 0;
    virtual void setVectorFromAllVariables( Alat::GhostVector& gu, const Alat::GhostVector& guall) const = 0;
    virtual void setVectorToAllVariables( Alat::GhostVector& guall, const Alat::GhostVector& gu) const = 0;

    virtual void setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const=0;
    virtual void setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
