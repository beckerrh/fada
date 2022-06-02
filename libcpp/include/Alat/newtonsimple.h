#ifndef __Alat_NewtonSimple_h
#define __Alat_NewtonSimple_h

#include  "nonlinearsolver.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonSimple : public NonlinearSolver
  {
protected:
public:
    ~NewtonSimple();
    NewtonSimple();
    NewtonSimple( const NewtonSimple& newtonsimple);
    NewtonSimple& operator=( const NewtonSimple& newtonsimple);
    std::string getName() const;
    NewtonSimple* clone() const;

    int getNVectors() const;
    void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f);
  };
}

/*--------------------------------------------------------------------------*/

#endif
