#include  "Fada/boundarymanager.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  "Integrators/laplaceintegrator.h"
#include  "Fada/changeintegrationpoint.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
LaplaceIntegrator::~LaplaceIntegrator(){}
LaplaceIntegrator::LaplaceIntegrator() : Fada::IntegratorSingleVariable(){}
LaplaceIntegrator::LaplaceIntegrator( const LaplaceIntegrator& laplaceintegrator) : Fada::IntegratorSingleVariable(laplaceintegrator){}
LaplaceIntegrator& LaplaceIntegrator::operator=( const LaplaceIntegrator& laplaceintegrator)
{
  Fada::IntegratorSingleVariable::operator=(laplaceintegrator);
  assert(0);
  return *this;
}
LaplaceIntegrator* LaplaceIntegrator::clone() const
{
  return new LaplaceIntegrator(*this);
}
std::string LaplaceIntegrator::getName() const
{
  return "LaplaceIntegrator";
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::initData()
{
  Fada::IntegratorSingleVariable::initData("Dirichlet");
  _diffusion = getParameter("diffusion");
  _boundarypenalty = getParameter("boundarypenalty");
  _interiorpenalty = getParameter("interiorpenalty");
  _interiorderivativepenalty = getParameter("interiorderivativepenalty", 0.0);
  _sigma = getParameter("nitschesigma", 1.0);
  _penalty =  ( getStringParameter("penalty", "no") == "yes" );
  assert(_ncompin==_ncompout);
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::Cells);
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::CouplingSides);
  addDomainsOfTerm(FadaEnums::Form, FadaEnums::BoundarySides);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::Cells);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::CouplingSides);
  addDomainsOfTerm(FadaEnums::Matrix, FadaEnums::BoundarySides);
  addDomainsOfTerm(FadaEnums::RHS, FadaEnums::BoundarySides);
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const
{
  double w = _diffusion*weight;
  double ws = w*_boundarypenalty/_femL->getHOfCell();
  if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  // if( femd._boundaryconditions[_indexout] == "Neumann" )
  {
    return;
  }
  // if(femd._boundaryconditions[_indexout] != "Dirichlet")
  if( not _boundarymanager->boundaryConditionIs("Dirichlet", color) )
  {
    std::cerr << "*** ERROR: unknown Boundary condition \'"  << _boundarymanager->boundaryConditions(color)  <<  "\' for color= "<<color<<"\n";
    assert(0);
  }
  // const Alat::DoubleVector& udir = femd.dirichletOfVarOfBdryCond("Dirichlet",_varname);
  assert(_dirichlet);
  assert(_femL);
  _dirichlet->getValue( _udir, color, _femL->getNormal(), _femL->x().x(), _femL->x().y(), _femL->x().z(), _time );
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    // std::cerr << icomp << " " << femd._udir[_indexin][icomp] << "\n";
    for(int ii = 0; ii < _nlocal; ii++)
    {
      (*_fL)(icomp,ii) += ws*( _udir[icomp]*_femL->phi(ii) );
      if(not _penalty)
      {
        (*_fL)(icomp,ii) -= _sigma*w *( _udir[icomp]*arma::dot(_femL->getNormal(),_femL->dphi(ii)) );
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::formCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  // std::cerr << "LaplaceIntegrator::formCell() " << getIntegrationFormula()->getName() << "\n";
  double w = _diffusion*weight;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      (*_fL)(icomp,ii) += w* arma::dot((*_UL).d(icomp), _femL->dphi(ii) );
    }
  }
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  double w = _diffusion*weight;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      for(int jj = 0; jj < _nlocal; jj++)
      {
        (*_ALL) (icomp, icomp, ii, jj) += w* arma::dot( _femL->dphi(jj), _femL->dphi(ii) );
      }
    }
  }
  // std::cerr <<(*_ALL)<<"\n";
  // if((*_ALL).norm()<1e-14)
  // {
  //   std::cerr << "_diffusion="<<_diffusion<<"\n";
  //   std::cerr << "_nlocal="<<_nlocal<<"\n";
  //   std::cerr << "_nlocal="<<_nlocal<<"\n";
  //   assert(0);
  // }
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  double w = _diffusion*weight;
  double ws = w*_boundarypenalty/_femL->getHOfCell();
  // if( femd._boundaryconditions[_indexout] == "Neumann" )
    if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  {
    return;
  }
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      if(not _penalty)
      {
        // assert(0);
        (*_fL)(icomp,ii) -= _sigma*w*arma::dot(_femL->getNormal(),_femL->dphi(ii))*(*_UL)(icomp);
        (*_fL)(icomp,ii) -= w* arma::dot( _femL->getNormal(), (*_UL).d(icomp) )*_femL->phi(ii);
      }
      (*_fL)(icomp,ii) += ws*_femL->phi(ii)*(*_UL)(icomp);
    }
  }
}

/*--------------------------------------------------------------------------*/

void LaplaceIntegrator::matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  double w = _diffusion*weight;
  double ws = w*_boundarypenalty/_femL->getHOfCell();
  if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  // if( femd._boundaryconditions[_indexout] == "Neumann" )
  {
    return;
  }
  for(int ii = 0; ii < _nlocal; ii++)
  {
    for(int jj = 0; jj < _nlocal; jj++)
    {
      double d1  = 0.0, d2 = 0.0;
      if(not _penalty)
      {
        d1 = _sigma*w*arma::dot( _femL->getNormal(), _femL->dphi(ii) )* _femL->phi(jj);
        d2 = w*arma::dot( _femL->getNormal(), _femL->dphi(jj) )* _femL->phi(ii);
      }
      double d3 = ws* _femL->phi(ii)* _femL->phi(jj);
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        (*_ALL) (icomp, icomp, ii, jj) += d3-d1-d2;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  double w = 0.5*_diffusion* weight;
  double h = _femL->getHOfCell();
  double ws = w*_interiorpenalty/h;
  double wj = w*_interiorderivativepenalty*h;
  const Alat::Node& normal = _femL->getNormal();
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    double ujump = (*_UL)(icomp)-(*_UR)(icomp);
    double dum = 0.5*( arma::dot((*_UL).d(icomp),normal) + arma::dot((*_UR).d(icomp),normal) );
    double duj =  arma::dot((*_UL).d(icomp),normal) - arma::dot((*_UR).d(icomp),normal);

    for(int ii = 0; ii < _nlocal; ii++)
    {
      double ndphil = arma::dot(normal, _femL->dphi(ii));
      double ndphir = arma::dot(normal, _femR->dphi(ii));
      (*_fL)(icomp,ii) -= 2.0*w* dum*_femL->phi(ii);
      (*_fR)(icomp,ii) += 2.0*w* dum*_femR->phi(ii);
      (*_fL)(icomp,ii) -= _sigma*w*ndphil*ujump;
      (*_fR)(icomp,ii) -= _sigma*w*ndphir*ujump;
      (*_fL)(icomp,ii) += ws*_femL->phi(ii)*ujump;
      (*_fR)(icomp,ii) -= ws*_femR->phi(ii)*ujump;
      (*_fL)(icomp,ii) += wj*ndphil*duj;
      (*_fR)(icomp,ii) -= wj*ndphir*duj;
    }
  }
}

/*--------------------------------------------------------------------------*/
void LaplaceIntegrator::matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const
{
  double w = 0.5*_diffusion* weight;
  double h = _femL->getHOfCell();
  double ws = w*_interiorpenalty/h;
  double wj = w*_interiorderivativepenalty*h;
  const Alat::Node& normal = _femL->getNormal();
  for(int ii = 0; ii < _nlocal; ii++)
  {
    double ndphil = arma::dot(normal, _femL->dphi(ii));
    double ndphir = arma::dot(normal, _femR->dphi(ii));
    for(int jj = 0; jj < _nlocal; jj++)
    {
      double ndphjl = arma::dot(normal, _femL->dphi(jj));
      double ndphjr = arma::dot(normal, _femR->dphi(jj));
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        (*_ALL) (icomp, icomp, ii, jj) -= w*_femL->phi(ii)*ndphjl;
        (*_ALR) (icomp, icomp, ii, jj) -= w*_femL->phi(ii)*ndphjr;
        (*_ARL) (icomp, icomp, ii, jj) += w*_femR->phi(ii)*ndphjl;
        (*_ARR) (icomp, icomp, ii, jj) += w*_femR->phi(ii)*ndphjr;

        (*_ALL) (icomp, icomp, ii, jj) -= _sigma*w*ndphil*_femL->phi(jj);
        (*_ALR) (icomp, icomp, ii, jj) += _sigma*w*ndphil*_femR->phi(jj);
        (*_ARL) (icomp, icomp, ii, jj) -= _sigma*w*ndphir*_femL->phi(jj);
        (*_ARR) (icomp, icomp, ii, jj) += _sigma*w*ndphir*_femR->phi(jj);

        (*_ALL) (icomp, icomp, ii, jj) += ws*_femL->phi(ii)*_femL->phi(jj);
        (*_ALR) (icomp, icomp, ii, jj) -= ws*_femL->phi(ii)*_femR->phi(jj);
        (*_ARL) (icomp, icomp, ii, jj) -= ws*_femR->phi(ii)*_femL->phi(jj);
        (*_ARR) (icomp, icomp, ii, jj) += ws*_femR->phi(ii)*_femR->phi(jj);

        (*_ALL) (icomp, icomp, ii, jj) += wj*ndphil*ndphjl;
        (*_ALR) (icomp, icomp, ii, jj) -= wj*ndphil*ndphjr;
        (*_ARL) (icomp, icomp, ii, jj) -= wj*ndphir*ndphjl;
        (*_ARR) (icomp, icomp, ii, jj) += wj*ndphir*ndphjr;
      }
    }
  }
}
