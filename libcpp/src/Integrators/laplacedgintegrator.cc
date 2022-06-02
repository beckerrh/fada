#include  "Integrators/laplacedgintegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

LaplaceDgIntegrator::~LaplaceDgIntegrator()
{}

/*--------------------------------------------------------------------------*/

LaplaceDgIntegrator::LaplaceDgIntegrator() : LaplaceIntegrator()
{}

/*--------------------------------------------------------------------------*/

LaplaceDgIntegrator::LaplaceDgIntegrator( const LaplaceDgIntegrator& laplacedgintegrator) : LaplaceIntegrator(laplacedgintegrator)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

LaplaceDgIntegrator& LaplaceDgIntegrator::operator=( const LaplaceDgIntegrator& laplacedgintegrator)
{
  LaplaceIntegrator::operator=(laplacedgintegrator);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string LaplaceDgIntegrator::getName() const
{
  return "LaplaceDgIntegrator";
}

/*--------------------------------------------------------------------------*/

LaplaceDgIntegrator* LaplaceDgIntegrator::clone() const
{
  return new LaplaceDgIntegrator(*this);
}

/*--------------------------------------------------------------------------*/

void LaplaceDgIntegrator::setDomainsOfTerms()
{
  LaplaceIntegrator::setDomainsOfTerms();
  addDomainsOfTerm(FadaEnums::Form  , FadaEnums::InteriorSides);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::InteriorSides);
}
