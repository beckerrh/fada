#ifndef __Alat_UmfSolverSomeVariables_h
#define __Alat_UmfSolverSomeVariables_h

#include  "intvector.h"
#include  "stringvector.h"
#include  "iterativesolverwithvisitor.h"
#include  "sparsematrix.h"
#include  "umfmatrixbase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class SystemVector;
  class VariableVectorInterface;
  class VectorInterface;
}
namespace Alat
{
  class UmfSolverSomeVariables : public Alat::IterativeSolverWithVisitor
  {
private:
    Alat::UmfMatrixBase _umf;
    Alat::StringVector _variables;
    Alat::IntVector _offsets;
    mutable Alat::DoubleVector _u, _f;
    bool _reinitcalled, _computecalled;
    const Alat::SystemMatrixInterface* _systemmatrix;

public:
    ~UmfSolverSomeVariables();
    UmfSolverSomeVariables();
    UmfSolverSomeVariables( const UmfSolverSomeVariables& structuredumfsolver);
    UmfSolverSomeVariables& operator=( const UmfSolverSomeVariables& structuredumfsolver);
    std::string getName() const;
    UmfSolverSomeVariables* clone() const;
    bool hasIterationInfo() const;

    void setMatrix(int level, const Alat::SystemMatrixInterface* systemmatrix, const Alat::StringVector& variables, const Alat::IntVector& ncompofvars, const Alat::IntVector& nofvars);
    void reInit();
    void compute();
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solve(AlatEnums::iterationstatus& status, Alat::SystemVector* u, const Alat::SystemVector* f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
