#include  "Alat/armadillo.h"
#include  "Alat/systemassemblevector.h"
#include  "Alat/systemvector.h"
#include  "Alat/intvector.h"
#include  "Alat/stringdoublevectormap.h"
#include  "Alat/stringset.h"
#include  "Alat/stringvector.h"
#include  <cassert>
#include  <math.h>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SystemVector::~SystemVector()
{
  for(SystemVector::iterator p = this->begin(); p != this->end(); p++)
  {
    if( p->second )
    {
      delete p->second;
      p->second = NULL;
    }
  }
}

SystemVector::SystemVector() : Alat::Map<std::string, Alat::VariableVectorInterface*>(), Alat::VectorInterface()
{}
SystemVector::SystemVector( const SystemVector& systemvector) : Alat::Map<std::string, Alat::VariableVectorInterface*>(systemvector), Alat::VectorInterface(systemvector)
{
  for(SystemVector::const_iterator p = systemvector.begin(); p != systemvector.end(); p++)
  {
    ( *this )[p->first]  = p->second->clone();
  }
}

SystemVector& SystemVector::operator=( const SystemVector& systemvector)
{
 Alat::SystemVector::operator=(systemvector);
  assert(0);
  return *this;
}

std::string SystemVector::getName() const
{
  std::stringstream ss;
  ss << "SystemVector_";
  for(SystemVector::const_iterator p = this->begin(); p != this->end(); p++)
  {
    ss << p->first;
  }
  return ss.str();
}

SystemVector* SystemVector::clone() const
{
  return new SystemVector(*this);
}

/*----------------------------------------------------------*/
Alat::StringSet SystemVector::getVariables() const
{
  Alat::StringSet variables;
  for(SystemVector::const_iterator p = this->begin(); p != this->end(); p++)
  {
    variables.insert(p->first);
  }
  return variables;
}

/*----------------------------------------------------------*/
void SystemVector::set_size(const Alat::StringSet& variables)
{
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    ( *this )[*p] = NULL;
  }
}

int SystemVector::getNVars() const
{
  return size();
}

/*--------------------------------------------------------------------------*/
bool SystemVector::hasVector(std::string name) const
{
  return this->find(name) != this->end();
}

const Alat::VariableVectorInterface* SystemVector::getVector(std::string name) const
{
  return ( *this )[name];
}

Alat::VariableVectorInterface* SystemVector::getVector(std::string name)
{
  return ( *this )[name];
}

/*--------------------------------------------------------------------------*/
std::ostream& Alat::operator<<(std::ostream& s, const SystemVector& v)
{
  s << v.getName() << "\n";
  for(SystemVector::const_iterator p = v.begin(); p != v.end(); p++)
  {
    s << p->first << ": ";
    p->second->writeAscii(s);
    s << "\n";
  }
  return s;
}

/*--------------------------------------------------------------------------*/
std::ostream& SystemVector::writeAscii(std::ostream& os) const
{
  os << *this;
  return os;
}

/*--------------------------------------------------------------------------*/
void SystemVector::zeros()
{
  for(SystemVector::iterator p = this->begin(); p != this->end(); p++)
  {
    p->second->zeros();
  }
}

/*--------------------------------------------------------------------------*/
double SystemVector::scalarProduct(const Alat::VectorInterface* v) const
{
  const SystemVector* vv = dynamic_cast<const SystemVector*>( v );
  assert(vv);
  double d = 0.0;
  for(SystemVector::const_iterator p = vv->begin(); p != vv->end(); p++)
  {
    d += getVector(p->first)->scalarProduct( p->second );
  }
  return d;
}

void SystemVector::equal(double d)
{
  for(SystemVector::iterator p = this->begin(); p != this->end(); p++)
  {
    p->second->equal( d );
  }
}

void SystemVector::equal(const Alat::VectorInterface* v)
{
  const SystemVector* vv = dynamic_cast<const SystemVector*>( v );
  assert(vv);
  for(SystemVector::const_iterator p = vv->begin(); p != vv->end(); p++)
  {
    getVector(p->first)->equal( p->second );
  }
}

void SystemVector::add(const double& d, const Alat::VectorInterface* v)
{
  const SystemVector* vv = dynamic_cast<const SystemVector*>( v );
  assert(vv);
  for(SystemVector::const_iterator p = vv->begin(); p != vv->end(); p++)
  {
    getVector(p->first)->add( d, p->second );
  }
}

/*--------------------------------------------------------------------------*/
double SystemVector::norm() const
{
  return sqrt( scalarProduct(this) );
}
/*--------------------------------------------------------------------------*/
void SystemVector::scale(const double& d)
{
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    p->second->scale(d);
  }
}

/*--------------------------------------------------------------------------*/
void SystemVector::scalePerVariables(const Alat::StringDoubleVectorMap& scales)
{
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    // double d = scales[p->first];
    // p->second->scale(d);
    p->second->scale(scales[p->first]);
  }
}

void SystemVector::scalePerVariablesInverse(const Alat::StringDoubleVectorMap& scales)
{
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    // double d = 1.0/scales[p->first];
    // p->second->scale(d);
    p->second->scaleinv(scales[p->first]);
  }
}

/*----------------------------------------------------------*/
void SystemVector::savehdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames) const
{
  assert(blocknames.size()==size());
  Alat::armaivec ncomps(size()), ns(size());
  int i=0;
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    ncomps[i] = p->second->getNComponents();
    ns[i] = p->second->n();
    i++;
  }
  // std::cerr << "savehdf5 @@@@@@@@@@@@@@@ filename="<<filename<<"\n";
  ncomps.save(arma::hdf5_name(filename,"/ncomps"));
  ns.save(arma::hdf5_name(filename,"/ns",arma::hdf5_opts::append));
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    p->second->savehdf5(filename, blocknames[p->first]);
  }
}
/*----------------------------------------------------------*/
void SystemVector::loadhdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames)
{
  // std::cerr << "loadhdf5 @@@@@@@@@@@@@@@ filename="<<filename<<"\n";
  assert(blocknames.size()==size());
  Alat::armaivec ncomps, ns;
  ncomps.load(arma::hdf5_name(filename,"/ncomps"));
  ns.load(arma::hdf5_name(filename,"/ns"));
  for(SystemVector::const_iterator p = begin(); p != end(); p++)
  {
    p->second->loadhdf5(filename, blocknames[p->first]);
  }

}
