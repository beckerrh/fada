#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/richardsonsystem.h"
#include  <cassert>
#include  <limits>
#include  <math.h>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

RichardsonSystem::~RichardsonSystem()
{}

RichardsonSystem::RichardsonSystem(const std::string& type, int nvectors, const std::string& solutiontype) : RichardsonRB(type, nvectors, solutiontype)
{}

RichardsonSystem::RichardsonSystem( const RichardsonSystem& richardsonoptimal) : RichardsonRB(richardsonoptimal)
{
  assert(0);
}

RichardsonSystem& RichardsonSystem::operator=( const RichardsonSystem& richardsonoptimal)
{
  RichardsonRB::operator=(richardsonoptimal);
  assert(0);
  return *this;
}

std::string RichardsonSystem::getName() const
{
  std::stringstream ss;
  ss<<"RichardsonSystem_"<< _type << "_" << _nvectors<<"_"<<_solutiontype;
  return ss.str();
}

RichardsonSystem* RichardsonSystem::clone() const
{
  assert(0);
//return new RichardsonSystem(*this);
}

/*--------------------------------------------------------------------------*/
void RichardsonSystem::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{         
  assert(0);
  // _nvars = getVisitor()->getNVars();
  _nshift = _nvars+1;

  RichardsonRB::basicInit(parameterfile, blockname);

  _scalarproduct.set_size(_nvars);
  _vectors.set_size(_nvars);
  _H.set_size(_nvectors*_nvars, _nvectors*_nvars);
  _b.set_size(_nvectors*_nvars);
  _x.set_size(_nvectors*_nvars);
}

/*--------------------------------------------------------------------------*/
int RichardsonSystem::getNVectors() const
{
  return _nshift*_nvectors + 2;
}

/*--------------------------------------------------------------------------*/
void RichardsonSystem::_computeSmallSystem(int index, int nmemory) const
{
  _H.resize(nmemory*_nvars, nmemory*_nvars);
  _b.resize(nmemory*_nvars);
  _x.resize(nmemory*_nvars);

  Alat::GhostVector& r = getMemory(_nshift*_nvectors);
  //-------------------------------------------------
  if(_solutiontype == "gal")
  //-------------------------------------------------
  {
    for(int i = 0; i < nmemory; i++)
    {
      for(int jvar = 0; jvar < _nvars; jvar++)
      {                     
        assert(0);
        // getVisitor()->scalarProduct( _scalarproduct, getAV(i,jvar), getV(index) );
        for(int ivar = 0; ivar < _nvars; ivar++)
        {
          _H(ivar+_nvars*index, jvar+_nvars*i) = _scalarproduct[ivar];
        }
      }
      if(index != i)
      {
        for(int jvar = 0; jvar < _nvars; jvar++)
        {
          assert(0);
          // getVisitor()->scalarProduct( _scalarproduct, getAV(index,jvar), getV(i) );
          for(int ivar = 0; ivar < _nvars; ivar++)
          {
            _H(ivar+_nvars*i, jvar+_nvars*index) = _scalarproduct[ivar];
          }
        }
      }
      assert(0);
      // getVisitor()->scalarProduct( _scalarproduct, r, getV(i) );
      for(int ivar = 0; ivar < _nvars; ivar++)
      {
        _b[ivar+_nvars*i] = _scalarproduct[ivar];
      }
    }
  }
  //-------------------------------------------------
  else if(_solutiontype == "ls")
  //-------------------------------------------------
  {
    for(int i = 0; i < nmemory; i++)
    {
      for(int ivar = 0; ivar < _nvars; ivar++)
      {
        for(int jvar = 0; jvar < _nvars; jvar++)
        {
          double d = getVisitor()->scalarProduct( getAV(i,jvar), getAV(index,ivar) );
          _H(ivar+_nvars*index, jvar+_nvars*i) = d;
          _H(jvar+_nvars*i, ivar+_nvars*index) = d;
        }
      }
      for(int ivar = 0; ivar < _nvars; ivar++)
      {
        _b[ivar+_nvars*i] = getVisitor()->scalarProduct( r, getAV(i,ivar) );
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void RichardsonSystem::_addvector(Alat::GhostVector& u, int nmemory) const
{
  for(int i = 0; i < nmemory; i++)
  {
    for(int ivar = 0; ivar < _nvars; ivar++)
    {
      assert(0);
      // _scalarproduct[ivar] = _x[ivar+_nvars*i];
    }                   
    assert(0);
    // getVisitor()->vectorAdd( u, _scalarproduct, getV(i) );
  }
}

/*--------------------------------------------------------------------------*/
void RichardsonSystem::_matrixVectorProduct(int index) const
{
  for(int ivar = 0; ivar < _nvars; ivar++)
  {
    _vectors[ivar] = &getAV(index,ivar);
    getVisitor()->vectorZero( *_vectors[ivar] );
  }                
  assert(0);
  // getVisitor()->matrixVectorProduct(*_ghostmatrix,  _vectors, getV(index), 1.0);
}
