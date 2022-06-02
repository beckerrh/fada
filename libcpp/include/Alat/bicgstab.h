#ifndef __Alat_BiCgStab_h
#define __Alat_BiCgStab_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class BiCgStab : public IterativeSolverWithPreconditioner
  {
public:
    ~BiCgStab();
    BiCgStab();
    BiCgStab( const BiCgStab& bicgstab);
    BiCgStab& operator=( const BiCgStab& bicgstab);
    std::string getName() const;
    BiCgStab* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
