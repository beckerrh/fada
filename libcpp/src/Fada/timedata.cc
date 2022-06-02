#include  "Fada/timedata.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

TimeData::~TimeData()
{}

/*--------------------------------------------------------------------------*/

TimeData::TimeData() : time(0.0), deltat(0.0), deltatold(0.0)
{}

/*--------------------------------------------------------------------------*/

TimeData::TimeData( const TimeData& timedata)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

TimeData& TimeData::operator=( const TimeData& timedata)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string TimeData::getName() const
{
  return "TimeData";
}

/*--------------------------------------------------------------------------*/

TimeData* TimeData::clone() const
{
  return new TimeData(*this);
}
