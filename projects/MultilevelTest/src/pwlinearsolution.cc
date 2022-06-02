#include  "pwlinearsolution.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/

PwLinearSolution::~PwLinearSolution()
{}

PwLinearSolution::PwLinearSolution() : Fada::DataFunction()
{}

PwLinearSolution::PwLinearSolution( const PwLinearSolution& pwlinearsolution) : Fada::DataFunction(pwlinearsolution)
{
  ( *this ).operator=(pwlinearsolution);
}

PwLinearSolution& PwLinearSolution::operator=( const PwLinearSolution& pwlinearsolution)
{
  Fada::DataFunction::operator=(pwlinearsolution);
  assert(0);
  return *this;
}

std::string PwLinearSolution::getName() const
{
  return "PwLinearSolution";
}

PwLinearSolution* PwLinearSolution::clone() const
{
  return new PwLinearSolution(*this);
}

/*--------------------------------------------------------------------------*/

double PwLinearSolution::operator()(double x, double y, double z, double t) const
{
  if(x <= 1.0)
  {
    return 3.*x + 7.0;
  }
  return 10.*( 2.0-x );
}
double PwLinearSolution::x(double x, double y, double z, double t) const
{
  if(x <= 1.0)
  {
    return 3.0;
  }
  return -10.;
}

double PwLinearSolution::y(double x, double y, double z, double t) const
{
  return 0.0;
}
double PwLinearSolution::xx(double x, double y, double z, double t) const
{
  return 0.0;
}
double PwLinearSolution::yy(double x, double y, double z, double t) const
{
  return 0.0;
}

