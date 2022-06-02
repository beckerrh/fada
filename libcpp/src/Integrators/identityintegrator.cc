#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Alat/node.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  "Integrators/identityintegrator.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
IdentityIntegrator::~IdentityIntegrator(){}
IdentityIntegrator::IdentityIntegrator() : Fada::IntegratorSingleVariable(){}
IdentityIntegrator::IdentityIntegrator( const IdentityIntegrator& identityintegrator) : Fada::IntegratorSingleVariable(identityintegrator)
{
  assert(0);
}
IdentityIntegrator& IdentityIntegrator::operator=( const IdentityIntegrator& identityintegrator)
{
  Fada::IntegratorSingleVariable::operator=(identityintegrator);
  assert(0);
  return *this;
}
std::string IdentityIntegrator::getName() const
{
  return "IdentityIntegrator";
}
IdentityIntegrator* IdentityIntegrator::clone() const
{
  return new IdentityIntegrator(*this);
}

/*--------------------------------------------------------------------------*/
void IdentityIntegrator::initData()
{
  Fada::IntegratorSingleVariable::initData();
  _coefficient = getParameter("coefficient");
  assert(_ncompin==_ncompout);
}

/*--------------------------------------------------------------------------*/
void IdentityIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::Cells);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/
void IdentityIntegrator::formCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  double w = _coefficient*weight;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      (*_fL)(icomp,ii) += w* ( (*_UL)(icomp)*_femL->phi(ii) );
    }
  }
}

/*--------------------------------------------------------------------------*/

void IdentityIntegrator::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  double w = _coefficient*weight;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      for(int jj = 0; jj < _nlocal; jj++)
      {
        (*_ALL) (icomp, icomp, ii, jj) += w*( _femL->phi(jj)*_femL->phi(ii) );
      }
    }
  }
}
