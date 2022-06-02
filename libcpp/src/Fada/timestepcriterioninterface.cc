#include  "Fada/timestepcriterioninterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

TimeStepCriterionInterface::~TimeStepCriterionInterface()
{}

/*--------------------------------------------------------------------------*/

TimeStepCriterionInterface::TimeStepCriterionInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

TimeStepCriterionInterface::TimeStepCriterionInterface( const TimeStepCriterionInterface& timestepcriterioninterface) : Alat::InterfaceBase(timestepcriterioninterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

TimeStepCriterionInterface& TimeStepCriterionInterface::operator=( const TimeStepCriterionInterface& timestepcriterioninterface)
{
  InterfaceBase::operator=(timestepcriterioninterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string TimeStepCriterionInterface::getInterfaceName() const
{
  return "TimeStepCriterionInterface";
}

/*--------------------------------------------------------------------------*/

TimeStepCriterionInterface* TimeStepCriterionInterface::clone() const
{
  assert(0);
  // return new TimeStepCriterionInterface(*this);
}

/*--------------------------------------------------------------------------*/

void TimeStepCriterionInterface::checkStatus(const TimeStepPilotInformation& timesteppilotinformation) const
{
  _notWritten("checkStatus(int)");
}

bool TimeStepCriterionInterface::reject(const TimeStepPilotInformation& timesteppilotinformation) const
{
  _notWritten("reject(int)");
}
