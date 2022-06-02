#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Alat/systemassemblematrix.h"
#include  "Integrators/trustregion.h"
#include  "Alat/stringdoublemap.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
TrustRegion::~TrustRegion(){}
TrustRegion::TrustRegion() : Fada::IntegratorSingleVariable(){}
TrustRegion::TrustRegion( const TrustRegion& trustregion) : Fada::IntegratorSingleVariable(trustregion)
{
  ( *this ).operator=(trustregion);
}
TrustRegion& TrustRegion::operator=( const TrustRegion& trustregion)
{
  Fada::IntegratorSingleVariable::operator=(trustregion);
  assert(0);
  return *this;
}
std::string TrustRegion::getName() const
{
  return "TrustRegion";
}
TrustRegion* TrustRegion::clone() const
{
  return new TrustRegion(*this);
}

/*--------------------------------------------------------------------------*/
void TrustRegion::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::ComputeNormSquared, FadaEnums::Cells);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/
void TrustRegion::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  // std::cerr << "TrustRegion::matrixCell() _trparameter="<<_trparameter<<"\n";
  if(_trparameter == 0.0)
  {
    return;
  }
  double w = weight*_trparameter;
  for(int ii = 0; ii < _nlocal; ii++)
  {
    for(int jj = 0; jj < _nlocal; jj++)
    {
      double mass =  _femL->phi(jj)*_femL->phi(ii);
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        (*_ALL) (icomp, icomp, ii, jj) += w*mass;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void TrustRegion::computeNormSquaredCell(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  double result = 0.0;
  for(int icomp = 0; icomp < _ncompin; icomp++)
  {
    result += weight *(*_UL)(icomp)*(*_UL)(icomp);
  }
  norms[_varnameout] += result;
}
