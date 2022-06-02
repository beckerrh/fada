#include  "Fada/boundarymanager.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/modelinterface.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemassemblevector.h"
#include  "transportintegrator.h"
#include  "Fada/changeintegrationpoint.h"
#include  "Fada/integrationformulaline.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>
#include  <math.h>

// using namespace Integrators;

/*--------------------------------------------------------------------------*/
TransportIntegrator::~TransportIntegrator()
{
  if(_IFboundarytrapez)
  {
    delete _IFboundarytrapez;
    _IFboundarytrapez = NULL;
  }
}
TransportIntegrator::TransportIntegrator() : Fada::IntegratorSingleVariable(), _IFboundarytrapez(NULL){}
TransportIntegrator::TransportIntegrator( const TransportIntegrator& transportintegrator) : Fada::IntegratorSingleVariable(transportintegrator){}
TransportIntegrator& TransportIntegrator::operator=( const TransportIntegrator& transportintegrator)
{
  Fada::IntegratorSingleVariable::operator=(transportintegrator);
  return *this;
}
TransportIntegrator* TransportIntegrator::clone() const
{
  return new TransportIntegrator(*this);
}
std::string TransportIntegrator::getName() const
{
  return "TransportIntegrator";
}

/*--------------------------------------------------------------------------*/
void TransportIntegrator::initData()
{
  Fada::IntegratorSingleVariable::initData("Dirichlet");
  _symmetrytype =  getModel()->getStringParameter("symmetrytype");
  assert((_symmetrytype=="primal")||(_symmetrytype=="dual")||(_symmetrytype=="skew"));
  if(getDimension() == 2)
  {
    _IFboundarytrapez = new Fada::LineTrapez;
  }
  else
  {
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
void TransportIntegrator::setDomainsOfTerms()
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
void TransportIntegrator::rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const
{
  Alat::Node beta;
  getData("beta")->getValue(beta, _femL->x());
  double bn = arma::dot(beta, _femL->getNormal());
  double bnm = fmin(0.0, bn);
  if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  {
    return;
  }
  if( not _boundarymanager->boundaryConditionIs("Dirichlet", color) )
  {
    std::cerr<<"*** TransportIntegrator::rhsBoundarySide(): unknown boundary_condition  for color "<<color<< "\n";
    assert(0);
  }
  _dirichlet->getValue( _udir, color, _femL->getNormal(), _femL->x().x(), _femL->x().y(), _femL->x().z(), _time );
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      (*_fL)(icomp,ii) -= weight*bnm*( _udir[icomp]*_femL->phi(ii) );
    }
  }
}

void TransportIntegrator::formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  double w = 0.5*weight;
  Alat::Node beta;
  getData("beta")->getValue(beta, _femL->x());
  double bn = arma::dot(beta, _femL->getNormal());
  double bnm = fmin(0.0, bn);
  double bnp = fmax(0.0, bn);
  if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  {
    assert(bn >= 0.0);
    if(_symmetrytype == "skew")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          (*_fL)(icomp,ii) += w* bn* (*_UL)(icomp) *_femL->phi(ii);
        }
      }
    }
    else if(_symmetrytype == "dual")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          (*_fL)(icomp,ii) += weight* bn* (*_UL)(icomp) *_femL->phi(ii);
        }
      }
    }
  }
  if( _boundarymanager->boundaryConditionIs("Dirichlet", color) )
  {
    if(_symmetrytype == "skew")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          (*_fL)(icomp,ii) += w* fabs(bn)* (*_UL)(icomp) *_femL->phi(ii);
        }
      }
    }
    else if(_symmetrytype == "primal")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          (*_fL)(icomp,ii) -= weight* bnm* (*_UL)(icomp) *_femL->phi(ii);
        }
      }
    }
    else if(_symmetrytype == "dual")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          (*_fL)(icomp,ii) += weight* bnp* (*_UL)(icomp) *_femL->phi(ii);
        }
      }
    }
  }
}

void TransportIntegrator::matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  double w = 0.5*weight;
  Alat::Node beta;
  getData("beta")->getValue(beta, _femL->x());
  double bn = arma::dot(beta, _femL->getNormal());
  double bnm = fmin(0.0, bn);
  double bnp = fmax(0.0, bn);
  if( _boundarymanager->boundaryConditionIs("Neumann", color) )
  {
    if(_symmetrytype == "skew")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          for(int jj = 0; jj < _nlocal; jj++)
          {
            (*_ALL) (icomp, icomp, ii, jj) += w* bn*_femL->phi(ii)* _femL->phi(jj);
          }
        }
      }
    }
    else if(_symmetrytype == "dual")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          for(int jj = 0; jj < _nlocal; jj++)
          {
            (*_ALL) (icomp, icomp, ii, jj) += weight* bn*_femL->phi(ii)* _femL->phi(jj);
          }
        }
      }
    }
  }
  if( _boundarymanager->boundaryConditionIs("Dirichlet", color) )
    // else if( femd._boundaryconditions[_varname] == "Dirichlet" )
  {
    if(_symmetrytype == "skew")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          for(int jj = 0; jj < _nlocal; jj++)
          {
            (*_ALL) (icomp, icomp, ii, jj) += w* fabs(bn)*_femL->phi(ii)* _femL->phi(jj);
          }
        }
      }
    }
    else if(_symmetrytype == "primal")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          for(int jj = 0; jj < _nlocal; jj++)
          {
            (*_ALL) (icomp, icomp, ii, jj) -= weight* bnm*_femL->phi(ii)* _femL->phi(jj);
          }
        }
      }
    }
    else if(_symmetrytype == "dual")
    {
      for(int icomp = 0; icomp < _ncompout; icomp++)
      {
        for(int ii = 0; ii < _nlocal; ii++)
        {
          for(int jj = 0; jj < _nlocal; jj++)
          {
            (*_ALL) (icomp, icomp, ii, jj) += weight* bnp*_femL->phi(ii)* _femL->phi(jj);
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void TransportIntegrator::formCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  double w = 0.5*weight;
  Alat::Node beta;
  getData("beta")->getValue(beta, _femL->x());
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      if(_symmetrytype == "skew")
      {
        (*_fL)(icomp,ii) += w*(arma::dot(beta, (*_UL).d(icomp))*_femL->phi(ii) - arma::dot(beta, _femL->dphi(ii))*(*_UL)(icomp) );
      }
      else if(_symmetrytype == "primal")
      {
        (*_fL)(icomp,ii) += weight* arma::dot( beta, (*_UL).d(icomp))*_femL->phi(ii);
      }
      else if(_symmetrytype == "dual")
      {
        (*_fL)(icomp,ii) -= weight*arma::dot(beta, _femL->dphi(ii))*(*_UL)(icomp);
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void TransportIntegrator::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  double w = 0.5*weight;
  Alat::Node beta;
  getData("beta")->getValue(beta, _femL->x());
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      for(int jj = 0; jj < _nlocal; jj++)
      {
        if(_symmetrytype == "skew")
        {
          (*_ALL) (icomp, icomp, ii, jj) += w*( _femL->phi(ii)*arma::dot(beta, _femL->dphi(jj)) );
          (*_ALL) (icomp, icomp, ii, jj) -= w*arma::dot(beta, _femL->dphi(ii))*( _femL->phi(jj) );
        }
        else if(_symmetrytype == "primal")
        {
          (*_ALL) (icomp, icomp, ii, jj) += weight*( _femL->phi(ii)*arma::dot(beta, _femL->dphi(jj)) );
        }
        else if(_symmetrytype == "dual")
        {
          (*_ALL) (icomp, icomp, ii, jj) -= weight*arma::dot(beta, _femL->dphi(ii))*( _femL->phi(jj) );
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void TransportIntegrator::formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  double w = weight;
  Alat::Node beta;
  getModel()->getData("beta")->getValue(beta, _femL->x());
  // std::cerr << "beta="<<beta<<" ";
  // assert(0);
  double bn = arma::dot(beta, _femL->getNormal());
  double bnm = fmin(0.0, bn);
  double bnp = bn-bnm;
  double bnabs = bnp-bnm;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      if(_symmetrytype == "skew")
      {
        (*_fL)(icomp,ii) += w*bnm*_femL->phi(ii)*(*_UR)(icomp);
        (*_fR)(icomp,ii) -= w*bnp*_femR->phi(ii)*(*_UL)(icomp);

        (*_fL)(icomp,ii) += w*0.5* bnabs* _femL->phi(ii)*(*_UL)(icomp);
        (*_fR)(icomp,ii) += w*0.5* bnabs* _femR->phi(ii)*(*_UR)(icomp);
      }
      else if(_symmetrytype == "primal")
      {
        (*_fL)(icomp,ii) -= w*bnm*_femL->phi(ii)*(*_UL)(icomp);
        (*_fL)(icomp,ii) += w*bnm*_femL->phi(ii)*(*_UR)(icomp);
        (*_fR)(icomp,ii) -= w*bnp*_femR->phi(ii)*(*_UL)(icomp);
        (*_fR)(icomp,ii) += w*bnp*_femR->phi(ii)*(*_UR)(icomp);
      }
      else if(_symmetrytype == "dual")
      {
        (*_fL)(icomp,ii) += w*bnp*_femL->phi(ii)*(*_UL)(icomp);
        (*_fR)(icomp,ii) -= w*bnp*_femR->phi(ii)*(*_UL)(icomp);
        (*_fL)(icomp,ii) += w*bnm*_femL->phi(ii)*(*_UR)(icomp);
        (*_fR)(icomp,ii) -= w*bnm*_femR->phi(ii)*(*_UR)(icomp);
      }
    }
  }
}

/*--------------------------------------------------------------------------*/

void TransportIntegrator::matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const
{
  double w = weight;
  Alat::Node beta;
  getModel()->getData("beta")->getValue(beta, _femL->x());
  double bn = arma::dot(beta, _femL->getNormal());
  double bnm = fmin(0.0, bn);
  double bnp = bn-bnm;
  double bnabs = bnp-bnm;
  for(int icomp = 0; icomp < _ncompout; icomp++)
  {
    for(int ii = 0; ii < _nlocal; ii++)
    {
      for(int jj = 0; jj < _nlocal; jj++)
      {
        if(_symmetrytype == "skew")
        {
          (*_ALR) (icomp, icomp, ii, jj) += w*bnm*_femL->phi(ii)*_femR->phi(jj);
          (*_ARL) (icomp, icomp, ii, jj) -= w*bnp*_femR->phi(ii)*_femL->phi(jj);

          // upwind
          (*_ALL) (icomp, icomp, ii, jj) += w*0.5* bnabs* _femL->phi(ii)*_femL->phi(jj);
          (*_ARR) (icomp, icomp, ii, jj) += w*0.5* bnabs* _femR->phi(ii)*_femR->phi(jj);
        }
        else if(_symmetrytype == "primal")
        {
          (*_ALL) (icomp, icomp, ii, jj) -= w*bnm*_femL->phi(ii)*_femL->phi(jj);
          (*_ALR) (icomp, icomp, ii, jj) += w*bnm*_femL->phi(ii)*_femR->phi(jj);
          (*_ARL) (icomp, icomp, ii, jj) -= w*bnp*_femR->phi(ii)*_femL->phi(jj);
          (*_ARR) (icomp, icomp, ii, jj) += w*bnp*_femR->phi(ii)*_femR->phi(jj);
        }
        else if(_symmetrytype == "dual")
        {
          (*_ALL) (icomp, icomp, ii, jj) += w*bnp*_femL->phi(ii)*_femL->phi(jj);
          (*_ARL) (icomp, icomp, ii, jj) -= w*bnp*_femR->phi(ii)*_femL->phi(jj);
          (*_ALR) (icomp, icomp, ii, jj) += w*bnm*_femL->phi(ii)*_femR->phi(jj);
          (*_ARR) (icomp, icomp, ii, jj) -= w*bnm*_femR->phi(ii)*_femR->phi(jj);
        }
      }
    }
  }
}
