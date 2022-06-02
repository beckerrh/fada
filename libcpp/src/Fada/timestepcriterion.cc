#include  "Fada/timestepcriterion.h"
#include  "Fada/timesteppilotinformation.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeStepCriterion::~TimeStepCriterion(){}
TimeStepCriterion::TimeStepCriterion(double minvalue, double maxvalue, double rejectvalue, int ignore_first, const std::string& name) : TimeStepCriterionInterface(), _rejectvalue(rejectvalue), _minvalue(minvalue), _maxvalue(maxvalue), _ignore_first(ignore_first), _name(name)
{
  // std::cerr << "TimeStepCriterion::TimeStepCriterion(): "<<_name<<'\n';
  // std::cerr << "rejectvalue maxvalue minvalue" << _rejectvalue << " " << _maxvalue << " " << _minvalue<<"\n";
  assert(_rejectvalue >= _maxvalue);
  if(_maxvalue < _minvalue)
  {
    std::cerr << "*** ERROR in TimeStepCriterion<T> _minvalue="<<_minvalue<<">"<<_maxvalue<<" _maxvalue\n";
    assert(0);
  }
}

TimeStepCriterion::TimeStepCriterion( const TimeStepCriterion& timestepcriterion)
{
  *this = timestepcriterion;
}

TimeStepCriterion& TimeStepCriterion::operator=( const TimeStepCriterion& timestepcriterion)
{
  assert(0);
  _minvalue = timestepcriterion._minvalue;
  _maxvalue = timestepcriterion._maxvalue;
  _rejectvalue = timestepcriterion._rejectvalue;
  _value = timestepcriterion._value;
  _increase = timestepcriterion._increase;
  _decrease = timestepcriterion._decrease;
  _criteriamap = timestepcriterion._criteriamap;
  _name = timestepcriterion._name;
  return *this;
}

TimeStepCriterion* TimeStepCriterion::clone() const
{
  assert(0);
}

std::string TimeStepCriterion::getName() const
{
  return "TimeStepCriterion";
}

/*--------------------------------------------------------------------------*/
void TimeStepCriterion::addCriterion(const std::string& name, int minvalue, int maxvalue, int rejectvalue)
{
  _criteriamap[name][0] = minvalue;
  _criteriamap[name][1] = maxvalue;
  _criteriamap[name][2] = rejectvalue;
}

/*--------------------------------------------------------------------------*/
bool TimeStepCriterion::ignore(int iteration) const
{
  return iteration < _ignore_first;
}

/*--------------------------------------------------------------------------*/
bool TimeStepCriterion::reject(const TimeStepPilotInformation& timesteppilotinformation) const
{
  double value;
  if(_name == "constant")
  {
    return false;
  }

  if(_name == "trustregion")
  {
    value = timesteppilotinformation.trustregionparameter;
  }
  else if(_name == "estimator")
  {
    value = timesteppilotinformation.estimator;
  }
  if(value >= _rejectvalue)
  {
    std::cerr << "TimeStepCriterion::reject() " << _name << " : " << _rejectvalue << " <=? " << value << "\n";
    return true;
  }
  for(CriteriaMap::const_iterator p = _criteriamap.begin(); p != _criteriamap.end(); p++)
  {
    const std::string& name = ( *p ).first;
    int value = *( timesteppilotinformation._infomap[name] );
    int rejectvalue = ( *p ).second[2];
    if(value >= rejectvalue)
    {
      std::cerr << "TimeStepCriterion::reject() " << name << " : " << rejectvalue << " <=? " << value << "\n";
      return true;
    }
  }
  return false;
}

/*--------------------------------------------------------------------------*/

void TimeStepCriterion::checkStatus(const TimeStepPilotInformation& timesteppilotinformation) const
{
  if(_name == "constant")
  {
    _increase = false;
    _decrease = false;
    return;
  }
  if(_name == "trustregion")
  {
    _value = timesteppilotinformation.trustregionparameter;
  }
  else if(_name == "estimator")
  {
    _value = timesteppilotinformation.estimator;
  }

  _decreasenames.clear();
  _increasenames.clear();
  _increase = ( _value < _minvalue );
  _decrease = ( _value > _maxvalue );
  if(_value < _minvalue)
  {
    _increasenames.insert(_name);
  }
  if(_value > _maxvalue)
  {
    _decreasenames.insert(_name);
  }
  std::cerr << "checkStatus: "<<_name<< " ["<<_minvalue<<" ; "<<_maxvalue<<" ] " << _value<<"\n";
  for(CriteriaMap::const_iterator p = _criteriamap.begin(); p != _criteriamap.end(); p++)
  {
    const std::string& name = ( *p ).first;
    int value = *( timesteppilotinformation._infomap[name] );
    int minvalue = ( *p ).second[0];
    int maxvalue = ( *p ).second[1];
    std::cerr << "checkStatus: "<<name<< " ["<<minvalue<<" ; "<<maxvalue<<" ] " << value<<"\n";
    // std::cerr << "vant _increase="<<_increase<<" _decrease="<<_decrease<<"\n";
    _increase = _increase&( value < minvalue );
    _decrease = _decrease|( value > maxvalue );
    if(value < minvalue)
    {
      _increasenames.insert(name);
    }
    if(value > maxvalue)
    {
      _decreasenames.insert(name);
    }
  }
  // std::cerr << "checkStatus: _increasenames: "<<_increasenames<<"\n";
  // std::cerr << "checkStatus: _decreasenames: "<<_decreasenames<<"\n";
  // std::cerr << "checkStatus: _increase: "<<_increase<<"\n";
  // std::cerr << "checkStatus: _decrease: "<<_decrease<<"\n";
}

/*--------------------------------------------------------------------------*/

std::ostream& Fada::operator<<(std::ostream& os, const TimeStepCriterion& criterion)
{
  os <<criterion._name<<"[" << criterion._minvalue << "," << criterion._maxvalue << "] -> " << criterion._value;
  return os;
}

/*--------------------------------------------------------------------------*/

bool TimeStepCriterion::decrease() const
{
  return _decrease;
}

/*--------------------------------------------------------------------------*/

bool TimeStepCriterion::increase() const
{
  return _increase;
}

/*--------------------------------------------------------------------------*/
