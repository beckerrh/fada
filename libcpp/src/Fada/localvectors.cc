#include  "Fada/localvectors.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LocalVectors::~LocalVectors(){}
LocalVectors::LocalVectors(){}
LocalVectors::LocalVectors( const LocalVectors& localvectors){}
LocalVectors& LocalVectors::operator=( const LocalVectors& localvectors)
{
  assert(0);
  return *this;
}
std::string LocalVectors::getName() const
{
  return "LocalVectors";
}

/*--------------------------------------------------------------------------*/
void LocalVectors::clear()
{
  _vectormap.clear();
  _dataloc.clear();
}

/*--------------------------------------------------------------------------*/
void LocalVectors::set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& n, const GlobalVectorMap& vectormap, const Alat::StringIntMap& ncompdata, const Alat::StringIntMap& ndata)
{
  _vectormap.clear();
  for(GlobalVectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
  {
    _vectormap[p->first].set_size(ncomp, n);
  }
  _dataloc.set_size(ncompdata, ndata);
}

/*--------------------------------------------------------------------------*/
Alat::SystemAssembleVector& LocalVectors::getVector(std::string name)
{
  return _vectormap[name];
}

const Alat::SystemAssembleVector& LocalVectors::getVector(std::string name) const
{
  return _vectormap[name];
}

const Fada::LocalVectors::LocalVectorMap& LocalVectors::getVectorMap() const
{
  return _vectormap;
}

Alat::SystemAssembleVector& LocalVectors::getData()
{
  return _dataloc;
}

const Alat::SystemAssembleVector& LocalVectors::getData() const
{
  return _dataloc;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const LocalVectors& g)
{
  os << "_dataloc="<<g.getData()<<"\n";
  for( LocalVectors::LocalVectorMap::const_iterator p = g.getVectorMap().begin(); p != g.getVectorMap().end(); p++ )
  {
    os << p->first << " => " << p->second << "\n";
  }
  return os;
}
