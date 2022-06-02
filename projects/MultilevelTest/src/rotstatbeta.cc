#include  "Fada/constantfunction.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/neumannzero.h"
#include  "Fada/problemdata.h"
#include  "rotstatbeta.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/

std::string RotStatBetax::getName() const
{
  return "RotStatBetax";
}

double RotStatBetax::operator()(double x, double y, double z, double t) const
{
  return 1.0+y;
}

/*--------------------------------------------------------------------------*/

std::string RotStatBetay::getName() const
{
  return "RotStatBetay";
}

double RotStatBetay::operator()(double x, double y, double z, double t) const
{
  return -( 1.0+x );
}

/*--------------------------------------------------------------------------*/

std::string RotStatBetaz::getName() const
{
  return "RotStatBetaz";
}

double RotStatBetaz::operator()(double x, double y, double z, double t) const
{
  return 0.5;
}
