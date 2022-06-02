#ifndef __Fada_VisitorSolverManagerNonlinear_h
#define __Fada_VisitorSolverManagerNonlinear_h

#include  "Alat/nonlinearsolvervisitorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class SolverManagerInterface;

  class VisitorSolverManagerNonlinear : public Alat::NonlinearSolverVisitorInterface
  {
protected:
    SolverManagerInterface* _solvermanager;
    const SolverManagerInterface* getSolverManager() const;
    SolverManagerInterface* getSolverManager();
    int _level;

public:
    ~VisitorSolverManagerNonlinear();
    VisitorSolverManagerNonlinear(SolverManagerInterface* solvermanager, int level=0);
    VisitorSolverManagerNonlinear( const VisitorSolverManagerNonlinear& visitorsolvermanagernonlinear);
    VisitorSolverManagerNonlinear& operator=( const VisitorSolverManagerNonlinear& visitorsolvermanagernonlinear);
    std::string getName() const;
    VisitorSolverManagerNonlinear* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    std::ostream& printLoopInformation(std::ostream& os) const;
    int getVectorLevel() const;
    void newVector(Alat::GhostVector* u);
    double residual(AlatEnums::residualstatus& status, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void setLinearTolerance(double rtol, double gtol, Alat::GhostLinearSolver& B);
    int solve(AlatEnums::iterationstatus& status, const Alat::GhostLinearSolver& B, const Alat::GhostMatrix& A, Alat::GhostVector& du, const Alat::GhostVector& r) const;

    std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const;
    void vectorZero(Alat::GhostVector& u) const;
    void vectorAdd(Alat::GhostVector& u, double s, const Alat::GhostVector& du) const;
    double vectorNorm(const Alat::GhostVector& r) const;
    double scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const;
    void vectorEqual(Alat::GhostVector& u, const Alat::GhostVector& du) const;

    void constructMatrixAndLinearSolvers(AlatEnums::residualstatus& status, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A,  Alat::GhostVector& u);
    void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& h, const Alat::GhostVector& u, const Alat::GhostVector& du) const;
    void setLavrentievParameter(double parameter) const;
    double computeNormSquared(AlatEnums::residualstatus& status, const Alat::GhostVector& u, const Alat::GhostVector& du) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
