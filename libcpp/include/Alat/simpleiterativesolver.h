#ifndef __Alat_SimpleIterativeSolver_h
#define __Alat_SimpleIterativeSolver_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SimpleIterativeSolver : public IterativeSolverWithPreconditioner
  {
private:
protected:
public:
    ~SimpleIterativeSolver();
    SimpleIterativeSolver();
    SimpleIterativeSolver( const SimpleIterativeSolver& simpleiterativesolver);
    SimpleIterativeSolver& operator=( const SimpleIterativeSolver& simpleiterativesolver);
    std::string getName() const;
    SimpleIterativeSolver* clone() const;

    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
