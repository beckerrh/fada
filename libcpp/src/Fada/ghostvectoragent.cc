#include  "Fada/ghostvectoragent.h"
#include  "Alat/tokenize.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
GhostVectorAgent::~GhostVectorAgent()
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
GhostVectorAgent::GhostVectorAgent() : Alat::Map<Alat::GhostVector, Alat::VectorInterface*>() {}
GhostVectorAgent::GhostVectorAgent( const GhostVectorAgent& ghostvectoragent) : Alat::Map<Alat::GhostVector, Alat::VectorInterface*>(ghostvectoragent)
{
  assert(0);
}
GhostVectorAgent& GhostVectorAgent::operator=( const GhostVectorAgent& ghostvectoragent)
{
  Alat::Map<Alat::GhostVector, Alat::VectorInterface*>::operator=(ghostvectoragent);
  assert(0);
  return *this;
}
std::string GhostVectorAgent::getName() const
{
  return "GhostVectorAgent";
}
std::ostream& GhostVectorAgent::printLoopInformation(std::ostream& os) const
{
  os << "GhostVectorAgent: size=" << size() << "\n";
  for(const_iterator p=begin();p!=end();p++)
  {
    os << p->first << " --> " << p->second->getName() << "\n";
  }                                                        
  return os;
}

/*--------------------------------------------------------------------------*/
Alat::StringIntMap GhostVectorAgent::statistics() const
{
  Alat::StringIntMap numberoftypes;
  for(const_iterator p = begin(); p != end(); p++)
  {
    std::string name = p->first.getName();
    Alat::StringVector bouts = Alat::Tokenize(name, "_");
    name = "";
    int last = 1;
    if(bouts.size()>2) last = bouts.size()-2;
    for(int i=0;i<last;i++)
    {
      name += bouts[i]+"_";
    }
    std::string typeandname = p->first.getType() + "_" + bouts[0];
    if( numberoftypes.find(typeandname) == numberoftypes.end() )
    {
      numberoftypes[typeandname] = 1;
    }
    else
    {
      numberoftypes[typeandname]++;
    }
  }
  return numberoftypes;
}

/*--------------------------------------------------------------------------*/
bool GhostVectorAgent::exists(const Alat::GhostVector& mg) const
{
  return find(mg) != end();
}

/*--------------------------------------------------------------------------*/
void GhostVectorAgent::enrol(const Alat::GhostVector& mg)
{
  iterator p = find(mg);
  if( p == end() )
  {
    ( *this )[mg] = NULL;
    // insert(std::make_pair(mg,static_cast<VectorInterface*>(NULL)));
  }
  else
  {
    std::cerr << "(register) already registered: " << mg << std::endl;
    std::cerr << "(register) registered are:\n" << *this << std::endl;
    assert(0);
    exit(1);
  }
}

/*--------------------------------------------------------------------------*/
Alat::VectorInterface* GhostVectorAgent::operator()(const Alat::GhostVector& g) const
{
  const_iterator p = find(g);
  if( p == end() )
  {
    std::cerr << ": GhostVectorAgent::operator(): ERROR"<<std::endl;
    std::cerr << ": Alat::GhostVector '"<< g <<"' not found in list of: "<<std::endl;
    std::cerr << " "<< *this << std::endl;
    assert(0);
    exit(1);
  }
  Alat::VectorInterface* vp = p->second;
  if(vp == NULL)
  {
    std::cerr <<  "GhostVectorAgent  GlobalVector* NULL\t" << p->first;
    std::cerr << "\n" << *this << std::endl;
    assert(0);
    exit(1);
  }
  return vp;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const GhostVectorAgent& gva)
{
  int i = 0, n = gva.size();
  os << "GhostVectorAgent: size=" << n << ", ";
  for(GhostVectorAgent::const_iterator p = gva.begin(); p != gva.end(); p++, i++)
  {
    os << "GhostVector("<<i<<")=('"<< p->first << "',"<< p->second->getName() <<")\n";
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
