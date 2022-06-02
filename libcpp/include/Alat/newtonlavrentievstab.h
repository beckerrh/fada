#ifndef __Alat_NewtonLavrentievStab_h
#define __Alat_NewtonLavrentievStab_h

#include  "newtonlavrentiev.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonLavrentievStab : public NewtonLavrentiev
  {
protected:
    bool _changeRegularization(int iteration, double& regularization, double modelfactor);

public:
    ~NewtonLavrentievStab();
    NewtonLavrentievStab();
    NewtonLavrentievStab( const NewtonLavrentievStab& newtonsimple);
    NewtonLavrentievStab& operator=( const NewtonLavrentievStab& newtonsimple);
    std::string getName() const;
    NewtonLavrentievStab* clone() const;

    int getNVectors() const;
    void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f);
  };
}

/*--------------------------------------------------------------------------*/

#endif
