#include  "Integrators/transportdgintegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

TransportDgIntegrator::~TransportDgIntegrator()
{}

/*--------------------------------------------------------------------------*/

TransportDgIntegrator::TransportDgIntegrator() : TransportIntegrator()
{}

/*--------------------------------------------------------------------------*/

TransportDgIntegrator::TransportDgIntegrator( const TransportDgIntegrator& transportdgintegrator) : TransportIntegrator(transportdgintegrator)
{}

/*--------------------------------------------------------------------------*/

TransportDgIntegrator& TransportDgIntegrator::operator=( const TransportDgIntegrator& transportdgintegrator)
{
  TransportIntegrator::operator=(transportdgintegrator);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string TransportDgIntegrator::getName() const
{
  return "TransportDgIntegrator";
}

/*--------------------------------------------------------------------------*/

TransportDgIntegrator* TransportDgIntegrator::clone() const
{
  return new TransportDgIntegrator(*this);
}

/*--------------------------------------------------------------------------*/

void TransportDgIntegrator::setDomainsOfTerms()
{
  TransportIntegrator::setDomainsOfTerms();
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::InteriorSides);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::InteriorSides);
}
