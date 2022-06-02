#include  "Fada/functionalinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

FunctionalInterface::~FunctionalInterface()
{}

/*--------------------------------------------------------------------------*/

FunctionalInterface::FunctionalInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

FunctionalInterface::FunctionalInterface( const FunctionalInterface& functionalinterface) : Alat::InterfaceBase(functionalinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

FunctionalInterface& FunctionalInterface::operator=( const FunctionalInterface& functionalinterface)
{
  InterfaceBase::operator=(functionalinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string FunctionalInterface::getName() const
{
  return "FunctionalInterface";
}

/*--------------------------------------------------------------------------*/

void FunctionalInterface::basicInit(const Alat::ParameterFile* parameterfile, const Fada::ProblemData* model_problemdata, const Fada::ParameterMap<double>& phys_params, const Fada::ParameterMap<double>& num_params)
{
  _notWritten("basicInit");
}

/*--------------------------------------------------------------------------*/

Fada::IntegratorInterface* FunctionalInterface::getIntegrator() const
{
  _notWritten("getIntegrator");
}
