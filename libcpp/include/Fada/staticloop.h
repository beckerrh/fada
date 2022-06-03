#ifndef  __Fada_StaticLoop_h
#define  __Fada_StaticLoop_h

#include  "linearloop.h"
#include  "Alat/newtondata.h"
#include  "Alat/nonlinearsolverinterface.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class NonlinearSolverInterface;

  class StaticLoop : public LinearLoop
  {
protected:
    Alat::NonlinearSolverInterface* _nlsolver;
    std::string _modelname;
    Alat::NewtonData _newtondata;

public:
    ~StaticLoop();
    StaticLoop();
    StaticLoop( const StaticLoop& loop);
    StaticLoop& operator=( const StaticLoop& loop);
    std::string getName() const;
    FadaEnums::looptype getType() const;

    void basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    const Alat::NonlinearSolverInterface* getNonLinearSolver() const;
    Alat::NonlinearSolverInterface* getNonLinearSolver();
    //! /todo sigma is the coefficient of Id in time-stepping, this is not nice
    void nonlinearSolve(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& f);
    void residual(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x) const;
    void run();
    void restartLinearSolver(const Alat::GhostVector& gu);
  };
}

/*---------------------------------------------------------*/

#endif
