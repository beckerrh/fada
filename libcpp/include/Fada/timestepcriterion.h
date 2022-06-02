#ifndef __Fada_TimeStepCriterion_h
#define __Fada_TimeStepCriterion_h

#include  "Alat/fixarray.h"
#include  "Alat/map.h"
#include  "Alat/stringset.h"
#include  "timestepcriterioninterface.h"
#include  <iostream>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ParameterFile;
  class TimeStepCriterion : public TimeStepCriterionInterface
  {
public:
    std::string _name;
    mutable double _value;
    double _minvalue, _maxvalue;
    mutable Alat::StringSet _decreasenames, _increasenames;

private:
    double _rejectvalue;
    mutable bool _increase, _decrease;
    int _ignore_first;

    typedef Alat::Map<std::string, Alat::FixArray<3, int> > CriteriaMap;
    CriteriaMap _criteriamap;



public:
    ~TimeStepCriterion( );
    TimeStepCriterion(double minvalue, double maxvalue, double rejectvalue, int ignore_first, const std::string& name);
    TimeStepCriterion( const TimeStepCriterion& timestepcriterion );
    TimeStepCriterion& operator=( const TimeStepCriterion& timestepcriterion);
    std::string getName() const;
    TimeStepCriterion* clone() const;
    void addCriterion(const std::string& name, int minvalue, int maxvalue, int rejectvalue=1000000);
    bool reject(const TimeStepPilotInformation& timesteppilotinformation) const;
    void checkStatus(const TimeStepPilotInformation& timesteppilotinformation) const;
    bool decrease() const;
    bool increase() const;
    bool ignore(int iteration) const;
  };
  std::ostream& operator<<(std::ostream& os, const TimeStepCriterion& criterion);
}

/*--------------------------------------------------------------------------*/

#endif
