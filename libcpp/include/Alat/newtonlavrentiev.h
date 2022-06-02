#ifndef __Alat_NewtonLavrentiev_h
#define __Alat_NewtonLavrentiev_h

#include  "nonlinearsolver.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonLavrentiev : public NonlinearSolver
  {
protected:
    double _firstregularization, _maxregularization;
    std::string _type;                           
    
    bool _LINEARIZATION_TEST;
    bool _USE_LINEARIZATION;
    bool _changeRegularization(int iteration, double& regularization, double modelfactor);

public:
    ~NewtonLavrentiev();
    NewtonLavrentiev(std::string type="lin");
    NewtonLavrentiev( const NewtonLavrentiev& newtonsimple);
    NewtonLavrentiev& operator=( const NewtonLavrentiev& newtonsimple);
    std::string getName() const;
    NewtonLavrentiev* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    int getNVectors() const;
    void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f);
  };
}

/*--------------------------------------------------------------------------*/

#endif
