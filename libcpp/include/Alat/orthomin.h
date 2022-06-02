#ifndef __Alat_Orthomin_h
#define __Alat_Orthomin_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Orthomin : public IterativeSolverWithPreconditioner
  {
public:
    ~Orthomin();
    Orthomin();
    Orthomin( const Orthomin& orthomin);
    Orthomin& operator=( const Orthomin& orthomin);
    std::string getName() const;
    Orthomin* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
