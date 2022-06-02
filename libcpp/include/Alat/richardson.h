#ifndef __Alat_Richardson_h
#define __Alat_Richardson_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Richardson : public IterativeSolverWithPreconditioner
  {
private:
protected:
    double _omega;
public:
    ~Richardson();
    Richardson(double omega = 1.0);
    Richardson( const Richardson& richardson);
    Richardson& operator=( const Richardson& richardson);
    std::string getName() const;
    Richardson* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
