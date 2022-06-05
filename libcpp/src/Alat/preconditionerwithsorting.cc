#include  "Alat/doublevector.h"
#include  "Alat/fixarray.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/preconditionerwithsorting.h"
#include  "Alat/sorterinterface.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
PreconditionerWithSorting::~PreconditionerWithSorting()
{
  if(_sorter)
  {
    delete _sorter;
    _sorter = NULL;
  }
}
PreconditionerWithSorting::PreconditionerWithSorting(const Alat::VariableMatrixInterface* matrix) : Alat::PreconditionerWithVariableMatrix(matrix), _sorter(NULL){}
PreconditionerWithSorting::PreconditionerWithSorting( const PreconditionerWithSorting& preconditionerwithsorting) : Alat::PreconditionerWithVariableMatrix(preconditionerwithsorting)
{
  assert(0);
}
PreconditionerWithSorting& PreconditionerWithSorting::operator=( const PreconditionerWithSorting& preconditionerwithsorting)
{
  Alat::PreconditionerWithVariableMatrix::operator=(preconditionerwithsorting);
  assert(0);
  return *this;
}

std::string PreconditionerWithSorting::getName() const
{
  return "PreconditionerWithSorting";
}

PreconditionerWithSorting* PreconditionerWithSorting::clone() const
{
  assert(0);
//return new PreconditionerWithSorting(*this);
}

/*--------------------------------------------------------------------------*/
const Alat::IntVector& PreconditionerWithSorting::getP() const
{
  return _p;
}

const Alat::IntVector& PreconditionerWithSorting::getPinv() const
{
  return _pinv;
}

Alat::IntVector& PreconditionerWithSorting::getPinv()
{
  return _pinv;
}

Alat::SorterInterface*& PreconditionerWithSorting::newSorterPointer()
{
  return _sorter;
}

const Alat::SorterInterface* PreconditionerWithSorting::getSorter() const
{
  return _sorter;
}

/*--------------------------------------------------------------------------*/
void PreconditionerWithSorting::reInit()
{
  int n = _matrix->getN();
  _p.set_size(n);
  _pinv.set_size(n);
}

/*--------------------------------------------------------------------------*/

void PreconditionerWithSorting::sortIndices()
{
  if(_sorter == NULL)
  {
    for(int i = 0; i < _pinv.size(); i++)
    {
      _p[i] = i;
    }
  }
  else
  {
    _sorter->sort(_p, _matrix);
  }
  for(int i = 0; i < _pinv.size(); i++)
  {
    _pinv[_p[i]] = i;
  }
  for(int i = 0; i < _p.size(); i++)
  {
    if( _p[_pinv[i]] != i )
    {
      _error_string("sortIndices", "problem in sorting");
    }
  }
}

void PreconditionerWithSorting::permute_from(int icomp, Alat::DoubleVector& x, const Alat::VariableVector& y) const
{
  assert( _p.size() == y.n() );
  assert( _p.size() == x.size() );
  for(int i = 0; i < x.size(); i++)
  {
    // x[i] =  y(icomp, _p[i]);
    x[i] =  y(_p[i], icomp);
  }
}

void PreconditionerWithSorting::permute_to(int icomp, Alat::VariableVector& y, const Alat::DoubleVector& x) const
{
  assert( _p.size() == y.n() );
  assert( _p.size() == x.size() );
  for(int i = 0; i < y.n(); i++)
  {
    // y(icomp, i) =  x[_pinv[i]];
    y(i, icomp) =  x[_pinv[i]];
  }
}
