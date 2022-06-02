#ifndef __Alat_UmfSolverOneVariable_h
#define __Alat_UmfSolverOneVariable_h

#include  "iterativesolverwithvisitor.h"
#include  "preconditioner.h"
#include  "sparsematrix.h"
#include  "umfmatrixbase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VariableMatrixInterface;
}
namespace Alat
{
  class UmfSolverOneVariable : public virtual Alat::IterativeSolverWithVisitor, public virtual Alat::Preconditioner
  {
private:
    bool _reinitcalled, _computecalled;
    Alat::UmfMatrixBase _umf;
    mutable Alat::DoubleVector _u, _f;
    const Alat::VariableMatrixInterface* _variablematrix;

public:
    ~UmfSolverOneVariable();
    UmfSolverOneVariable();
    UmfSolverOneVariable(const Alat::VariableMatrixInterface* variablematrix);
    UmfSolverOneVariable( const UmfSolverOneVariable& umfsolver);
    UmfSolverOneVariable& operator=( const UmfSolverOneVariable& umfsolver);
    std::string getName() const;
    std::string getInterfaceName() const;
    UmfSolverOneVariable* clone() const;

    void setMatrix(const Alat::VariableMatrixInterface* variablematrix);
    void reInit();
    void compute();
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solve(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const;

    int getNVectors() const;
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
