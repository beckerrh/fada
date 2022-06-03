#ifndef __Fada_VisitorSolverManagerOneLevel_h
#define __Fada_VisitorSolverManagerOneLevel_h

#include  "chronometer.h"
#include  "Alat/iterativesolvervisitorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class SystemVector;
}
namespace Fada
{
  class DomainSolverInterface;
  class SolverManager;

  class VisitorSolverManagerOneLevel : public Alat::IterativeSolverVisitorInterface
  {
protected:
    mutable Chronometer _chronometer;
    bool _integration;
    int _level;
    SolverManager* _solvermanager;
    const Alat::GhostVector* _linearizationpoint;
    const Alat::StringSet _variables;

    SolverManager* getSolverManager();
    const SolverManager* getSolverManager() const;
    const DomainSolverInterface* getDomainSolver(int idomain) const;
    DomainSolverInterface* getDomainSolver(int idomain);
    Alat::SystemVector* getVector(int idomain, Alat::GhostVector& gp) const;
    const Alat::SystemVector* getVector(int idomain, const Alat::GhostVector& gp) const;

public:
    ~VisitorSolverManagerOneLevel();
    VisitorSolverManagerOneLevel( SolverManager* solvermanager, int level, const Alat::StringSet& variables, bool integration);
    VisitorSolverManagerOneLevel( const VisitorSolverManagerOneLevel& visitoronelevel);
    VisitorSolverManagerOneLevel& operator=( const VisitorSolverManagerOneLevel& visitoronelevel);
    std::string getName() const;
    VisitorSolverManagerOneLevel* clone() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);

    const Alat::StringSet& getVariables() const;
    void newVector(Alat::GhostVector* u);
    int getVectorLevel() const;
    const Alat::SystemMatrixInterface* getDomainMatrix(int i, const Alat::GhostMatrix& A) const;
    Alat::SystemVector* getDomainVector(int i, Alat::GhostVector& u) const;
    const Alat::SystemVector* getDomainVector(int i, const Alat::GhostVector& u) const;

    void setLinearizationPoint(const Alat::GhostVector& linearizationpoint);
    void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;
    // const Alat::IntVector& getDomainsPermutation(int iteration) const;
    void vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const;
    void vectorZero(Alat::GhostVector& v) const;
    void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    void vectorScale(Alat::GhostVector& r, double d) const;
    double scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const;
    void solveOnDomain(int idomain, const Alat::GhostLinearSolver& linearsolverdomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void vectorEqualOnDomain(int idomain, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void matrixVectorProductCoupling(int i, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f, double d) const;
    void setsmoothtype(const Alat::GhostLinearSolver& linearsolverdomain, std::string type);
  };
}

/*--------------------------------------------------------------------------*/

#endif
