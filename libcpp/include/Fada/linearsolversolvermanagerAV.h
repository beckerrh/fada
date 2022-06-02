#ifndef __Fada_LinearSolverSolverManagerAV_h
#define __Fada_LinearSolverSolverManagerAV_h

#include  "Fada/chronometer.h"
#include  "Alat/linearsolverinterface.h"
#include  "Fada/linearsolvervector.h"
#include  "Fada/multigrid.h"
#include  "Alat/vector.h"
#include  "linearsolversolvermanager.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostLinearSolver;
  class GhostMatrix;
}

namespace Fada
{
  class LinearSolverSolverManagerAV : public virtual Alat::LinearSolverInterface, public virtual Alat::PreconditionerInterface
  {
protected:
    int _nlevels;
    const Alat::GhostLinearSolver _domainlinearsolver;
    const Alat::GhostMatrix _ghostmatrix;
    Fada::SolverManagerInterface* _solvermanager;
    Alat::Vector<LinearSolverSolverManager*> _linearsolvers;
    Alat::Vector<Alat::StringVector> _variables;
    mutable Alat::Vector<Alat::GhostVector> _memory;

public:
    ~LinearSolverSolverManagerAV();
    LinearSolverSolverManagerAV(int nlevels, const Alat::GhostLinearSolver& domainlinearsolver, Fada::SolverManagerInterface* solvermanager);
    LinearSolverSolverManagerAV( const LinearSolverSolverManagerAV& multilevellinearsolver);
    LinearSolverSolverManagerAV& operator=( const LinearSolverSolverManagerAV& multilevellinearsolver);
    std::string getName() const;
    LinearSolverSolverManagerAV* clone() const;
    std::string getInterfaceName() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor = NULL);
    void reInit();
    void compute();
    void computePreconditioner();
    void restart();
    bool hasIterationInfo() const;
    void setsmoothtype(std::string smoothtype);

    std::ostream& printLoopInformation(std::ostream& os) const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;

    int getNVectors() const;
    int getNLevels() const;
    void setTolerance(double rtol, double gtol);
    int getNumberOfIterations() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
