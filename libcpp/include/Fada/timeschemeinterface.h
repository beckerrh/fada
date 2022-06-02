#ifndef __Fada_TimeSchemeInterface_h
#define __Fada_TimeSchemeInterface_h

#include  "Alat/interfacebase.h"
#include  "Alat/enums.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class GhostVector;
  template<class T>
  class Vector;
}
namespace Fada
{
  class DynamicLoop;
  class NewtonData;
  class TimeData;
  class TimeStepPilot;
  class TimeStepPilotInformation;

  class TimeSchemeInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~TimeSchemeInterface();
    TimeSchemeInterface();
    TimeSchemeInterface( const TimeSchemeInterface& timeschemeinterface);
    TimeSchemeInterface& operator=( const TimeSchemeInterface& timeschemeinterface);
    std::string getName() const;

    virtual void basicInit(DynamicLoop* loop) = 0;
    virtual int getNTimeStepMemory() const = 0;
    virtual int getNPreviousTimeSteps() const = 0;
    virtual void printTimeStepInfo(const TimeData& timedata, int iteration) const=0;
    virtual double massCoeffImplicit(const TimeData& timedata, int iteration) const = 0;
    virtual void computeRightHandSideTimeSchemeExplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const =0;
    virtual void computeRightHandSideTimeSchemeImplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const =0;
    virtual bool estimate(int iteration, Alat::GhostVector& f, Alat::GhostVector& u, Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, TimeStepPilot& timesteppilot, TimeStepPilotInformation& timesteppilotinformation)=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
