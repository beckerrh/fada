#ifndef __Alat_NewtonRb_h
#define __Alat_NewtonRb_h

#include  "nonlinearsolver.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonRb : public NonlinearSolver
  {
protected:
    std::string _linesearchtype;

public:
    ~NewtonRb();
    NewtonRb(std::string linesearchtype);
    NewtonRb( const NewtonRb& newtonsimple);
    NewtonRb& operator=( const NewtonRb& newtonsimple);
    std::string getName() const;
    NewtonRb* clone() const;

    int getNVectors() const;
    void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f);
  };
}

/*--------------------------------------------------------------------------*/

#endif
