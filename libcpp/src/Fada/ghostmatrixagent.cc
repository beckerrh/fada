#include  "Fada/ghostmatrixagent.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
GhostMatrixAgent::~GhostMatrixAgent()
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
GhostMatrixAgent::GhostMatrixAgent() : Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>() {}
GhostMatrixAgent::GhostMatrixAgent( const GhostMatrixAgent& ghostvectoragent) : Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>(ghostvectoragent)
{
  assert(0);
}
GhostMatrixAgent& GhostMatrixAgent::operator=( const GhostMatrixAgent& ghostvectoragent)
{
  Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>::operator=(ghostvectoragent);
  assert(0);
  return *this;
}
std::string GhostMatrixAgent::getName() const
{
  return "GhostMatrixAgent";
}
std::ostream& GhostMatrixAgent::printLoopInformation(std::ostream& os) const
{
  os << "GhostMatrixAgent: size=" << size() << "\n";
  for(const_iterator p=begin();p!=end();p++)
  {
    os << p->first << " --> " << p->second->getName() << "\n";
  }                                                        
  return os;
}

/*--------------------------------------------------------------------------*/
bool GhostMatrixAgent::exists(const Alat::GhostMatrix& mg) const
{
  return find(mg) != end();
}

/*--------------------------------------------------------------------------*/
void GhostMatrixAgent::enrol(const Alat::GhostMatrix& mg)
{
  iterator p = find(mg);
  if( p == end() )
  {
    ( *this )[mg] = NULL;
  }
  else
  {
    std::cerr << "(register) already registered: " << mg << std::endl;
    std::cerr << "(register) registered are:\n" << *this << std::endl;
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
Alat::MatrixInterface* GhostMatrixAgent::operator()(const Alat::GhostMatrix& g) const
{
  const_iterator p = find(g);
  if( p == end() )
  {
    std::cerr << ": GhostMatrixAgent::operator(): ERROR"<<std::endl;
    std::cerr << ": GhostMatrix '"<< g <<"' not found in list of: "<<std::endl;
    std::cerr << " "<< *this << std::endl;
    assert(0);
  }
  Alat::MatrixInterface* vp = p->second;
  if(vp == NULL)
  {
    std::cerr <<  "GhostMatrixAgent  GlobalVector* NULL\t" << p->first;
    std::cerr << "\n" << *this << std::endl;
    assert(0);
  }
  return vp;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const GhostMatrixAgent& gva)
{
  int i = 0, n = gva.size();
  os << "GhostMatrixAgent: size=" << n << ", ";
  for(GhostMatrixAgent::const_iterator p = gva.begin(); p != gva.end(); p++, i++)
  {
    os << "GhostMatrix("<<i<<")=('"<< p->first << "',"<< p->second <<")";
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
