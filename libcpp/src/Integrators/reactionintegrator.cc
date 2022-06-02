#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/doublematrix.h"
#include  "Alat/node.h"
#include  "Integrators/reactionintegrator.h"
#include  "Integrators/reactioninterface.h"
#include  <cassert>
#include  "Fada/varinfointegrator.h"
#include  "Fada/femmanager.h"

using namespace Integrators;

/*--------------------------------------------------------------------------*/
ReactionIntegrator::~ReactionIntegrator(){}
ReactionIntegrator::ReactionIntegrator() : Fada::IntegratorSingleVariable(){}
ReactionIntegrator::ReactionIntegrator( const ReactionIntegrator& reactionintegrator) : Fada::IntegratorSingleVariable(reactionintegrator)
{
  assert(0);
}
ReactionIntegrator& ReactionIntegrator::operator=( const ReactionIntegrator& reactionintegrator)
{
  Fada::IntegratorSingleVariable::operator=(reactionintegrator);
  assert(0);
  return *this;
}
std::string ReactionIntegrator::getName() const
{
  return "ReactionIntegrator";
}
ReactionIntegrator* ReactionIntegrator::clone() const
{
  return new ReactionIntegrator(*this);
}

/*--------------------------------------------------------------------------*/
void ReactionIntegrator::initData()
{
  Fada::IntegratorSingleVariable::initData();
  const Fada::DataInterface* data = getData("reaction");
  _reaction = dynamic_cast<const ReactionInterface*>( data );
  assert(_reaction);
  assert(_ncompin==_ncompout);
}

/*--------------------------------------------------------------------------*/
void ReactionIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::Cells);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/
void ReactionIntegrator::formCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  Alat::DoubleVector r(_ncompout);
  _reaction->reaction( r, (*_UL)(), _femL->x().x(), _femL->x().y(), _femL->x().z(), getTime() );
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      (*_fL)(icomp,ii) += weight*r[icomp]*_femL->phi(ii);
    }
  }
}

/*--------------------------------------------------------------------------*/
void ReactionIntegrator::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  Alat::DoubleMatrix rprime(_ncompout, _ncompout);
  _reaction->reactionPrime( rprime, (*_UL)(), _femL->x().x(), _femL->x().y(), _femL->x().z(), getTime() );
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int jcomp = 0; jcomp < _ncompout; jcomp++)
    {
      for(int ii = 0; ii < _nlocal; ii++)
      {
        for(int jj = 0; jj < _nlocal; jj++)
        {
          double mass = weight*_femL->phi(jj)*_femL->phi(ii);
          (*_ALL) (icomp, jcomp, ii, jj) += mass*rprime(icomp, jcomp);
        }
      }
    }
  }
}
