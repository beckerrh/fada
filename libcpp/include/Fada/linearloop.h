#ifndef __Fada_LinearLoop_h
#define __Fada_LinearLoop_h

#include  "Alat/ghostlinearsolver.h"
#include  "Alat/ghostmatrix.h"
#include  "loop.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class LinearLoop : public Loop
  {
protected:
    Alat::GhostVector _f;
    Alat::GhostMatrix _A;
    Alat::GhostLinearSolver _linearsolverallvars;

public:
    ~LinearLoop();
    LinearLoop();
    LinearLoop( const LinearLoop& loop);
    LinearLoop& operator=( const LinearLoop& loop);
    std::string getName() const;
    FadaEnums::looptype getType() const;

    void basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();

    void linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostVector& x, const Alat::GhostVector& b);
    void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A,  Alat::GhostVector& u);
    void constructLinearSolver(Alat::GhostMatrix& A, const Alat::GhostVector& u);
  };
}

/*---------------------------------------------------------*/

#endif
