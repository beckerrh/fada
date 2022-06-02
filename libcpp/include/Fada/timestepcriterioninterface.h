#ifndef __Fada_TimeStepCriterionInterface_h
#define __Fada_TimeStepCriterionInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TimeStepPilotInformation;

  class TimeStepCriterionInterface : public Alat::InterfaceBase
  {
private:
protected:
public:
    ~TimeStepCriterionInterface();
    TimeStepCriterionInterface();
    TimeStepCriterionInterface( const TimeStepCriterionInterface& timestepcriterioninterface);
    TimeStepCriterionInterface& operator=( const TimeStepCriterionInterface& timestepcriterioninterface);
    std::string getInterfaceName() const;
    TimeStepCriterionInterface* clone() const;
    virtual void checkStatus(const TimeStepPilotInformation& timesteppilotinformation) const;
    virtual bool reject(const TimeStepPilotInformation& timesteppilotinformation) const;
    virtual bool decrease() const = 0;
    virtual bool increase() const = 0;
    virtual bool ignore(int iteration) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
