#include  "Alat/filescanner.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Fada/domaindecompositionsmoother.h"
#include  <algorithm>
#include  <cassert>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif

using namespace Fada;

class CompareForPermute
{
private:
  int _start;
  const Alat::IntVector& _vector;
public:
  CompareForPermute(const Alat::IntVector& vector, int start) : _vector(vector), _start(start) {}
  bool operator()(int i, int j) const
  {
    return _vector[_start+i] < _vector[_start+j];
  }
};

/*--------------------------------------------------------------------------*/

DomainDecompositionSmoother::~DomainDecompositionSmoother()
{}

DomainDecompositionSmoother::DomainDecompositionSmoother(const Alat::VariableMatrixInterface* matrix, int patchsize) : PreconditionerWithSorting(matrix), _patchsize(patchsize)
{}

DomainDecompositionSmoother::DomainDecompositionSmoother( const DomainDecompositionSmoother& domaindecompositionsmoother) : PreconditionerWithSorting(domaindecompositionsmoother)
{
  assert(0);
}

DomainDecompositionSmoother& DomainDecompositionSmoother::operator=( const DomainDecompositionSmoother& domaindecompositionsmoother)
{
  PreconditionerWithSorting::operator=(domaindecompositionsmoother);
  assert(0);
  return *this;
}

std::string DomainDecompositionSmoother::getName() const
{
  return "DomainDecompositionSmoother";
}

DomainDecompositionSmoother* DomainDecompositionSmoother::clone() const
{
  assert(0);
//return new DomainDecompositionSmoother(*this);
}

/*--------------------------------------------------------------------------*/

void DomainDecompositionSmoother::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  PreconditionerWithSorting::basicInit(parameterfile, blockname, visitor);
}

/*--------------------------------------------------------------------------*/

void DomainDecompositionSmoother::reInit()
{
  PreconditionerWithSorting::reInit();
  sortIndices();
  const Alat::VariableMatrixInterface* variablematrix = dynamic_cast<const Alat::VariableMatrixInterface*>( _matrix );
  assert(variablematrix);
  _ncomp = variablematrix->getNComponents();
  assert( _ncomp == variablematrix->getMComponents() );
  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  int nall = sparsitypattern->n();
  int npatches = nall/_patchsize;
  if(npatches == 0)
  {
    npatches = 1;
  }
  _patchsize = nall/npatches;
  int nrest = nall - npatches*_patchsize;
  _patchsizes.set_size(npatches);
  for(int i = 0; i < nrest; i++)
  {
    _patchsizes[i] = _patchsize+1;
  }
  for(int i = nrest; i < npatches; i++)
  {
    _patchsizes[i] = _patchsize;
  }
  std::cerr << "nall _patchsize npatches " << nall << " " << _patchsize << " " << npatches  << "\n";

  int patchsizep1 = _patchsize+1;
  int firstsize = patchsizep1*nrest;

  // determine sizes
  Alat::Vector<Alat::IntVector > rowsizes(npatches);
  int irowmatrix = 0;
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    int nrows = _patchsizes[ipatch];
    rowsizes[ipatch].set_size(nrows);
    for(int il = 0; il < nrows; il++)
    {
      rowsizes[ipatch][il] = 0;
      for(int pos = sparsitypattern->rowstart(_p[irowmatrix]); pos < sparsitypattern->rowstop(_p[irowmatrix]); pos++)
      {
        int j = _pinv[sparsitypattern->col(pos)];
        int jpatch, jl;
        if(j < firstsize)
        {
          jpatch = j /patchsizep1;
          jl = j % patchsizep1;
        }
        else
        {
          jpatch = nrest+( j-firstsize )/_patchsize;
          jl = ( j-firstsize )%_patchsize;
        }
        if(ipatch == jpatch)
        {
          rowsizes[ipatch][il]++;
        }
      }
      irowmatrix++;
    }
  }
  // fill _sparsitypatternvector
  _umf.set_size(npatches);
  Alat::Vector<Alat::SparsityPattern*> _sparsitypatternvector;
  _sparsitypatternvector.set_size(npatches);
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    _sparsitypatternvector[ipatch] = _umf[ipatch].getSparseMatrix().getSparsityPattern();
  }
  _newpos.set_size(npatches);
  irowmatrix = 0;
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    Alat::IntVector& rowstart = _sparsitypatternvector[ipatch]->rowstart();
    Alat::IntVector& col = _sparsitypatternvector[ipatch]->col();
    int nrows = _patchsizes[ipatch];
    int ntotalnew = 0;
    for(int irow = 0; irow < nrows; irow++)
    {
      ntotalnew += rowsizes[ipatch][irow];
    }
    _newpos[ipatch].set_size(_ncomp*_ncomp*ntotalnew);
    _sparsitypatternvector[ipatch]->col().set_size(_ncomp*_ncomp*ntotalnew);
    _sparsitypatternvector[ipatch]->rowstart().set_size(_ncomp*nrows+1);

    // _sparsitypatternvector[ipatch]->rowstart(0) = 0;
    rowstart[0] = 0;
    for(int irow = 0; irow < nrows; irow++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = _ncomp*irow + icomp;
        rowstart[index+1] = rowstart[index] + _ncomp*rowsizes[ipatch][irow];
        // _sparsitypatternvector[ipatch]->rowstart(index+1) = _sparsitypatternvector[ipatch]->rowstart(index) + _ncomp*rowsizes[ipatch][irow];
      }
    }
    int count = 0;
    for(int il = 0; il < nrows; il++)
    {
      // Alat::IntVector rowinmatrix, posinmatrix;
      Alat::IntSet rowinmatrix, posinmatrix;
      for(int pos = sparsitypattern->rowstart(_p[irowmatrix]); pos < sparsitypattern->rowstop(_p[irowmatrix]); pos++)
      {
        int j = _pinv[sparsitypattern->col(pos)];
        int jpatch, jl;
        if(j < firstsize)
        {
          jpatch = j /patchsizep1;
          jl = j % patchsizep1;
        }
        else
        {
          jpatch = nrest+( j-firstsize )/_patchsize;
          jl = ( j-firstsize )%_patchsize;
        }
        if(ipatch == jpatch)
        {
          // ?? je sais pas suite changement IntVector
          // rowinmatrix.push_back(jl);
          // posinmatrix.push_back(pos);
          rowinmatrix.insert(jl);
          posinmatrix.insert(pos);
        }
      }
      // for(int ii = 0; ii < rowinmatrix.size(); ii++)
      Alat::IntSet::const_iterator q=posinmatrix.begin();
      for(Alat::IntSet::const_iterator p=rowinmatrix.begin();p!=rowinmatrix.end();p++)
      {
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < _ncomp; jcomp++)
          {
            _newpos[ipatch][count][0] = icomp;
            _newpos[ipatch][count][1] = jcomp;
            _newpos[ipatch][count][2] =*p;
            // _newpos[ipatch][count][2] = posinmatrix[ii];
            // _sparsitypatternvector[ipatch]->col(count++) = _ncomp*rowinmatrix[ii]+jcomp;
            // col[count++] = _ncomp*rowinmatrix[ii]+jcomp;
            col[count++] = _ncomp* *q+jcomp;
          }
        }
        q++;
      }
      irowmatrix++;
    }
  }

// trier les lignes pour umf
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    Alat::IntVector& col = _sparsitypatternvector[ipatch]->col();
    int nrows = _sparsitypatternvector[ipatch]->n();
    for(int irow = 0; irow < nrows; irow++)
    {
      int rowsize = _sparsitypatternvector[ipatch]->rowsize(irow);
      int start = _sparsitypatternvector[ipatch]->rowstart(irow);
      Alat::IntVector permute( rowsize ), help(rowsize);
      std::iota(permute.begin(), permute.end(), 0);
      CompareForPermute compareforpermute( _sparsitypatternvector[ipatch]->col(), start );
      std::sort(permute.begin(), permute.end(), compareforpermute);
      for(int ii = 0; ii < rowsize; ii++)
      {
        help[ii] = _sparsitypatternvector[ipatch]->col(start+ii);
      }
      for(int ii = 0; ii < rowsize; ii++)
      {
        // _sparsitypatternvector[ipatch]->col(start+ii) = help[permute[ii]];
        col[start+ii] = help[permute[ii]];
      }
      for(int iinfo = 0; iinfo < 3; iinfo++)
      {
        for(int ii = 0; ii < rowsize; ii++)
        {
          help[ii] = _newpos[ipatch][start+ii][iinfo];
        }
        for(int ii = 0; ii < rowsize; ii++)
        {
          _newpos[ipatch][start+ii][iinfo] = help[permute[ii]];
        }
      }
    }
  }


  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    _umf[ipatch].getSparseMatrix().getValue().set_size( _umf[ipatch].getSparseMatrix().getSparsityPattern()->ntotal() );
    _umf[ipatch].reInit();
  }

  _u.set_size(_ncomp*nall);
  _f.set_size(_ncomp*nall);
  _upatch.set_size(npatches);
  _fpatch.set_size(npatches);
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    int n = _sparsitypatternvector[ipatch]->n();
    _upatch[ipatch].set_size(n);
    _fpatch[ipatch].set_size(n);
  }
}

/*--------------------------------------------------------------------------*/

void DomainDecompositionSmoother::computePreconditioner()
{
  const Alat::VariableMatrixInterface* matrix = dynamic_cast<const Alat::VariableMatrixInterface*>( _matrix );
  assert(matrix);

  int npatches = _patchsizes.size();
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    Alat::DoubleVector& mat = _umf[ipatch].getSparseMatrix().getValue();
    for(int i = 0; i < mat.size(); i++)
    {
      int icomp = _newpos[ipatch][i][0];
      int jcomp = _newpos[ipatch][i][1];
      int posinA = _newpos[ipatch][i][2];
      mat[i] = matrix->getValue(icomp, jcomp, posinA);
    }
  }
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    _umf[ipatch].computeLu();
  }
}

/*--------------------------------------------------------------------------*/

void DomainDecompositionSmoother::_solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const
{
  const Alat::VariableMatrixInterface* variablematrix = dynamic_cast<const Alat::VariableMatrixInterface*>( _matrix );
  assert(variablematrix);

  const Alat::VariableVector* fvar = dynamic_cast<const Alat::VariableVector*>( f );
  Alat::VariableVector* uvar = dynamic_cast<Alat::VariableVector*>( u );
  if(not fvar)
  {
    assert(not uvar);
    const Alat::SystemVector* fall = dynamic_cast<const Alat::SystemVector*>( f );
    assert(fall);
    assert(fall->getNVars() == 1);
    Alat::SystemVector* uall = dynamic_cast< Alat::SystemVector*>( u );
    assert(uall);
    fvar = dynamic_cast<const Alat::VariableVector*>( fall->getVector(0) );
    uvar = dynamic_cast< Alat::VariableVector*>( uall->getVector(0) );
  }

  assert(fvar);
  assert(uvar);
  assert( _ncomp == fvar->getNComponents() );
  assert( _ncomp == uvar->getNComponents() );
  int npatches = _patchsizes.size();
  uvar->zeros();

  for(int i = 0; i < fvar->n(); i++)
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      int index = _ncomp*_pinv[i] + icomp;
      _f[index] = ( *fvar )(icomp, i);
    }
  }
  int sum = 0;
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    int n = _patchsizes[ipatch];
    for(int i = 0; i < n; i++)
    {
      int index = sum+i;
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        _fpatch[ipatch][i*_ncomp+icomp] = _f[index*_ncomp+icomp];
      }
    }
    sum += n;
  }
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    _umf[ipatch].solve(_upatch[ipatch], _fpatch[ipatch]);
  }

  sum = 0;
  for(int ipatch = 0; ipatch < npatches; ipatch++)
  {
    int n = _patchsizes[ipatch];
    for(int i = 0; i < n; i++)
    {
      int index = sum+i;
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        _u[index*_ncomp+icomp] = _upatch[ipatch][i*_ncomp+icomp];
      }
    }
    sum += n;
  }
  for(int i = 0; i < uvar->n(); i++)
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      int index = _ncomp*_pinv[i] + icomp;
      ( *uvar )(icomp, i) = _u[index];
    }
  }
}
