#ifndef __Fada_TimeScheme_h
#define __Fada_TimeScheme_h

#include  "Alat/ghostvector.h"
#include  "timeschemeinterface.h"
#include  "timesteppilot.h"
#include  "timesteppilotinformation.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{ 
  class NewtonData;
  class SolverManager;

  class TimeScheme : public TimeSchemeInterface
  {
protected:
    DynamicLoop* _loop;
    Alat::Vector<Alat::GhostVector>& _memory;

public:
    ~TimeScheme();
    TimeScheme(Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata);
    TimeScheme( const TimeScheme& timescheme);
    TimeScheme& operator=( const TimeScheme& timescheme);
    std::string getName() const;
    TimeScheme* clone() const;

    void basicInit(DynamicLoop* loop);
    const SolverManager* getSolverManager() const;
    SolverManager* getSolverManager();
    int getNTimeStepMemory() const;
    void printTimeStepInfo(const TimeData& timedata, int iteration) const;
    void computeRightHandSideTimeSchemeImplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const;
    // bool doTimeStep(int iteration, NewtonData& newtondata, Alat::GhostVector& f, Alat::GhostVector& u, Alat::Vector<Alat::GhostVector>& prev, TimeData& timedata, TimeStepPilot& timesteppilot);
    bool estimate(int iteration, Alat::GhostVector& f, Alat::GhostVector& u, Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, TimeStepPilot& timesteppilot, TimeStepPilotInformation& timesteppilotinformation);
  };
}

/*--------------------------------------------------------------------------*/

#endif
