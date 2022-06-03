#include  "Alat/filescanner.h"
#include  "Alat/iterationinfo.h"
#include  "Alat/newtondata.h"
#include  "Fada/timedata.h"
#include  "Fada/timestepcriterion.h"
#include  "Fada/timesteppilot.h"
#include  "Fada/timesteppilotinformation.h"
#include  "Alat/set.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <cmath>
#include  <iostream>
#include  <limits>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeStepPilot::~TimeStepPilot(){}
TimeStepPilot::TimeStepPilot() : Alat::InterfaceBase(), _redo_time_step(0), _stop_dynamic_loop(0), _solvermanager(NULL), _n_redo_time_step(0), _criterianame("none"), _timesteppilotinformation(NULL){}
TimeStepPilot::TimeStepPilot( const TimeStepPilot& timesteppilot) : Alat::InterfaceBase(timesteppilot)
{
  assert(0);
}

TimeStepPilot& TimeStepPilot::operator=( const TimeStepPilot& timesteppilot)
{
  assert(0);
  return *this;
}

std::string TimeStepPilot::getName() const
{
  return "TimeStepPilot";
}

std::string TimeStepPilot::getInterfaceName() const
{
  return "TimeStepPilot";
}

/*--------------------------------------------------------------------------*/
void TimeStepPilot::basicInit(const Alat::ParameterFile* parameterfile, const SolverManager* solvermanager, TimeData& timedata, Alat::IterationInfo& infonl)
{
  _solvermanager = solvermanager;
  Alat::DataFormatHandler dataformathandler;
  double value_increase_dt;
  double value_decrease_dt;
  double value_reject_dt;
  int ignore_first;
  int newton_niter_increase_dt;
  int newton_niter_decrease_dt;
  // int newton_niter_reject_dt;
  int linear_niter_increase_dt;
  int linear_niter_decrease_dt;
  int linear_niter_reject_dt;
  int matrix_nredo_increase_dt;
  int matrix_nredo_decrease_dt;
  int matrix_nredo_reject_dt;
  dataformathandler.insert("t_end", &_tend, 1.0);
  dataformathandler.insert("t_init", &_tinit, 0.);
  dataformathandler.insert("dtmin", &_dtmin, 1.e-8);
  dataformathandler.insert("dtmax", &_dtmax, 1000.0);
  dataformathandler.insert("dt", &timedata.deltat, 0.001);
  dataformathandler.insert("cut_dt", &_cut_dt, 0.5);
  dataformathandler.insert("increase_dt", &_increase_dt, 1.1);
  dataformathandler.insert("decrease_dt", &_decrease_dt, 0.8);
  dataformathandler.insert("stationary_residual_gtol", &_stationary_residual_gtol, 1e-14);
  dataformathandler.insert("stationary_residual_rtol", &_stationary_residual_rtol, 1e-10);
  dataformathandler.insert("nitermax", &_nitermax, 1000000);
  dataformathandler.insert("frequency", &_frequency, 1);
  dataformathandler.insert("linear_niter_increase_dt", &linear_niter_increase_dt, 40);
  dataformathandler.insert("linear_niter_decrease_dt", &linear_niter_decrease_dt, 40);
  dataformathandler.insert("matrix_nredo_increase_dt", &matrix_nredo_increase_dt, 2);
  dataformathandler.insert("matrix_nredo_decrease_dt", &matrix_nredo_decrease_dt, 3);
  dataformathandler.insert( "matrix_nredo_reject_dt", &matrix_nredo_reject_dt, std::numeric_limits<int>::max() );
  dataformathandler.insert( "linear_niter_reject_dt", &linear_niter_reject_dt, std::numeric_limits<int>::max() );
  dataformathandler.insert("ignore_first", &ignore_first, 0);
  dataformathandler.insert("exactfinaltime", &exactfinaltime, false);
  if(_criterianame == "estimator")
  {
    dataformathandler.insert("estimator_increase_dt", &value_increase_dt, 0.01);
    dataformathandler.insert("estimator_decrease_dt", &value_decrease_dt, 0.02);
    dataformathandler.insert("estimator_reject_dt", &value_reject_dt, 0.05);
  }
  else if(_criterianame == "trustregion")
  {
    dataformathandler.insert("trustregion_increase_dt", &value_increase_dt, 0.1);
    dataformathandler.insert("trustregion_decrease_dt", &value_decrease_dt, 0.5);
    dataformathandler.insert("trustregion_reject_dt", &value_reject_dt, 10.);
  }
  else if(_criterianame == "constant")
  {
    value_increase_dt = 0;
    value_decrease_dt = 0;
    value_reject_dt = 0;
  }
  else
  {
    _error_string("TimeStepPilot", "bad time step criterion\n", _criterianame);
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "TimeStepPilot", 0);

  if(_nitermax == -1)
  {
    _nitermax = std::numeric_limits<int>::max();
  }
  double rho = _timesteppilotinformation->_rhomatrix;
  double rtol = _timesteppilotinformation->_rtol;
  int maxiter =  (int) log(rtol)/log(rho);
  infonl.getMaxiter() = maxiter + 4;


  newton_niter_increase_dt = infonl.getMaxiter()-2;
  newton_niter_decrease_dt = infonl.getMaxiter();
  // newton_niter_reject_dt = info.getMaxiter();

  // if( newton_niter_decrease_dt < log(rtol)/log(rho) )
  // {
  //   std::cerr<<"****TimeStepPilot::Warning newton_niter_increase_dt redefinition due to rtol en rho values:("<<rtol<<","<<rho<<")=>"<<newton_niter_decrease_dt<<"-> ";
  //   newton_niter_increase_dt = int( log(rtol)/log(rho) );
  //   std::cerr<<newton_niter_increase_dt<<'\n';
  //   newton_niter_decrease_dt = newton_niter_increase_dt+2;
  //   newton_niter_reject_dt = newton_niter_increase_dt+4;
  //   info.getMaxiter() = newton_niter_reject_dt;
  //   std::cerr << "new newton_niter_increase_dt="<<newton_niter_increase_dt<<"\n";
  //   std::cerr << "new maxiter="<<info.getMaxiter()<<"\n";
  //   // assert(0);
  // }
  timedata.time = _tinit;
  timedata.deltatold = timedata.deltat;
  _timestepcriteria = new TimeStepCriterion(value_increase_dt, value_decrease_dt, value_reject_dt, ignore_first, _criterianame);
  _timestepcriteria->addCriterion("newton", newton_niter_increase_dt, newton_niter_decrease_dt);
  _timestepcriteria->addCriterion("matrix", matrix_nredo_increase_dt, matrix_nredo_decrease_dt, matrix_nredo_reject_dt);
  _timestepcriteria->addCriterion("linear", linear_niter_increase_dt, linear_niter_decrease_dt, linear_niter_reject_dt);
}

/*--------------------------------------------------------------------------*/
void TimeStepPilot::setTimeStepPilotInformation(TimeStepPilotInformation& timesteppilotinformation) const
{
  _timesteppilotinformation = &timesteppilotinformation;
  if(_criterianame == "estimator")
  {
    timesteppilotinformation.compute_estimator = true;
  }
  else if(_criterianame == "trustregion")
  {
    timesteppilotinformation.compute_trustregion = true;
  }
}

/*---------------------------------------------------------*/
void TimeStepPilot::checkTimeStep(const TimeData& timedata, TimeStepPilotInformation* timesteppilotinformation, int iteration)
{
  if(iteration == 1)
  {
    getFirstResidual() = timesteppilotinformation->newtondata.getFirstResidual();
  }
  _residual = timesteppilotinformation->newtondata.getFirstResidual();
  if( _residual <= _stationary_residual_rtol*getFirstResidual() )
  {
    std::cerr << "TimeStepPilot::checkTimeStep() _residual="<<_residual<<" _stationary_residual_rtol="<<_stationary_residual_rtol<<" _first_residual="<<_first_residual<<"\n";
    _stop_dynamic_loop = true;
    getStopInfo() = "Stationary Solution Attained (relative tol)";
  }
  else if( _residual <= _stationary_residual_gtol )
  {
    _stop_dynamic_loop = true;
    getStopInfo() = "Stationary Solution Attained (global tol)";
  }
  else if( iteration >= getNIterationMax() )
  {
    _stop_dynamic_loop = true;
    getStopInfo() = "Maximum Number Of Time Steps Attained";
  }
  else if(fabs( timedata.time-getEndTime() ) < timedata.deltat*1.e-02)
  {
    _stop_dynamic_loop = true;
    getStopInfo() = "Final Time Attained";
  }
  else if( timedata.time > getEndTime() )
  {
    _stop_dynamic_loop = true;
    getStopInfo() = "Final Time Exceeded";
  }
  else
  {
    _stop_dynamic_loop = false;
    _redo_time_step = false;
  }
  if( _timestepcriteria->reject(*timesteppilotinformation) and not _timestepcriteria->ignore( iteration ) )
  {
    if(_criterianame == "constant")
    {
      _stop_dynamic_loop = true;
      _redo_time_step = false;
    }
    else
    {
      _redo_time_step = true;
    }
  }
}

/*---------------------------------------------------------*/
void TimeStepPilot::computeNewDeltaT(TimeData& timedata, const TimeStepPilotInformation* timesteppilotinformation, int iteration)
{
  if(_criterianame == "constant")
  {
    return;
  }
  // std::cerr << "TimeStepPilot::computeNewDeltaT() redoTimeStep()="<<redoTimeStep()<< " _timestepcriteria.size()="<< _timestepcriteria.size()<<"\n";
  if( redoTimeStep() )
  {
    // timedata.deltatold = timedata.deltat;
    timedata.deltat *= _cut_dt;
    // std::cerr << "TimeStepPilot::computeNewDeltaT() redoTimeStep() _cut_dt="<<_cut_dt << " timedata.deltat="<<timedata.deltat<<"\n";
    if( timedata.deltat <= _dtmin )
    {
      _error_string("computeNewDeltaT", "smallest dt attained without convergence\n", timedata.deltat);
    }
  }
  else
  {
    timedata.deltatold = timedata.deltat;
    if( _timestepcriteria->ignore( iteration ) )
    {
      std::cerr << "IGNORING " <<_timestepcriteria->_name << "\n";
      return;
    }
    _timestepcriteria->checkStatus(*timesteppilotinformation);
    const Alat::StringSet& increasenames = _timestepcriteria->_increasenames;
    const Alat::StringSet& decreasenames = _timestepcriteria->_decreasenames;
    // std::cerr << "TimeStepPilot::computeNewDeltaT() increasenames = " << increasenames << " decreasenames=" << decreasenames<<"\n";
    bool increase = false, decrease = false;
    increase = _timestepcriteria->increase();
    decrease = _timestepcriteria->decrease();
    // std::cerr << "TimeStepPilot::computeNewDeltaT() increase = " << increase << " decrease=" << decrease<<"\n";
    if(increase && decrease)
    {
      _warning_string( "computeNewDeltaT", "simultaneous increase and decrease\n");
      increase = false;
    }
    if(increase)
    {
      std::stringstream ss;
      for(Alat::StringSet::const_iterator it = increasenames.begin(); it != increasenames.end(); it++)
      {
        ss << *it<<" ";
      }
      _message_string( "computeNewDeltaT", "increasing due to: " + ss.str() );
      timedata.deltat *= _increase_dt;
    }
    else if(decrease)
    {
      std::cerr<<"*****DECREASE"<<'\n';
      std::stringstream ss;
      for(Alat::StringSet::const_iterator it = decreasenames.begin(); it != decreasenames.end(); it++)
      {
        ss << *it<<" ";
        _message_string( "computeNewDeltaT", "decreasing due to: " + ss.str() );
      }
      timedata.deltat *= _decrease_dt;
    }
    // std::cerr << "TimeStepPilot::computeNewDeltaT() NOT redoTimeStep() _increase_dt="<<_increase_dt << " _decrease_dt="<<_decrease_dt<< " timedata.deltat="<<timedata.deltat<<"\n";
  }
}

/*---------------------------------------------------------*/
void TimeStepPilot::setNewtonProblem(TimeData& timedata)
{
  timedata.deltat *= _cut_dt;
  // std::cerr << "TimeStepPilot::setNewtonProblem() _cut_dt="<<_cut_dt << " timedata.deltat="<<timedata.deltat<<"\n";
  if( timedata.deltat <= _dtmin )
  {
    _error_string("computeNewDeltaT", "smallest dt attained without convergence\n", timedata.deltat);
  }
  _redo_time_step = true;
}

/*---------------------------------------------------------*/
double TimeStepPilot::getEndTime() const
{
  return _tend;
}

/*---------------------------------------------------------*/
double TimeStepPilot::getInitTime() const
{
  return _tinit;
}

/*---------------------------------------------------------*/
bool TimeStepPilot::stopDynamicLoop() const
{
  return _stop_dynamic_loop;
}

/*---------------------------------------------------------*/
bool TimeStepPilot::redoTimeStep() const
{
  return _redo_time_step;
}

/*---------------------------------------------------------*/
int TimeStepPilot::getNIterationMax() const
{
  return _nitermax;
}

/*---------------------------------------------------------*/
int& TimeStepPilot::getNIterationMax()
{
  return _nitermax;
}

/*---------------------------------------------------------*/
double& TimeStepPilot::getResidual()
{
  return _residual;
}

/*---------------------------------------------------------*/
double TimeStepPilot::getResidual() const
{
  return _residual;
}

/*---------------------------------------------------------*/
double& TimeStepPilot::getFirstResidual()
{
  return _first_residual;
}

/*---------------------------------------------------------*/
double TimeStepPilot::getFirstResidual() const
{
  return _first_residual;
}

/*---------------------------------------------------------*/
std::string& TimeStepPilot::getStopInfo()
{
  return _stop_info;
}

/*---------------------------------------------------------*/
std::string TimeStepPilot::getStopInfo() const
{
  return _stop_info;
}

/*---------------------------------------------------------*/
int TimeStepPilot::getNRedoTimeStep() const
{
  return _n_redo_time_step;
}

/*---------------------------------------------------------*/
int& TimeStepPilot::getNRedoTimeStep()
{
  return _n_redo_time_step;
}

/*---------------------------------------------------------*/
int TimeStepPilot::getFrequency() const
{
  return _frequency;
}
