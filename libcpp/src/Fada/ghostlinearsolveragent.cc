#include  "Fada/ghostlinearsolveragent.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
GhostLinearSolverAgent::~GhostLinearSolverAgent()
{
  for(iterator p = begin(); p != end(); p++)
  {
    if(p->second)
    {
      delete p->second;
      p->second = NULL;
    }
  }
}

GhostLinearSolverAgent::GhostLinearSolverAgent() : Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>()
{}
GhostLinearSolverAgent::GhostLinearSolverAgent( const GhostLinearSolverAgent& ghostvectoragent) : Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>(ghostvectoragent)
{
  assert(0);
}

GhostLinearSolverAgent& GhostLinearSolverAgent::operator=( const GhostLinearSolverAgent& ghostvectoragent)
{
  Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>::operator=(ghostvectoragent);
  assert(0);
  return *this;
}

std::string GhostLinearSolverAgent::getName() const
{
  return "GhostLinearSolverAgent";
}

std::ostream& GhostLinearSolverAgent::printLoopInformation(std::ostream& os) const
{
  os << "GhostLinearSolverAgent: size=" << size() << "\n";
  for(const_iterator p = begin(); p != end(); p++)
  {
    assert(p->second);
    os << p->first << " --> ";
    p->second->printLoopInformation(os);
    os << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
bool GhostLinearSolverAgent::exists(const Alat::GhostLinearSolver& mg) const
{
  return find(mg) != end();
}

/*--------------------------------------------------------------------------*/
void GhostLinearSolverAgent::enrol(const Alat::GhostLinearSolver& mg)
{
  iterator p = find(mg);
  if( p == end() )
  {
    ( *this )[mg] = NULL;
    // insert(std::make_pair(mg,static_cast<Alat::LinearSolverInterface*>(NULL)));
  }
  else
  {
    std::cerr << "(register) already registered: " << mg << std::endl;
    std::cerr << "(register) registered are:\n" << *this << std::endl;
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
Alat::LinearSolverInterface* GhostLinearSolverAgent::operator()(const Alat::GhostLinearSolver& g) const
{
  const_iterator p = find(g);
  if( p == end() )
  {
    std::cerr << ": GhostLinearSolverAgent::operator(): ERROR"<<std::endl;
    std::cerr << ": Alat::GhostLinearSolver '"<< g <<"' not found in list of: "<<std::endl;
    std::cerr << " "<< *this << std::endl;
    assert(0);
  }
  Alat::LinearSolverInterface* vp = p->second;
  // if(vp == NULL)
  // {
  //   // std::cerr <<  "GhostLinearSolverAgent  GlobalVector* NULL\t" << p->first;
  //   // std::cerr << "\n" << *this << std::endl;
  //   // assert(0);
  // }
  return vp;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const GhostLinearSolverAgent& gva)
{
  int i = 0, n = gva.size();
  os << "GhostLinearSolverAgent: size=" << n << ", ";
  for(GhostLinearSolverAgent::const_iterator p = gva.begin(); p != gva.end(); p++, i++)
  {
    os << "GhostLinearSolver("<<i<<")=('"<< p->first << "',"<< p->second <<")";
    if( i < n-1 )
    {
      os << ", ";
    }
    else
    {
      os << ". ";
    }
  }
  return os;
}
