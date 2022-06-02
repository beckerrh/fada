#include  "Alat/assemblevector.h"
#include  "Fada/femfunctions.h"
#include  "Fada/feminterface.h"
#include  "Alat/intvector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
FemFunctions::~FemFunctions(){}
FemFunctions::FemFunctions() :  Alat::Map<std::string, Fada::FemFunction>(){}  
FemFunctions::FemFunctions( const Fada::FemFunctions& femfunctions) : Alat::Map<std::string, Fada::FemFunction>(femfunctions)
{
  ( *this ).operator=(femfunctions);
}
FemFunctions& FemFunctions::operator=( const Fada::FemFunctions& femfunctions)
{
  Alat::Map<std::string, Fada::FemFunction>::operator=(femfunctions);
  return *this;
}
std::string FemFunctions::getName() const
{
  return "FemFunctions";
}
FemFunctions* FemFunctions::clone() const
{
  return new FemFunctions(*this);
}

/*--------------------------------------------------------------------------*/
void FemFunctions::basicInit(const Alat::StringIntMap& ncomps)
{
  clear();
  for(Alat::StringIntMap::const_iterator p=ncomps.begin(); p!=ncomps.end(); p++)
  {
    (*this)[p->first].set_size(p->second);
  }
}

void FemFunctions::reInitAndEqual(const Fada::FemFunctions& other)
{
  clear();
  for(Fada::FemFunctions::const_iterator p=other.begin(); p!=other.end(); p++)
  {
    (*this)[p->first].reInitAndEqual(p->second);
  }
}

/*--------------------------------------------------------------------------*/
const Alat::DoubleVector& FemFunctions::operator()(std::string ivar) const
{
  return (*this)[ivar]();
}

const double& FemFunctions::operator()(std::string ivar, int icomp) const
{
  return (*this)[ivar](icomp);
}

double& FemFunctions::operator()(std::string ivar, int icomp)
{
  return (*this)[ivar](icomp);
}

const Alat::Node& FemFunctions::d(std::string ivar, int icomp) const
{
  return (*this)[ivar].d(icomp);
}

Alat::Node& FemFunctions::d(std::string ivar, int icomp)
{
  return (*this)[ivar].d(icomp);
}

/*--------------------------------------------------------------------------*/
void FemFunctions::computeFunctionAndGradient(std::string ivar, const Fada::FemInterface* fem, const Alat::AssembleVector& uloc)
{
  if(fem)
  {
    fem->computeFunctionAndGradient((*this)[ivar](), (*this)[ivar].d(), uloc);
  }
  else
  {
    for(int icomp = 0; icomp < (*this)[ivar].ncomp(); icomp++)
    {
      assert((*this)[ivar]().size() == 1);
      // (*this)[ivar](icomp) = uloc[icomp][0];
      (*this)[ivar](icomp) = uloc(icomp,0);
    }
  }
}
