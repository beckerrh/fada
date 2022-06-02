#include  "Fada/solvermanagerinterface.h"
#include  "Fada/timedata.h"
#include  "Fada/timeschemecranknicolson.h"
#include  "Alat/vector.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeSchemeCrankNicolson::~TimeSchemeCrankNicolson(){}

TimeSchemeCrankNicolson::TimeSchemeCrankNicolson(int order, Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata) : TimeScheme(memory, newtondata), _order(order){}

TimeSchemeCrankNicolson::TimeSchemeCrankNicolson( const TimeSchemeCrankNicolson& timeschemebdf) : TimeScheme(timeschemebdf)
{
  assert(0);
}

TimeSchemeCrankNicolson& TimeSchemeCrankNicolson::operator=( const TimeSchemeCrankNicolson& timeschemebdf)
{
  TimeScheme::operator=(timeschemebdf);
  assert(0);
  return *this;
}

std::string TimeSchemeCrankNicolson::getName() const
{
  std::stringstream ss;
  ss << _order;
  return "TimeSchemeCrankNicolson"+ ss.str();
}

TimeSchemeCrankNicolson* TimeSchemeCrankNicolson::clone() const
{
  assert(0);
//return new TimeSchemeCrankNicolson(*this);
}

/*--------------------------------------------------------------------------*/
int TimeSchemeCrankNicolson::getNPreviousTimeSteps() const
{
  return _order+1;
}

//
//   2/k M u^{n+1} + f(u^{n+1}) =  2 f +  2/k Mu^{n} - f(u^{n}
//
//

/*--------------------------------------------------------------------------*/
double TimeSchemeCrankNicolson::massCoeffImplicit(const TimeData& timedata, int iteration) const
{
  return 2.0;
}

/*--------------------------------------------------------------------------*/
void TimeSchemeCrankNicolson::computeRightHandSideTimeSchemeExplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const
{
  assert( _order == 1 );
  
  double d = 2./timedata.deltat;
  getSolverManager()->integrateTimeRhs(status, f, u, u, d);
  getSolverManager()->constructRightHandSide(status, f);
}
