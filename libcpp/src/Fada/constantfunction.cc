#include  "Fada/constantfunction.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

ConstantFunction::~ConstantFunction()
{}

/*--------------------------------------------------------------------------*/

ConstantFunction::ConstantFunction(double d) : DataFunction(), _d(d)
{}

/*--------------------------------------------------------------------------*/

ConstantFunction::ConstantFunction( const ConstantFunction& constantfunction) : DataFunction(constantfunction), _d(constantfunction._d)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

ConstantFunction& ConstantFunction::operator=( const ConstantFunction& constantfunction)
{
  DataFunction::operator=(constantfunction);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string ConstantFunction::getName() const
{
  return "ConstantFunction";
}

/*--------------------------------------------------------------------------*/

ConstantFunction* ConstantFunction::clone() const
{
  return new ConstantFunction(*this);
}

/*--------------------------------------------------------------------------*/

double ConstantFunction::operator() (double x, double y, double z, double t) const
{
  return _d;
}
