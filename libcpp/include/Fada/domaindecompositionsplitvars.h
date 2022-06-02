#ifndef __Fada_DomainDecompositionSplitVars_h
#define __Fada_DomainDecompositionSplitVars_h

#include  "Fada/chronometer.h"
#include  "Alat/ghostlinearsolver.h"
#include  "Alat/preconditioner.h"
#include  "Alat/stringvector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class SolverManagerInterface;
  class Chronometer;

  class DomainDecompositionSplitVars : public Alat::Preconditioner
  {
private:
    Fada::SolverManagerInterface* _solvermanager;
    int _level;
    std::string _variables;
    int _nvariables;
    Alat::Vector<Alat::StringVector> _variablesvector;
    std::string _type, _smoothtype;
    mutable Fada::Chronometer _chronometer;
    Alat::Vector<Alat::GhostLinearSolver> _domainlinearsolvers;
    const Alat::GhostMatrix _ghostmatrix;

protected:
    // const Fada::SolverManagerInterface* getSolverManager() const;
    const Alat::StringVector& variablesOfVector(int i) const;

public:
    ~DomainDecompositionSplitVars();
    DomainDecompositionSplitVars(std::string type, int level, Fada::SolverManagerInterface* solvermanager, const Alat::GhostMatrix& ghostmatrix, std::string variables);
    DomainDecompositionSplitVars( const DomainDecompositionSplitVars& preconditionerdomaindecomposition);
    DomainDecompositionSplitVars& operator=( const DomainDecompositionSplitVars& preconditionerdomaindecomposition);
    std::string getName() const;
    DomainDecompositionSplitVars* clone() const;

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
