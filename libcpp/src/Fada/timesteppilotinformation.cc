#include  "Fada/timesteppilotinformation.h"
#include  "Alat/newtondata.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

TimeStepPilotInformation::~TimeStepPilotInformation()
{}

/*--------------------------------------------------------------------------*/

TimeStepPilotInformation::TimeStepPilotInformation(const Alat::NewtonData& newtondata_) : estimator(0.0), trustregionparameter(0.0), compute_estimator(false), compute_trustregion(false), newtondata(newtondata_)
{
  _infomap["linear"]=&(newtondata._n_linear_iter);
  _infomap["newton"]=&(newtondata._n_newton_iter);
  _infomap["matrix"]=&(newtondata._n_redo_matrix);
}

/*--------------------------------------------------------------------------*/

TimeStepPilotInformation::TimeStepPilotInformation( const TimeStepPilotInformation& timesteppilotinformation): newtondata(timesteppilotinformation.newtondata)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

TimeStepPilotInformation& TimeStepPilotInformation::operator=( const TimeStepPilotInformation& timesteppilotinformation)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string TimeStepPilotInformation::getName() const
{
  return "TimeStepPilotInformation";
}

/*--------------------------------------------------------------------------*/

TimeStepPilotInformation* TimeStepPilotInformation::clone() const
{
  return new TimeStepPilotInformation(*this);
}
