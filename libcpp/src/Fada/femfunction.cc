#include  "Alat/assemblevector.h"
#include  "Fada/femfunction.h"
#include  "Fada/feminterface.h"
#include  "Alat/intvector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
FemFunction::~FemFunction(){}
FemFunction::FemFunction() :  _u(), _du(){}  
FemFunction::FemFunction( const Fada::FemFunction& femfunctions)
{
  _u = femfunctions._u;
  _du = femfunctions._du;
}
FemFunction& FemFunction::operator=( const Fada::FemFunction& femfunctions)
{
  _u = femfunctions._u;
  _du = femfunctions._du;
  return *this;
}
std::string FemFunction::getName() const
{
  return "FemFunction";
}
FemFunction* FemFunction::clone() const
{
  return new FemFunction(*this);
}

/*--------------------------------------------------------------------------*/
int FemFunction::ncomp() const
{
  return _u.size();
}

void FemFunction::set_size(int ncomp)
{
  _u.set_size(ncomp);
  _du.set_size(ncomp);
}

void FemFunction::reInitAndEqual(const Fada::FemFunction& other)
{
  int ncomp = other.ncomp();
  _u.set_size(ncomp);
  _du.set_size(ncomp);
  for(int icomp=0;icomp<ncomp;icomp++)
  {
    _u[icomp] = other(icomp);
    _du[icomp] = other.d(icomp);
  }
}

/*--------------------------------------------------------------------------*/
const Alat::DoubleVector& FemFunction::operator()() const
{
  return _u;
}
Alat::DoubleVector& FemFunction::operator()()
{
  return _u;
}
const Alat::Vector<Alat::Node>& FemFunction::d() const
{
  return _du;
}
Alat::Vector<Alat::Node>& FemFunction::d()
{
  return _du;
}

// const double& FemFunction::operator()(int icomp) const
// {
//   return _u[icomp];
// }
// double& FemFunction::operator()(int icomp)
// {
//   return _u[icomp];
// }
// const Alat::Node& FemFunction::d(int icomp) const
// {
//   return _du[icomp];
// }
// Alat::Node& FemFunction::d(int icomp)
// {
//   return _du[icomp];
// }

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const FemFunction& g)
{
  os << "u="<<g() <<"\n";
  os << "d="<<g.d() <<"\n";
  return os;
}
