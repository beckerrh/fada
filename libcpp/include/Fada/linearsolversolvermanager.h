#ifndef __Fada_LinearSolverSolverManager_h
#define __Fada_LinearSolverSolverManager_h

#include  "Fada/chronometer.h"
#include  "Alat/linearsolverinterface.h"
#include  "Fada/linearsolvervector.h"
#include  "Fada/multigrid.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostLinearSolver;
  class GhostMatrix;
}

namespace Fada
{
    class LinearSolverSolverManager : public virtual Alat::LinearSolverInterface, public virtual Alat::PreconditionerInterface
  {
protected:
    bool _basicinitcalled, _integration;
    std::string _coarsesolver, _smootheriteration, _dirname, _blockname;
    const Alat::GhostLinearSolver _domainlinearsolver;
    const Alat::GhostMatrix _ghostmatrix;
    Fada::SolverManager* _solvermanager;
    Fada::Multigrid _multigrid;
    mutable Fada::LinearSolverVector _linearsolvers;
    mutable Fada::Chronometer _chronometer;
    std::string _ddvariables;
    const Alat::StringVector& _variables;

    void readParameters(const Alat::ParameterFile* parameterfile, std::string blockname);

public:
    ~LinearSolverSolverManager();
    LinearSolverSolverManager(const Alat::StringVector& variables, int nlevels, const Alat::GhostLinearSolver& domainlinearsolver, Fada::SolverManager* solvermanager);
    LinearSolverSolverManager( const LinearSolverSolverManager& multilevellinearsolver);
    LinearSolverSolverManager& operator=( const LinearSolverSolverManager& multilevellinearsolver);
    std::string getName() const;
    LinearSolverSolverManager* clone() const;
    std::string getInterfaceName() const;
    void printChronometer(std::ostream& os) const;

    const Fada::LinearSolverVector& getLinearSolvers() const;
    Fada::LinearSolverVector& getLinearSolvers();
    bool hasIterationInfo() const;
    const Alat::IterationInfo* getIterationInfo() const;
    Alat::IterationInfo* getIterationInfo();

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    void reInit();
    void compute();
    void computePreconditioner();
    void restart();

    std::ostream& printLoopInformation(std::ostream& os) const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;

    int getNVectors() const;
    int getNLevels() const;
    Alat::LinearSolverInterface*& getLinearSolverPointer(int level);
    Alat::LinearSolverInterface* getLinearSolver(int level);
    const Alat::LinearSolverInterface* getLinearSolver(int level) const;
    bool allSolversSet() const;
    void setsmoothtype(std::string smoothtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
