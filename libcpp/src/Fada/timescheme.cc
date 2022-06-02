#include  "Fada/dynamicloop.h"
#include  "Alat/iterationinfo.h"
#include  "Alat/nonlinearsolverinterface.h"
#include  "Fada/solvermanagerinterface.h"
#include  "Fada/timescheme.h"
#include  "Fada/timesteppilot.h"
#include  <cassert>
#include  <cmath>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TimeScheme::~TimeScheme()
{}

TimeScheme::TimeScheme(Alat::Vector<Alat::GhostVector>& memory, const Alat::NewtonData& newtondata) : TimeSchemeInterface(), _memory(memory)
{}
TimeScheme::TimeScheme( const TimeScheme& timescheme) : TimeSchemeInterface(timescheme), _memory(timescheme._memory)
{
  assert(0);
}

TimeScheme& TimeScheme::operator=( const TimeScheme& timescheme)
{
  TimeSchemeInterface::operator=(timescheme);
  assert(0);
  return *this;
}

std::string TimeScheme::getName() const
{
  return "TimeScheme";
}

TimeScheme* TimeScheme::clone() const
{
  assert(0);
  // return new TimeScheme(*this);
}

/*--------------------------------------------------------------------------*/
void TimeScheme::basicInit(DynamicLoop* loop)
{
  _loop = loop;
}

/*--------------------------------------------------------------------------*/
const SolverManagerInterface* TimeScheme::getSolverManager() const
{
  return _loop->getSolverManager();
}

SolverManagerInterface* TimeScheme::getSolverManager()
{
  return _loop->getSolverManager();
}

/*--------------------------------------------------------------------------*/
void TimeScheme::printTimeStepInfo(const TimeData& timedata, int iteration) const
{
  std::cout << "\n============= " << iteration << " (" << getName() << ") ";
  std::cout << " [t,dt] "<< timedata.time << " " << timedata.deltat << "\t";
  std::cout << std::endl;
}

/*--------------------------------------------------------------------------*/
int TimeScheme::getNTimeStepMemory() const
{
  return 4;
}

/*--------------------------------------------------------------------------*/
bool TimeScheme::estimate(int iteration, Alat::GhostVector& f, Alat::GhostVector& u, Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, TimeStepPilot& timesteppilot, TimeStepPilotInformation& timesteppilotinformation)
{
  if( ( timesteppilotinformation.compute_estimator )&&( iteration%timesteppilot.getFrequency() == 0 ) )
  //calcul de ||uold-u|| et de deltat||u_t||.....
  {
    Alat::GhostVector& h1  = _memory[2];
    Alat::GhostVector& h2  = _memory[3];
    // std::cerr << "TimeScheme::estimate() u="<<getSolverManager()->vectorNorm(u) <<"\n";
    // std::cerr << "TimeScheme::estimate() prev[0]="<<getSolverManager()->vectorNorm(prev[0]) <<"\n";
    timesteppilotinformation.estimator=getSolverManager()->computeTimeEstimator(u, prev[0], h1, h2);
    // std::cerr << "TimeScheme::estimate() u="<<getSolverManager()->vectorNorm(u) <<"\n";
    // std::cerr << "TimeScheme::estimate() prev[0]="<<getSolverManager()->vectorNorm(prev[0]) <<"\n";
    std::cerr << "timesteppilotinformation.estimator="<<timesteppilotinformation.estimator<<"\n";
  }
  if(timesteppilotinformation.compute_trustregion)
  {
    AlatEnums::residualstatus status;
    assert(0);
    getSolverManager()->vectorZero( f );
    getSolverManager()->constructRightHandSide(status, f);
    Alat::GhostVector& h1  = _memory[2];
    Alat::GhostVector& h2  = _memory[3];
    getSolverManager()->vectorZero( h1 );
    getSolverManager()->vectorAdd( h1, 1.0, u );
    getSolverManager()->vectorAdd( h1, -1.0, prev[0] );
    getSolverManager()->vectorZero( h2 );
    getSolverManager()->vectorEqual(h2, f);
    getSolverManager()->constructForm(status, h2, prev[0]);
    getSolverManager()->computeLinearization(status, h2, prev[0], h1);
    double respred = getSolverManager()->vectorNorm(h2);
    getSolverManager()->vectorZero( h2 );
    getSolverManager()->vectorEqual(h2, f);
    getSolverManager()->constructForm(status, h2, u);
    double res = getSolverManager()->vectorNorm(h2);
    std::cerr << "res respred " << res << " " << respred << " " << res/respred <<"\n";
    timesteppilotinformation.trustregionparameter = fabs(res/respred-1.0);
    // assert(0);
  }

  timesteppilot.checkTimeStep( timedata, &timesteppilotinformation, iteration );
  if( timesteppilot.stopDynamicLoop() )
  {
    return true;
  }
  return false;
}

/*--------------------------------------------------------------------------*/

void TimeScheme::computeRightHandSideTimeSchemeImplicit(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::Vector<Alat::GhostVector>& prev, const TimeData& timedata, int iteration) const
{
  getSolverManager()->constructRightHandSide(status, f);
}
