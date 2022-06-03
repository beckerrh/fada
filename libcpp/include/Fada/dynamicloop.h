#ifndef __Fada_DynamicLoop_h
#define __Fada_DynamicLoop_h

#include  "Alat/doublevector.h"
#include  "Alat/intvector.h"
#include  "Alat/vector.h"
#include  "fadalightenums.h"
#include  "staticloop.h"
#include  "timedata.h"
#include  "timesteppilot.h"
#include  "timesteppilotinformation.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TimeSchemeInterface;

  class DynamicLoop : public StaticLoop
  {
protected:
    Alat::GhostVector _postprocesstimeintegral;
    double _outputdeltat, _nextoutputtime;
    int _outputincrement, _firstnumber;
    TimeSchemeInterface* _timescheme;
    Alat::Vector<Alat::GhostVector> _timestepmem;
    Alat::Vector<Alat::GhostVector> _previoustimesteps;
    // Alat::DoubleVector _output_times;
    std::vector<double> _output_times;
    Alat::IntVector _output_times_indices;
    TimeData _timedata;
    void _outputSolution(int iteration, bool stop);
    TimeStepPilot _timesteppilot;
    TimeStepPilotInformation _timesteppilotinformation;

public:
    ~DynamicLoop();
    DynamicLoop();
    DynamicLoop( const DynamicLoop& dynamicloop);
    DynamicLoop& operator=( const DynamicLoop& dynamicloop);
    std::string getName() const;
    DynamicLoop* clone() const;
    FadaEnums::looptype getType() const;

    void basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();
    TimeData& getTimeData();
  };
}

/*--------------------------------------------------------------------------*/

#endif
