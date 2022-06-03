#ifndef __Fada_TimeStepPilot_h
#define __Fada_TimeStepPilot_h

#include  "timestepcriterion.h"
#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IterationInfo;
}
namespace Fada
{
  class SolverManager;
  
  class TimeStepPilot : Alat::InterfaceBase
  {
public:
    std::string _criterianame;
    // std::string _criteriatype;

protected:
    double _increase_dt, _decrease_dt;
    double _tend, _tinit;
    double _residual, _stationary_residual_gtol, _stationary_residual_rtol, _first_residual;
    int _nitermax, _frequency, _n_redo_time_step;
    std::string _stop_info;
    bool _redo_time_step, _stop_dynamic_loop;
    const SolverManager* _solvermanager;
    double _cut_dt;
    TimeStepCriterion* _timestepcriteria;
    mutable const TimeStepPilotInformation* _timesteppilotinformation;

public:
    double _dtmin, _dtmax;
    bool exactfinaltime;
    
    ~TimeStepPilot();
    TimeStepPilot();
    TimeStepPilot( const TimeStepPilot& timesteppilot);
    TimeStepPilot& operator=( const TimeStepPilot& timesteppilot);
    std::string getName() const;
    std::string getInterfaceName() const;

    double getEndTime() const;
    double getInitTime() const;
    bool stopDynamicLoop() const;
    bool redoTimeStep() const;
    double getResidual() const;
    double& getResidual();
    double getFirstResidual() const;
    double& getFirstResidual();
    int getFrequency() const;
    int getNIterationMax() const;
    int& getNIterationMax();
    std::string& getStopInfo();
    std::string getStopInfo() const;
    void setNewtonProblem(TimeData& timedata);
    int getNRedoTimeStep() const;
    int& getNRedoTimeStep();

    void basicInit(const Alat::ParameterFile* parameterfile, const SolverManager* solvermanager, TimeData& timedata, Alat::IterationInfo& infonl);
    void checkTimeStep(const TimeData& timedata, TimeStepPilotInformation* timesteppilotinformation, int iteration);
    void computeNewDeltaT(TimeData& timedata, const TimeStepPilotInformation* timesteppilotinformation, int iteration);
    void setTimeStepPilotInformation(TimeStepPilotInformation& timesteppilotinformation) const;

  };
}

/*--------------------------------------------------------------------------*/

#endif
