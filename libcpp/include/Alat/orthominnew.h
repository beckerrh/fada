#ifndef __Alat_OrthominNew_h
#define __Alat_OrthominNew_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class OrthominNew : public IterativeSolverWithPreconditioner
  {
public:
    ~OrthominNew();
    OrthominNew();
    OrthominNew( const OrthominNew& orthomin);
    OrthominNew& operator=( const OrthominNew& orthomin);
    std::string getName() const;
    OrthominNew* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
