#ifndef __Alat_FGmres_h
#define __Alat_FGmres_h

#include  "doublevector.h"
#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class FGmres : public IterativeSolverWithPreconditioner
  {
protected:
    int _niteration;

public:
    ~FGmres();
    FGmres(int niteration);
    FGmres( const FGmres& gmres);
    FGmres& operator=( const FGmres& gmres);
    std::string getName() const;
    FGmres* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
