#ifndef __Alat_RichardsonMonotone_h
#define __Alat_RichardsonMonotone_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
class RichardsonMonotone : public IterativeSolverWithPreconditioner
{
private:
protected:
public:
  ~RichardsonMonotone();
  RichardsonMonotone();
  RichardsonMonotone( const RichardsonMonotone& richardsonmonotone);
  RichardsonMonotone& operator=( const RichardsonMonotone& richardsonmonotone);
  std::string getName() const;
  RichardsonMonotone* clone() const;

  int getNVectors() const;
  void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
