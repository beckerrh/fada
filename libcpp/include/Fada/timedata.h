#ifndef __Fada_TimeData_h
#define __Fada_TimeData_h

#include  <string>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class TimeData
  {
public:
    ~TimeData();
    TimeData();
    TimeData( const TimeData& timedata);
    TimeData& operator=( const TimeData& timedata);
    std::string getName() const;
    TimeData* clone() const;

    double time, deltat, deltatold;
  };
}

/*--------------------------------------------------------------------------*/

#endif
