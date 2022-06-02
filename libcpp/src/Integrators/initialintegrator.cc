#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Alat/node.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  "Integrators/initialintegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
InitialIntegrator::~InitialIntegrator(){}
InitialIntegrator::InitialIntegrator() : Fada::Integrator(){}
InitialIntegrator::InitialIntegrator( const InitialIntegrator& initialIntegrator) : Fada::Integrator(initialIntegrator)
{
  assert(0);
}
InitialIntegrator& InitialIntegrator::operator=( const InitialIntegrator& initialIntegrator)
{
  Fada::Integrator::operator=(initialIntegrator);
  assert(0);
  return *this;
}
std::string InitialIntegrator::getName() const
{
  return "InitialIntegrator";
}
InitialIntegrator* InitialIntegrator::clone() const
{
  return new InitialIntegrator(*this);
}

/*--------------------------------------------------------------------------*/
void InitialIntegrator::initData(){}

/*--------------------------------------------------------------------------*/

void InitialIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::Initialize, FadaEnums::Cells);
}
