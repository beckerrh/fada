#include  "Fada/dynamicloop.h"
#include  "Alat/filescanner.h"
#include  "Alat/alatio.h"
#include  "Alat/iterationinfo.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  "Alat/nonlinearsolverinterface.h"
#include  "Fada/solvermanager.h"
#include  "Fada/timeschemebdf.h"
#include  "Fada/timeschemecranknicolson.h"
#include  "Fada/timesteppilot.h"
#include  "Alat/tokenize.h"
#include  "Alat/iterationinfo.h"
#include  <cassert>
#include  <fstream>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DynamicLoop::~DynamicLoop()
{
  if(_timescheme)
  {
    delete _timescheme;
    _timescheme = NULL;
  }
}

DynamicLoop::DynamicLoop() : StaticLoop(), _timesteppilotinformation(_newtondata), _postprocesstimeintegral("postprocesstimeintegral", "postprocess")
{}
DynamicLoop::DynamicLoop( const DynamicLoop& dynamicloop) : StaticLoop(dynamicloop), _timesteppilotinformation(dynamicloop._timesteppilotinformation)
{
  assert(0);
}

DynamicLoop& DynamicLoop::operator=( const DynamicLoop& dynamicloop)
{
  StaticLoop::operator=(dynamicloop);
  assert(0);
  return *this;
}

std::string DynamicLoop::getName() const
{
  return "DynamicLoop";
}

FadaEnums::looptype DynamicLoop::getType() const
{
  return FadaEnums::DynamicLoop;
}

DynamicLoop* DynamicLoop::clone() const
{
  return new DynamicLoop(*this);
}

/*--------------------------------------------------------------------------*/
TimeData& DynamicLoop::getTimeData()
{
  return _timedata;
}

/*---------------------------------------------------------*/
void DynamicLoop::basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  getChronometer().start("basicInit");
  std::string time_discretization, timesteppilot;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("time_discretization", &time_discretization, "bdf_2");
  dataformathandler.insert("timesteppilot", &timesteppilot, "estimator");
  dataformathandler.insert("outputincrement", &_outputincrement, 10);
  dataformathandler.insert("outputdeltat", &_outputdeltat, 10);
  dataformathandler.insert("firstnumber", &_firstnumber, 0);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Loop", 0);

  Alat::StringVector bouts = Alat::Tokenize(time_discretization, "_");
  if(bouts[0] == "bdf")
  {
    assert(bouts.size() == 2);
    int order = atoi( bouts[1].c_str() );
    _timescheme = new TimeSchemeBdf(order, _timestepmem, _newtondata);
  }
  else if(bouts[0] == "cn")
  {
    assert(bouts.size() == 2);
    int order = atoi( bouts[1].c_str() );
    _timescheme = new TimeSchemeCrankNicolson(order, _timestepmem, _newtondata);
  }
  else
  {
    std::cerr<<"****Error DynamicLoop::basicInit: unknown TimeDiscretization: "<<time_discretization<<'\n';
    assert(0);
    exit(1);
  }

  _previoustimesteps.set_size( _timescheme->getNPreviousTimeSteps() );
  for(int i = 0; i < _previoustimesteps.size(); i++)
  {
    std::stringstream ss;
    ss<<"timesteps_"<<i;
    _previoustimesteps[i].setName( ss.str() );
    _previoustimesteps[i].setType("unknowns");
    solvermanager->registerVector(_previoustimesteps[i]);
  }
  _timestepmem.set_size( _timescheme->getNTimeStepMemory() );
  for(int i = 0; i < _timestepmem.size(); i++)
  {
    std::stringstream ss;
    ss<<"timestepmem_"<<i;
    _timestepmem[i].setName( ss.str() );
    _timestepmem[i].setType("unknowns");
    solvermanager->registerVector(_timestepmem[i]);
  }

  getChronometer().stop("basicInit");
  solvermanager->registerVector(_postprocesstimeintegral);
  StaticLoop::basicInit(modelmanager, solvermanager, rundirectory, parameterfile);
  getChronometer().start("basicInit");
  _timescheme->basicInit(this);
  _timesteppilot._criterianame = timesteppilot;
  _timesteppilot.setTimeStepPilotInformation(_timesteppilotinformation);
  _timesteppilotinformation._rhomatrix = getNonLinearSolver()->getRhoMatrix();
  _timesteppilotinformation._rtol = getNonLinearSolver()->getIterationInfo().getRTol();
  _timesteppilot.basicInit( getParameterFile(), getSolverManager(), getTimeData(), getNonLinearSolver()->getIterationInfo() );

  getSolverManager()->setTimeScheme(time_discretization);
  getChronometer().stop("basicInit");
}

/*---------------------------------------------------------*/
void DynamicLoop::_outputSolution(int iteration, bool stop)
{
  assert(_output_times.size() > 0);
  bool output;
  if(_outputincrement > 0)
  {
    output = ( ( iteration%_outputincrement ) == 0 ) | stop;
  }
  else
  {
    output = ( _timedata.time >= _nextoutputtime -1e-10*_outputdeltat);
    if(output)
    {
      _nextoutputtime = _timedata.time + _outputdeltat;
    }
  }

  if( output)
  {
    assert(_timedata.time > _output_times[_output_times.size()-1]);
    int istep = _output_times.size()+_firstnumber;
    _output_times.push_back(_timedata.time);
    getSolverManager()->writeUnknownVariables(_f, _u, istep);

    std::string filenametimeinfo = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "DynamicLoopInfo");
    std::ofstream filetimeinfo( filenametimeinfo.c_str() );
    copy( _output_times.begin(), _output_times.end(), std::ostream_iterator<double>(filetimeinfo, " ") );
    
    // _output_times.write(filetimeinfo, "ascii");
    filetimeinfo.close();
        
    getSolverManager()->writePostProcessVariablesDynamic(_postprocesstimeintegral, istep);    
  }
}

/*---------------------------------------------------------*/
void DynamicLoop::run()
{
  initializeSolution(_u, _timedata );

  _output_times.push_back( _timedata.time );
  getSolverManager()->writeUnknownVariables(_f, _u, _firstnumber);
  getMeshComposition()->writeH5( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "Mesh") );
  getMeshComposition()->writeMeshInfo( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "MeshInfo") );
  getSolverManager()->writeVariablesInfo( );
  _nextoutputtime = _timedata.time + _outputdeltat;

  bool stop = false;
  int nlitertotal = 0, linitertotal = 0;
  int iteration = 0;
  getSolverManager()->vectorEqual(_previoustimesteps[0], _u);
  getSolverManager()->setTimeInfo(_timedata, _timescheme->massCoeffImplicit(_timedata, iteration)/_timedata.deltat);

  AlatEnums::residualstatus status;
  // getSolverManager()->computeLinearizationPoint(_u);
  getSolverManager()->matrixZero(_A);
  getSolverManager()->constructJacobianMatrix(status, _A, _u);
  getSolverManager()->constructLinearSolver(_linearsolverallvars, _u);

  getSolverManager()->writePostProcessVariablesDynamic(_postprocesstimeintegral, 0);
  while(!stop)
  {
    // std::cerr << "1 _timedata.time="<<_timedata.time<<"\n";
    getSolverManager()->vectorZero(_f);
    _timescheme->computeRightHandSideTimeSchemeExplicit(status, _f, _u, _previoustimesteps, _timedata, iteration);
    _timedata.time += _timedata.deltat;
    getSolverManager()->setTimeInfo(_timedata, _timescheme->massCoeffImplicit(_timedata, iteration)/_timedata.deltat);
    iteration++;
    // std::cerr << "2 _timedata.time="<<_timedata.time<<"\n";
    _timescheme->computeRightHandSideTimeSchemeImplicit(status, _f, _u, _previoustimesteps, _timedata, iteration);
    _timescheme->printTimeStepInfo(_timedata, iteration);
    getSolverManager()->beforeTimestep(_u);
    std::cerr << "DynamicLoop _newtondata="<<_newtondata<<"\n";
    nonlinearSolve(_newtondata, _u, _f);
    // std::cerr << "_previoustimesteps[0] = " << getSolverManager()->vectorNorm(_previoustimesteps[0])<<"\n";
    // std::cerr << "u = " << getSolverManager()->vectorNorm(_u)<<"\n";

    bool stepok = true;
    if(_newtondata.getNewtonStatus() != AlatEnums::NewtonStatusConverged)
    {
      if(_timesteppilot._criterianame == "constant")
      {
        _error_string("doTimeStep", "Newton not converged and constant time stepping strategy.");
      }
      std::cerr<<"****WARNING:   DynamicLoop::doTimeStep(): scheme status_newton: "<<AlatEnums::newtonStatusToString( _newtondata.getNewtonStatus() )<<'\n';
      stepok = false;
    }
    else
    {
      stop = _timescheme->estimate(iteration, _f, _u, _previoustimesteps, _timedata, _timesteppilot, _timesteppilotinformation);
      if( _timesteppilot.redoTimeStep() )
      {
        std::cerr << "DynamicLoop::Run() REDO TIMESTEP timedata.time=" <<_timedata.time<< "iteration="<<iteration <<"\n";
        stepok = false;
      }
    }
    if(not stepok)
    {
      _timesteppilot.getNRedoTimeStep()++;
      if(_timesteppilot.getNRedoTimeStep() == 20)
      {
        _error_string("doTimeStep", "max redo attained");
      }
      iteration--;
      _timedata.time -= _timedata.deltat;
      getSolverManager()->vectorEqual(_u, _previoustimesteps[0]);
      AlatEnums::residualstatus status;
      // std::cerr << "coucou1 " << _timedata.deltatold << " " << _timedata.deltat << "\n";
      getSolverManager()->setTimeInfo(_timedata, _timescheme->massCoeffImplicit(_timedata, iteration)/_timedata.deltat);
      // std::cerr << "coucou2 " << _timedata.deltatold << " " << _timedata.deltat << "\n";
      getSolverManager()->restartLinearSolver(status, _A, _linearsolverallvars, _u);
      _timesteppilot.setNewtonProblem(_timedata);
      // std::cerr << "coucou3 " << _timedata.deltatold << " " << _timedata.deltat << "\n";
    }
    else
    {
      //postprocess
      getSolverManager()->postProcess(status, _postprocess, _u, _previoustimesteps[0]);
      getSolverManager()->integrateInTimePostProcess(_postprocesstimeintegral, _postprocess);
      _outputSolution(iteration, stop);
      _timesteppilot.getNRedoTimeStep() = 0;
      nlitertotal += _newtondata.getNLinearIteration();
      linitertotal += _newtondata.getNNewtonIteration();
      for(int i = _previoustimesteps.size()-1; i > 0; i--)
      {
        getSolverManager()->vectorEqual(_previoustimesteps[i], _previoustimesteps[i-1]);
      }
      getSolverManager()->vectorEqual(_previoustimesteps[0], _u);

      if(  iteration%_timesteppilot.getFrequency() == 0 )
      {
        _timesteppilot.computeNewDeltaT(_timedata, &_timesteppilotinformation, iteration);
      }
      if( _timedata.deltat < _timesteppilot._dtmin) 
      {
        _timedata.deltat = _timesteppilot._dtmin;
      }
      else if( _timedata.deltat > _timesteppilot._dtmax )
      {
        _timedata.deltat = _timesteppilot._dtmax;
      }
      if(_timesteppilot.exactfinaltime)
      {
        if( _timesteppilot.getEndTime()-_timedata.time-_timedata.deltat < _timesteppilot._dtmin )
        {
          _timedata.deltat = _timesteppilot.getEndTime()-_timedata.time;
        }
        else if( _timedata.time+_timedata.deltat > _timesteppilot.getEndTime() )
        {
          _timedata.deltat = _timesteppilot.getEndTime()-_timedata.time;
        }        
      }

      std::string filename = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "IterationInfo");
      std::ofstream file( filename.c_str() );
      if( !file.is_open() )
      {
        std::cerr << "*** ERROR in DynamicLoop::run() cannot open file "<<filename<<"\n";
        assert(0);
        exit(1);
      }
      file<<nlitertotal<<" "<<linitertotal<<"\n";
      file.close();

      std::string filenametimeinfo = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "DynamicLoopInfo");
      std::ofstream filetimeinfo( filenametimeinfo.c_str() );
      // filetimeinfo << _output_times.size()-1+_firstnumber << " " << _timescheme->getTimeStepPilot()->getTime() << "\n";
      // _output_times.write(filetimeinfo, "ascii");
      Alat::saveAlat(filetimeinfo, _output_times, "ascii");
      // copy( _output_times.begin(), _output_times.end(), std::ostream_iterator<double>(filetimeinfo, " ") );
      filetimeinfo.close();
    }
  }
}
