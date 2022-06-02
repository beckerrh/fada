#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
AssembleMatrix& SystemAssembleMatrix::operator()(std::string ivar, std::string jvar)
{
  return (*this)[Alat::makePair(ivar,jvar)];
}

const AssembleMatrix& SystemAssembleMatrix::operator()(std::string ivar, std::string jvar) const
{
  return (*this)[Alat::makePair(ivar,jvar)];
}
bool SystemAssembleMatrix::hasKey(std::string ivar, std::string jvar) const
{
  return Alat::Map<Alat::StringPair, AssembleMatrix>::hasKey(Alat::makePair(ivar,jvar));
}

/*--------------------------------------------------------------------------*/
SystemAssembleMatrix::~SystemAssembleMatrix(){}
SystemAssembleMatrix::SystemAssembleMatrix() : Alat::Map<Alat::StringPair, AssembleMatrix>(){}
SystemAssembleMatrix::SystemAssembleMatrix( const SystemAssembleMatrix& systemassemblematrix) : Alat::Map<Alat::StringPair, AssembleMatrix>(systemassemblematrix)
{
}

SystemAssembleMatrix& SystemAssembleMatrix::operator=( const SystemAssembleMatrix& systemassemblematrix)
{
  Alat::Map<Alat::StringPair, AssembleMatrix>::operator=(systemassemblematrix);
  assert(0);
  return *this;
}

std::string SystemAssembleMatrix::getName() const
{
  return "SystemAssembleMatrix";
}

/*--------------------------------------------------------------------------*/
void SystemAssembleMatrix::zeros()
{
  for(Alat::Map<Alat::StringPair, AssembleMatrix>::iterator p=begin(); p!=end(); p++)
  {
    p->second.zeros();
  }
}

/*--------------------------------------------------------------------------*/
void SystemAssembleMatrix::set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& mcomp, const Alat::StringIntMap& n, const Alat::StringIntMap& m)
{
  clear();
  assert(ncomp.size()==n.size());
  assert(mcomp.size()==m.size());
  for(Alat::StringIntMap::const_iterator p = ncomp.begin(); p!= ncomp.end(); p++)
  {
    for(Alat::StringIntMap::const_iterator q = mcomp.begin(); q!= mcomp.end(); q++)
    {
      (*this)[Alat::makePair(p->first, q->first)].set_size(p->second, q->second, n[p->first], m[q->first]);
    }
  }
}

/*--------------------------------------------------------------------------*/
void SystemAssembleMatrix::set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& nloc)
{
  set_size(ncomp, ncomp, nloc, nloc);
}

/*--------------------------------------------------------------------------*/
void SystemAssembleMatrix::productVector(SystemAssembleVector& y, const Alat::SystemAssembleVector& x, double d) const
{
  for(Alat::Map<Alat::StringPair, AssembleMatrix>::const_iterator p=begin(); p!=end(); p++)
  {
    p->second.productVector(y[p->first.first], x[p->first.second], d);
  }
}

/*--------------------------------------------------------------------------*/
void SystemAssembleMatrix::productVectorTransposed(SystemAssembleVector& y, const Alat::SystemAssembleVector& x, double d) const
{
  for(Alat::Map<Alat::StringPair, AssembleMatrix>::const_iterator p=begin(); p!=end(); p++)
  {
    p->second.productVectorTransposed(y[p->first.second], x[p->first.first], d);
  }
}
