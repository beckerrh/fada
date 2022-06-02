#ifndef __Alat_Cg_h
#define __Alat_Cg_h

#include  "iterativesolverwithpreconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
class Cg : public IterativeSolverWithPreconditioner
{
private:
protected:
public:
  ~Cg();
  Cg();
  Cg( const Cg& cg);
  Cg& operator=( const Cg& cg);
  std::string getName() const;
  Cg* clone() const;

  int getNVectors() const;
  void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
