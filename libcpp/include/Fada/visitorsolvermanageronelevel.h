#ifndef __Fada_VisitorSolverManagerOneLevel_h
#define __Fada_VisitorSolverManagerOneLevel_h

#include  "chronometer.h"
#include  "Alat/iterativesolvervisitorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class SystemVectorInterface;
}
namespace Fada
{
  class DomainSolverInterface;
  class SolverManagerInterface;

  class VisitorSolverManagerOneLevel : public Alat::IterativeSolverVisitorInterface
  {
protected:
    mutable Chronometer _chronometer;
    bool _integration;
    int _level;
    SolverManagerInterface* _solvermanager;
    const Alat::GhostVector* _linearizationpoint;
    const Alat::StringSet _variables;

    SolverManagerInterface* getSolverManager();
    const SolverManagerInterface* getSolverManager() const;
    const DomainSolverInterface* getDomainSolver(int idomain) const;
    DomainSolverInterface* getDomainSolver(int idomain);
    Alat::SystemVectorInterface* getVector(int idomain, Alat::GhostVector& gp) const;
    const Alat::SystemVectorInterface* getVector(int idomain, const Alat::GhostVector& gp) const;

public:
    ~VisitorSolverManagerOneLevel();
    VisitorSolverManagerOneLevel( SolverManagerInterface* solvermanager, int level, const Alat::StringSet& variables, bool integration);
    VisitorSolverManagerOneLevel( const VisitorSolverManagerOneLevel& visitoronelevel);
    VisitorSolverManagerOneLevel& operator=( const VisitorSolverManagerOneLevel& visitoronelevel);
    std::string getName() const;
    VisitorSolverManagerOneLevel* clone() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);

    const Alat::StringSet& getVariables() const;
    void newVector(Alat::GhostVector* u);
    int getVectorLevel() const;
    const Alat::SystemMatrixInterface* getDomainMatrix(int i, const Alat::GhostMatrix& A) const;
    Alat::SystemVectorInterface* getDomainVector(int i, Alat::GhostVector& u) const;
    const Alat::SystemVectorInterface* getDomainVector(int i, const Alat::GhostVector& u) const;

    void setLinearizationPoint(const Alat::GhostVector& linearizationpoint);
    void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;
    const Alat::IntVector& getDomainsPermutation(int iteration) const;
    void vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const;
    void vectorZero(Alat::GhostVector& v) const;
    void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    void vectorScale(Alat::GhostVector& r, double d) const;
    double scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const;
    void solveOnDomain(int idomain, const Alat::GhostLinearSolver& linearsolverdomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void vectorEqualOnDomain(int idomain, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void matrixVectorProductCoupling(int i, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f, double d) const;
    void setsmoothtype(const Alat::GhostLinearSolver& linearsolverdomain, std::string type);
    void smoothInterface(int idomain, Alat::GhostVector& u) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
