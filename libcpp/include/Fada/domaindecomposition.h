#ifndef __Fada_DomainDecomposition_h
#define __Fada_DomainDecomposition_h

#include  "Fada/chronometer.h"
#include  "Alat/ghostlinearsolver.h"
#include  "Alat/preconditioner.h"
#include  "Alat/stringvector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class SolverManager;
  class Chronometer;

  class DomainDecomposition : public Alat::Preconditioner
  {
private:
    Fada::SolverManager* _solvermanager;
    int _level;
    std::string _type, _smoothtype;
    mutable Fada::Chronometer _chronometer;
    const Alat::GhostMatrix _ghostmatrix;
    const Alat::GhostLinearSolver _linearsolverdomain;

protected:
    // const Fada::SolverManagerInterface* getSolverManager() const;
    // const Alat::StringVector& variablesOfVector(int i) const;

public:
    ~DomainDecomposition();
    DomainDecomposition(std::string type, int level, Fada::SolverManager* solvermanager, const Alat::GhostMatrix& ghostmatrix, const Alat::GhostLinearSolver& linearsolverdomain);
    DomainDecomposition( const DomainDecomposition& preconditionerdomaindecomposition);
    DomainDecomposition& operator=( const DomainDecomposition& preconditionerdomaindecomposition);
    std::string getName() const;
    DomainDecomposition* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void reInit();
    void computePreconditioner();
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    int getNVectors() const;
    void setsmoothtype(std::string smoothtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
