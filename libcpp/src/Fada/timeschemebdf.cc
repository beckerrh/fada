#include  "Fada/solvermanager.h"
#include  "Fada/timedata.h"
#include  "Fada/timeschemebdf.h"
#include  "Alat/vector.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeSchemeBdf::~TimeSchemeBdf(){}

TimeSchemeBdf::TimeSchemeBdf(int order, Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata) : TimeScheme(memory, newtondata), _order(order){}

TimeSchemeBdf::TimeSchemeBdf( const TimeSchemeBdf& timeschemebdf) : TimeScheme(timeschemebdf)
{
  assert(0);
}

TimeSchemeBdf& TimeSchemeBdf::operator=( const TimeSchemeBdf& timeschemebdf)
{
  TimeScheme::operator=(timeschemebdf);
  assert(0);
  return *this;
}

std::string TimeSchemeBdf::getName() const
{
  std::stringstream ss;
  ss << _order;
  return "TimeSchemeBdf"+ ss.str();
}

TimeSchemeBdf* TimeSchemeBdf::clone() const
{
  assert(0);
//return new TimeSchemeBdf(*this);
}

/*--------------------------------------------------------------------------*/
int TimeSchemeBdf::getNPreviousTimeSteps() const
{
  return _order+1;
}

/*--------------------------------------------------------------------------*/
double TimeSchemeBdf::massCoeffImplicit(const TimeData& timedata, int iteration) const
{
  // int iteration = _timesteppilot.getIterationNumber();
  if( ( iteration == 0 )  || ( _order == 1 ) )
  {
    return 1.0;
  }
  else
  {
    std::cerr << "TimeSchemeBdf::massCoeffImplicit() " << timedata.deltatold << " " << timedata.deltat << "\n";
    double dt_old = timedata.deltatold;
    double dt = timedata.deltat;
    double c1 = ( dt+dt_old )/dt;
    double c2 = c1*c1;
    return ( c2-1.0 )/( c2-c1 );
  }
}

/*--------------------------------------------------------------------------*/
void TimeSchemeBdf::computeRightHandSideTimeSchemeExplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const
{
  Alat::GhostVector& bdfexplicit = _memory[0];
  if( ( iteration == 0 )  || ( _order == 1 ) )
  {
    getSolverManager()->vectorEqual(bdfexplicit, u);
  }
  else if( iteration > 0 )
  {
    if(_order == 2)
    {
      std::cerr << "TimeSchemeBdf::computeRightHandSideTimeSchemeExplicit() " << timedata.deltatold << " " << timedata.deltat << "\n";
      double dt_old = timedata.deltatold;
      double dt = timedata.deltat;
      double c1 = ( dt+dt_old )/dt;
      double c2 = c1*c1;
      // _masscoeffimplicit = ( c2-1.0 )/( c2-c1 );
      // std::cerr << "TimeSchemeBdf::computeRightHandSideTimeSchemeExplicit() iter=" << iter << " coeffold="<< c2/( c2-c1 )<<" coeffveryold=" << -1.0/( c2-c1 ) << "\n";
      //  std::cerr << " old " <<  getSolverManager()->vectorNorm(prev[0]) << "\n";
      //  std::cerr << " very old " <<  getSolverManager()->vectorNorm(prev[1]) << "\n";
      getSolverManager()->vectorZero(bdfexplicit);
      getSolverManager()->vectorAdd(bdfexplicit, c2/( c2-c1 ), prev[0]);
      getSolverManager()->vectorAdd(bdfexplicit, -1.0/( c2-c1 ), prev[1]);
    }
    else
    {
      assert(0);
    }
  }
  // std::cerr << "TimeSchemeBdf::computeRightHandSideTimeSchemeExplicit() _masscoeffimplicit " <<_masscoeffimplicit <<"\n";
  double d = 1./timedata.deltat;
  getSolverManager()->integrateTimeRhs(status, f, u, bdfexplicit, d);
  // std::cerr << "TimeSchemeBdf::computeRightHandSideTimeSchemeExplicit() f="<<getSolverManager()->vectorNorm(f)<<"\n";
}
