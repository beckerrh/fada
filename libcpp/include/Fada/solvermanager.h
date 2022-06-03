#ifndef __Fada_SolverManager_h
#define __Fada_SolverManager_h

#include  "Alat/interfacebase.h"
#include  "Alat/iomanager.h"
#include  "Alat/fixarray.h"
#include  "Alat/pair.h"
#include  "Alat/vector.h"
#include  "chronometer.h"
#include  "ghostlinearsolveragent.h"
#include  "ghostmatrixagent.h"
#include  "ghostvectoragent.h"
#include  "Alat/sparsitypatternwithrowindex.h"
#include  "variablemanager.h"
#include  "FadaMesh/meshcompositioninterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SorterInterface;
  class SystemVector;
}
namespace Fada
{
  class DofManagerAllVariables;
  class DomainSolverInterface;
  class SparsityPatternWithRowIndexSoft;
  class ModelInterface;
  class ModelInterface;

  class SolverManager : public Alat::InterfaceBase
  {
protected:
    bool _onlydirectsolvers;
    FadaEnums::looptype _looptype;
    Fada::ModelInterface* _model;
    const Alat::ParameterFile* _parameterfile;
    mutable const FadaMesh::MeshCompositionInterface* _meshcomposition;
    const Alat::IoManager* _io_manager;

    GhostMatrixAgent _ghost_matrix_agent;
    GhostLinearSolverAgent _ghost_linear_solver_agent;
    GhostVectorAgent _ghost_vector_agent;
    double _time, _deltat;

    // void setDomainsOfCoupling(Alat::Vector<Alat::FixArray<2, int> >& domainsofcoupling, Alat::SparsityPattern& _domainneighbours, Alat::Map<Alat::IntPair, int>& _couplingofdomains) const;
    void initSolverChronometer() const;
    void printSolverChronometer(std::ostream& os) const;

    Fada::DomainSolverInterface* newDomainSolver(const FadaMesh::MeshInterface* mesh) const;

    // gestion des solvers
    // Alat::IntVector _vectorOfCouplings;
    // mutable Alat::Vector<Alat::IntVector> _domainspermutation;
    Alat::Vector<DomainSolverInterface*> _domainsolvers;
    // Alat::Map<std::string, Alat::IntSet> _domainsofvar;
    Alat::StringSet _variables;

    void _initDomainsOfVar();
    void _writePostProcessScalars(const Alat::Map<std::string, Alat::IntSet>& ppvarswithoutfem, const Alat::SystemVector& vectorallvars, std::ofstream& file, std::string first_column="t") const;
    void _writePostProcessScalarsHeader(const Alat::Map<std::string, Alat::IntSet>& ppvarswithoutfem, std::ofstream& file, std::string first_column="t") const;

    // Alat::Vector<Alat::FixArray<2, int> > _domainsofcoupling;
    mutable Alat::Vector<bool> _sortcriterion;

    typedef Alat::Set<Alat::Pair<int, int> > SortedCouplingIndices;
    Alat::SparsityPattern _domainneighbours;
    Alat::Map<Alat::IntPair, int> _couplingofdomains;

    // Alat::Vector<Alat::SorterInterface*> _domainsorters;
    std::string _linearsolver;
    int _nlevels, _ncellsdirect;
    Alat::Map<std::string, Alat::IntSet> _ppvarswithoutfem;
    typedef Alat::Set<Alat::GhostLinearSolver> GhostLinearSolverSet;
    GhostLinearSolverSet _domainSolversOfSolver;

    mutable Fada::Chronometer _chronometer;
    // int getLeftSolverIndexOfCouplingSolver(int i) const;
    // int getRightSolverIndexOfCouplingSolver(int i) const;
    const Alat::ParameterFile* getParameterFile() const;
    const FadaMesh::MeshCompositionInterface* getMeshComposition() const;
    DomainSolverInterface*& getDomainSolverPointer(int i);
    Alat::LinearSolverInterface* newLinearSolver(const Alat::GhostLinearSolver& linearsolver);
    void _initStringDoubleMapForVectors(Alat::StringDoubleMap& map) const;
    void _reInitSansSolvers();

public:
    ~SolverManager();
    SolverManager();
    SolverManager( const SolverManager& solvermanager);
    SolverManager& operator=( const SolverManager& solvermanager);
    std::string getName() const;
    std::string getInterfaceName() const {return "_not_expected_";}

    void basicInit(Fada::ModelInterface* model, const FadaMesh::MeshCompositionInterface* meshcomposition, const Alat::IoManager& io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile = NULL);
    void reInit();
    void reInitMatrixAndLinearSolver();

    int getNlevels() const;
    int getNDomainSolvers() const;
    // int getNCouplingSolvers() const;
    void printChronometer(std::string filename) const;

    const Alat::LinearSolverInterface* getLinearSolver(const Alat::GhostLinearSolver& v) const;
    Alat::LinearSolverInterface* getLinearSolver(const Alat::GhostLinearSolver& v);

    // const Alat::Map<std::string, Alat::IntSet>& getDomainsOfVar() const;
    // const Alat::IntSet& getDomainsOfVar(std::string varname) const;
    // const Alat::Map<std::string, Alat::IntSet>& getCouplingsOfVar() const;
    // const Alat::IntSet& getCouplingsOfVar(std::string varname) const;

    void addLinearDomainSolvers(const Alat::GhostLinearSolver& linearsolver);

    std::ostream& printLoopInformation(std::ostream& os) const;

    const Alat::IoManager* getIoManager() const;
    DomainSolverInterface* getDomainSolver(int i);
    const DomainSolverInterface* getDomainSolver(int i) const;

    void registerVector(const Alat::GhostVector& v);
    void registerVectorIfNotExists(const Alat::GhostVector& v);
    void registerMatrix( Alat::GhostMatrix& v);
    void registerLinearSolver(const Alat::GhostLinearSolver& v);
    double getTime() const;
    double getDeltaT() const;
    void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit);
    void setTimeScheme(std::string time_discretization);
    void readUnknownVariables(Alat::GhostVector& v, int number = -1);
    void writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& gv, int number = -1) const;
    void writeVariablesInfo() const;
    void reInitVectorForInterpolation(Alat::GhostVector& u) const;

    void writePostProcessVariables(const Alat::GhostVector& v, int number = -1) const;
    void writePostProcessVariablesDynamic(const Alat::GhostVector& v, int number) const;
    void setLavrentievParameter(double parameter) const;
    // void matrixVectorProductCoupling(int i, int level, const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const;
    void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const;

    void vectorZero(Alat::GhostVector& f) const;
    void vectorAdd(Alat::GhostVector& y, double s, const Alat::GhostVector& x) const;
    void vectorEqual(Alat::GhostVector& y, const Alat::GhostVector& x) const;
    double vectorNorm(const Alat::GhostVector& y) const;
    double scalarProduct(const Alat::GhostVector& x, const Alat::GhostVector& y) const;
    std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const;
    void vectorNormPerVariable(Alat::StringDoubleMap& rnorm, const Alat::GhostVector& r) const;
    void matrixZero(Alat::GhostMatrix& A) const;
    void constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& f) const;
    void postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level=0) const;
    void initSolution(Alat::GhostVector& u);
    void constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const;
    void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& du) const;
    void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, const Alat::GhostVector& u);
    void setMeshDecomposition(const FadaMesh::MeshCompositionInterface* meshcomposition);
    void interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const;
    void reInitForInterpolation(const FadaMesh::MeshCompositionInterface* meshcomposition);
    void integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const;
    void constructLinearSolver(Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& u);
    void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const;

    void strongDirichletVectorSolution(Alat::GhostVector& u) const;
    void newtonProject(Alat::GhostVector& gu) const;
    void beforeTimestep(Alat::GhostVector& gu) const;
    void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const;

    int linearSolve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, const Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    // const Alat::IntVector& getDomainsPermutation(int iteration) const;
    double computeTimeEstimator(const Alat::GhostVector& u, const Alat::GhostVector& prev, Alat::GhostVector& h1, Alat::GhostVector& h2) const;
    void restartLinearSolver(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& gu);
    void setVectorFromAllVariables( Alat::GhostVector& gu, const Alat::GhostVector& guall) const;
    void setVectorToAllVariables( Alat::GhostVector& guall, const Alat::GhostVector& gu) const;
    void computePostProcessVector(Alat::SystemVector& vectorallvars, const Alat::GhostVector& v) const;

    void setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const;
    void setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
