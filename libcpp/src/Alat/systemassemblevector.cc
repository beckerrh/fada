#include  "Alat/systemassemblevector.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SystemAssembleVector::~SystemAssembleVector(){}
SystemAssembleVector::SystemAssembleVector() : Alat::Map<std::string, Alat::AssembleVector>(){}
SystemAssembleVector::SystemAssembleVector( const Alat::SystemAssembleVector& systemassemblevector) : Alat::Map<std::string, Alat::AssembleVector>(systemassemblevector){}
SystemAssembleVector& SystemAssembleVector::operator=( const Alat::SystemAssembleVector& systemassemblevector)
{
  Alat::Map<std::string, Alat::AssembleVector>::operator=(systemassemblevector);
  return *this;
}
std::string SystemAssembleVector::getName() const
{
  return "SystemAssembleVector";
}

/*--------------------------------------------------------------------------*/
void SystemAssembleVector::zeros()
{
  for(Alat::Map<std::string, Alat::AssembleVector>::iterator p = begin(); p!= end(); p++)
  {
    p->second.zeros();
  }
}

/*--------------------------------------------------------------------------*/
void SystemAssembleVector::set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& nloc)
{
  // std::cerr << "SystemAssembleVector::reInit( ncomp="<<ncomp<<" n="<<n<<"\n";
  clear();
  for(Alat::StringIntMap::const_iterator p = ncomp.begin(); p!= ncomp.end(); p++)
  {
    (*this)[p->first].set_size(ncomp[p->first], nloc[p->first]);
  }
}

/*--------------------------------------------------------------------------*/
double SystemAssembleVector::operator*(const Alat::SystemAssembleVector& v) const
{
  double d = 0.0;
  for(Alat::Map<std::string, Alat::AssembleVector>::const_iterator p = begin(); p!= end(); p++)
  {
    d += p->second * v[p->first];
  }
  return d;
}
