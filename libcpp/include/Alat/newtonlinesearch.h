#ifndef __Alat_NewtonLineSearch_h
#define __Alat_NewtonLineSearch_h

#include  "nonlinearsolver.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonLineSearch : public NonlinearSolver
  {
protected:
    std::string _linesearchtype;

public:
    ~NewtonLineSearch();
    NewtonLineSearch(std::string linesearchtype);
    NewtonLineSearch( const NewtonLineSearch& newtonsimple);
    NewtonLineSearch& operator=( const NewtonLineSearch& newtonsimple);
    std::string getName() const;
    NewtonLineSearch* clone() const;

    int getNVectors() const;
    void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f);
  };
}

/*--------------------------------------------------------------------------*/

#endif
