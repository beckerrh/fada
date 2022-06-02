#ifndef __Fada_TimeStepPilotInformation_h
#define __Fada_TimeStepPilotInformation_h

#include  "Alat/map.h"
#include  "Alat/newtondata.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class NewtonData;  
}
namespace Fada
{   
  class TimeStepPilotInformation
  {
public:
    ~TimeStepPilotInformation();
    TimeStepPilotInformation(const Alat::NewtonData& newtondata_);
    TimeStepPilotInformation( const TimeStepPilotInformation& timesteppilotinformation);
    TimeStepPilotInformation& operator=( const TimeStepPilotInformation& timesteppilotinformation);
    std::string getName() const;
    TimeStepPilotInformation* clone() const;

    Alat::Map<std::string, const int*> _infomap;

    double estimator, trustregionparameter;
    const Alat::NewtonData& newtondata;
    bool compute_estimator, compute_trustregion;
    double _rhomatrix;
    double _rtol;
  };
}

/*--------------------------------------------------------------------------*/

#endif
