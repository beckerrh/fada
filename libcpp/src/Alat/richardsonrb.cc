#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/richardsonrb.h"
#include  <cassert>
#include  <math.h>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
RichardsonRB::~RichardsonRB()
{}

RichardsonRB::RichardsonRB(const std::string& type, int nvectors, const std::string& solutiontype) : Alat::IterativeSolverWithPreconditioner(), _nvectors(nvectors), _nextupdate(0), _nextproduct(0), _nmemory(0), _nextmemory(0), _type(type), _solutiontype(solutiontype), _nvars(1), _nshift(2), _niterafterrestar(0)
{
  assert(_type == "cyc" or _type == "coef" or _type == "ortho" or _type == "restart");
  assert(_solutiontype == "ls" or _solutiontype == "gal" or _solutiontype == "gals");
  _H.set_size(_nvectors, _nvectors);
  _b.set_size(_nvectors);
  _x.set_size(_nvectors);
}

RichardsonRB::RichardsonRB( const RichardsonRB& richardsonoptimal) : IterativeSolverWithPreconditioner(richardsonoptimal)
{
  assert(0);
}

RichardsonRB& RichardsonRB::operator=( const RichardsonRB& richardsonoptimal)
{
  IterativeSolverWithPreconditioner::operator=(richardsonoptimal);
  assert(0);
  return *this;
}

std::string RichardsonRB::getName() const
{
  std::stringstream ss;
  ss<<"RichardsonRB_"<< _type << "_" << _nvectors<<"_"<<_solutiontype;
  return ss.str();
}

RichardsonRB* RichardsonRB::clone() const
{
  assert(0);
//return new RichardsonRB(*this);
}

Alat::GhostVector& RichardsonRB::getV(int i) const
{
  return getMemory(_nshift*i);
}

Alat::GhostVector& RichardsonRB::getAV(int i, int ivar) const
{
  return getMemory(_nshift*i+1+ivar);
}

/*--------------------------------------------------------------------------*/
void RichardsonRB::restart()
{
  _nmemory = 0;
  _nextmemory = 0;
}

/*--------------------------------------------------------------------------*/
void RichardsonRB::addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const
{
  // les impairs sont les produit avec la matrice des pairs !
  getVisitor()->vectorEqual( getV(_nextmemory), w );
  if(_type == "ortho")
  {
    for(int i = 0; i < _nmemory; i++)
    {
      if(i == _nextmemory)
      {
        continue;
      }
      double scal = getVisitor()->scalarProduct( getV(i), getV(_nextmemory) );
      getVisitor()->vectorAdd( getV(_nextmemory), -scal, getV(i) );
    }
  }
  double wnorm = getVisitor()->vectorNorm( w);
  if(wnorm)
  {
    getVisitor()->vectorScale( getV(_nextmemory), 1.0/wnorm );
  }
  _matrixVectorProduct(_nextmemory);

  int nmemory = _nmemory+1;
  _computeSmallSystem(_nextmemory, nmemory);
  _solveSmallSystem(status);
  if(status == AlatEnums::IterationStatusProblem)
  {
    return;
  }
  _addvector(u, nmemory);

  if(_nmemory < _nvectors-1)
  {
    _nextmemory++;
    _nmemory++;
  }
  else
  // memoire pleine !
  {
    if( ( _type == "cyc" )or ( _type == "ortho") )
    {
      if(_nextmemory == _nvectors-1)
      {
        _nextmemory = 0;
      }
      else
      {
        _nextmemory++;
      }
    }
    else if(_type == "coef")
    {
      arma::uword ind;
      arma::vec s = arma::abs(_x);
      s.min(ind);
      _nextmemory = ind;
    }
    else if(_type == "restart")
    {
      getVisitor()->vectorEqual( getV(0),  getV(_nextmemory) );
      getVisitor()->vectorEqual( getAV(0),  getAV(_nextmemory) );
      _nmemory = 0;
      _nextmemory = 0;
    }
    else
    {
      assert(0);
    }
  }
}

/*--------------------------------------------------------------------------*/
void RichardsonRB::_solveSmallSystem(AlatEnums::iterationstatus& status) const
{
  try
  {
    _x = arma::solve( _H, _b, arma::solve_opts::fast);
  }
  catch(std::exception& e)
  {
    std::cerr << "RichardsonRB::_solveSmallSystem(AlatEnums::iterationstatus& status) solve not ok\n";
    std::cerr << "_H="<<_H;
    _x = arma::pinv(_H)*_b;
//    status = AlatEnums::IterationStatusProblem;
//    return;
  }
  return;
}

/*--------------------------------------------------------------------------*/
void RichardsonRB::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _ghostmatrix = &A;
  const Alat::IterationInfo& info = *getIterationInfo();
  Alat::GhostVector& r = getMemory(_nshift*_nvectors);
  Alat::GhostVector& w = getMemory(_nshift*_nvectors+1);

  _conditionmean = 0.0;
  _conditionmax = 0.0;
  int iteration = 0;
  status = AlatEnums::IterationStatusRunning;
  while(1)
  {
    getVisitor()->residual(A, r, u, f);
    _rnorm = getVisitor()->vectorNorm(r);
    info.checkIteration(status, _rnorm, iteration);
    if( ( status == AlatEnums::IterationStatusConverged )or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
    {
      return;
    }
    iteration++;

    getVisitor()->vectorScale(r, 1./_rnorm);
    getVisitor()->vectorZero( w );
    getPreconditioner()->solveApproximate( _statuspreconditioner, A, w, r, iteration );
    addUpdate(status, w, u);
  }
}
