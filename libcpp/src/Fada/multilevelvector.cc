#include  "Fada/multilevelvector.h"
#include  "Alat/systemvectorinterface.h"
#include  <cassert>
#include  <math.h>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelVector::~MultiLevelVector()
{
  for(int i = 0; i < size(); i++)
  {
    if( ( *this )[i] )
    {
      delete ( *this )[i];
      ( *this )[i] = NULL;
    }
  }
}
MultiLevelVector::MultiLevelVector() : Alat::Vector<Alat::SystemVectorInterface*>(), Alat::VectorInterface(){}
MultiLevelVector::MultiLevelVector( const MultiLevelVector& multilevelvector) : Alat::Vector<Alat::SystemVectorInterface*>(multilevelvector), Alat::VectorInterface(multilevelvector)
{
  int n = multilevelvector.size();
  set_size(n);
  for(int i = 0; i < n; i++)
  {
    ( *this )[i] = multilevelvector[i]->clone();
  }
}
MultiLevelVector& MultiLevelVector::operator=( const MultiLevelVector& multilevelvector)
{
  VectorInterface::operator=(multilevelvector);
  assert(0);
  return *this;
}
std::string MultiLevelVector::getName() const
{
  return "MultiLevelVector";
}
MultiLevelVector* MultiLevelVector::clone() const
{
  return new MultiLevelVector(*this);
}

/*--------------------------------------------------------------------------*/
const Alat::SystemVectorInterface* MultiLevelVector::getVector(int level) const
{
  return ( *this )[level];
}
Alat::SystemVectorInterface* MultiLevelVector::getVector(int level)
{
  return ( *this )[level];
}
Alat::SystemVectorInterface*& MultiLevelVector::getVectorPointer(int level)
{
  return ( *this )[level];
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& s, const MultiLevelVector& v)
{
  assert(0);
//  s << v.size() << "\n";
//  for(int i = 0; i < v.size(); i++)
//  {
//    s << v[i]->getName() << ": ";
//    v[i]->write(s, "ascii", "full");
//    s << "\n";
//  }
//  return s;
}

/*--------------------------------------------------------------------------*/
int MultiLevelVector::nlevels() const
{
  return size();
}

/*--------------------------------------------------------------------------*/
void MultiLevelVector::set_size(int nlevels)
{
  Alat::Vector<Alat::SystemVectorInterface*>::set_size(nlevels);
}

/*--------------------------------------------------------------------------*/
void MultiLevelVector::zeros()
{
  for(int level = 0; level < nlevels(); level++)
  {
    getVector(level)->zeros();
  }
}

/*--------------------------------------------------------------------------*/
double MultiLevelVector::norm() const
{
  return sqrt( scalarProduct(this) );
}
double MultiLevelVector::norm(int level) const
{
  return sqrt( getVector(level)->norm() );
}

/*--------------------------------------------------------------------------*/
double MultiLevelVector::scalarProduct(const MultiLevelVector* v) const
{
  double d = 0.0;
  for(int level = 0; level < nlevels(); level++)
  {
    d += getVector(level)->scalarProduct( v->getVector(level) );
  }
  return d;
}

/*--------------------------------------------------------------------------*/
void MultiLevelVector::equal(const MultiLevelVector* v)
{
  for(int level = 0; level < nlevels(); level++)
  {
    getVector(level)->equal( v->getVector(level) );
  }
}
void MultiLevelVector::equal(double d)
{
  for(int level = 0; level < nlevels(); level++)
  {
    getVector(level)->equal( d );
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelVector::add(const double& d, const MultiLevelVector* v)
{
  for(int level = 0; level < nlevels(); level++)
  {
    getVector(level)->add( d, v->getVector(level) );
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelVector::scale(const double& d)
{
  for(int level = 0; level < nlevels(); level++)
  {
    getVector(level)->scale( d );
  }
}

/*--------------------------------------------------------------------------*/
std::ostream& MultiLevelVector::writeAscii(std::ostream& os) const
{
  for(int level = 0; level < nlevels(); level++)
  {
    os << "level = " << level<< "\n";
    getVector(level)->writeAscii( os );
  }
  return os;
}
