#include  "Alat/doublevector.h"
#include  "Alat/preconditionerwithsorting.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/iluvariablematrix.h"
#include  "Alat/variablematrix.h"
#include  <cassert>
#include  <cmath>
#include  "Alat/armadillo.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
IluVariableMatrix::~IluVariableMatrix(){}
IluVariableMatrix::IluVariableMatrix(int ncomp, const Alat::PreconditionerWithSorting* preconditionerwithsorting, std::string type) : VariableMatrix(ncomp, ncomp, type), _preconditionerwithsorting(preconditionerwithsorting), _variablematrix(NULL){}
IluVariableMatrix::IluVariableMatrix( const IluVariableMatrix& variablematrix) : VariableMatrix(variablematrix)
{
  assert(0);
}
IluVariableMatrix& IluVariableMatrix::operator=( const IluVariableMatrix& variablematrix)
{
  VariableMatrix::operator=(variablematrix);
  assert(0);
  return *this;
}
std::string IluVariableMatrix::getName() const
{
  if(_type == "simple")
  {
    return "IluVariableMatrix_Simple";
  }
  else if(_type == "diagonal")
  {
    return "IluVariableMatrix_Diagonal";
  }
  return "IluVariableMatrix_Full";
}

IluVariableMatrix* IluVariableMatrix::clone() const
{
  assert(0);
//return new IluVariableMatrix(*this);
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::set_size(int n, int ntotal)
{
  VariableMatrix::set_size(n, ntotal);
  if( _type == "full" )
  {
    _hfull.set_size(_ncomp);
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _hfull[icomp].set_size(n);
    }
  }
  else
  {
    _h.set_size(n);
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::computeIlu(double relax, const Alat::VariableMatrixInterface* variablematrixinterface, const Alat::IntVector& p, const Alat::IntVector& pinv)
{
  _variablematrix = dynamic_cast<const Alat::VariableMatrix*>( variablematrixinterface );
  assert(_variablematrix);
  const Alat::SparsityPattern* sparsitypattern = _variablematrix->getSparsityPattern();
  std::string type = _variablematrix->getType();
  if( _type == "full" )
  {
    assert(type == "full");
  }
  else if( _type == "diagonal" )
  {
    assert(type == "full" or type == "diagonal");
  }
  int ncomp = _variablematrix->getNComponents();
  assert(_ncomp == ncomp);
  int enlarge_stencil = 0;
  _sparsitypattern.reconstructWithNumbering( sparsitypattern, p, pinv, enlarge_stencil );
  _sparsitypattern.setDiagonal( _diag );
  _copy_entries(p, pinv);
  _modify(relax);

  if(_type == "simple")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      for(int pk = _sparsitypattern.rowstart(i); pk < _diag[i]; pk++)
      {
        int k = _sparsitypattern.col(pk);
        _simplematrix[pk] *=  _simplematrix[_diag[k]];
        for(int pj = _diag[k]+1; pj < _sparsitypattern.rowstop(k); pj++)
        {
          int j  = _sparsitypattern.col(pj);
          for(int ph = _sparsitypattern.rowstart(i); ph < _sparsitypattern.rowstop(i); ph++)
          {
            if(_sparsitypattern.col(ph) == j)
            {
              _simplematrix[ph] -= _simplematrix[pk]*_simplematrix[pj];
              break;
            }
          }
        }
      }
      double d = _simplematrix[_diag[i]];
      _simplematrix[_diag[i]] = 1.0/d;
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      for(int pk = _sparsitypattern.rowstart(i); pk < _diag[i]; pk++)
      {
        int k = _sparsitypattern.col(pk);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          _diagonalmatrix[icomp][pk] *=  _diagonalmatrix[icomp][_diag[k]];
        }
        for(int pj = _diag[k]+1; pj < _sparsitypattern.rowstop(k); pj++)
        {
          int j  = _sparsitypattern.col(pj);
          for(int ph = _sparsitypattern.rowstart(i); ph < _sparsitypattern.rowstop(i); ph++)
          {
            if(_sparsitypattern.col(ph) == j)
            {
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                _diagonalmatrix[icomp][ph] -= _diagonalmatrix[icomp][pk]*_diagonalmatrix[icomp][pj];
              }
              break;
            }
          }
        }
      }
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        double d = _diagonalmatrix[icomp][_diag[i]];
        _diagonalmatrix[icomp][_diag[i]] = 1.0/d;
      }
    }
  }
  if(_type == "full")
  {
    arma::mat H(_ncomp, _ncomp), D(_ncomp, _ncomp);
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      for(int pk = _sparsitypattern.rowstart(i); pk < _diag[i]; pk++)
      {
        int k = _sparsitypattern.col(pk);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < _ncomp; jcomp++)
          {
            H(icomp, jcomp) = _fullmatrix(icomp, jcomp)[pk];
            D(icomp, jcomp) = _fullmatrix(icomp, jcomp)[_diag[k]];
            _fullmatrix(icomp, jcomp)[pk] = 0.0;
          }
        }
        assert(pk != _diag[k]);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < _ncomp; jcomp++)
          {
            for(int kcomp = 0; kcomp < _ncomp; kcomp++)
            {
              _fullmatrix(icomp, jcomp)[pk] += H(icomp, kcomp)*D(kcomp, jcomp);
            }
          }
        }
        for(int pj = _diag[k]+1; pj < _sparsitypattern.rowstop(k); pj++)
        {
          int j  = _sparsitypattern.col(pj);
          for(int ph = _sparsitypattern.rowstart(i); ph < _sparsitypattern.rowstop(i); ph++)
          {
            if(_sparsitypattern.col(ph) == j)
            {
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                for(int jcomp = 0; jcomp < _ncomp; jcomp++)
                {
                  for(int kcomp = 0; kcomp < _ncomp; kcomp++)
                  {
                    _fullmatrix(icomp, jcomp)[ph] -= _fullmatrix(icomp, kcomp)[pk]*_fullmatrix(kcomp, jcomp)[pj];
                  }
                }
              }
              break;
            }
          }
        }
      }
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _ncomp; jcomp++)
        {
          H(icomp, jcomp) = _fullmatrix(icomp, jcomp)[_diag[i]];
        }
      }
      H = arma::inv(H);
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _ncomp; jcomp++)
        {
          _fullmatrix(icomp, jcomp)[_diag[i]] = H ( icomp, jcomp);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::ilu(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const
{
  const Alat::VariableVector* vf = dynamic_cast<const Alat::VariableVector*>( f );
  assert(vf);
  Alat::VariableVector* vu = dynamic_cast<Alat::VariableVector*>( u );
  assert(vu);
	// vu->zeros();
  if(_type != "full")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _preconditionerwithsorting->permute_from(icomp, _h, *vf);
      _forward(icomp, _h);
      _backward(icomp, _h);
      _preconditionerwithsorting->permute_to(icomp, *vu, _h);
    }
  }
  else
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _preconditionerwithsorting->permute_from(icomp, _hfull[icomp], *vf);
    }
    _forwardfull(_hfull);
    _backwardfull(_hfull);
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _preconditionerwithsorting->permute_to(icomp, *vu, _hfull[icomp]);
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_forward(int icomp, Alat::DoubleVector& x) const
{
  if(_type == "simple")
  {
    for(int i = 1; i < _sparsitypattern.n(); i++)
    {
      int ende = _diag[i];
      for(int pos = _sparsitypattern.rowstart(i); pos < ende; pos++)
      {
        int j = _sparsitypattern.col(pos);
        x[i] -= _simplematrix[pos]*x[j];
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = 1; i < _sparsitypattern.n(); i++)
    {
      int ende = _diag[i];
      for(int pos = _sparsitypattern.rowstart(i); pos < ende; pos++)
      {
        int j = _sparsitypattern.col(pos);
        x[i] -= _diagonalmatrix[icomp][pos]*x[j];
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_forwardfull(Alat::Vector<Alat::DoubleVector>& x) const
{
  assert(_type == "full");
  for(int i = 1; i < _sparsitypattern.n(); i++)
  {
    int ende = _diag[i];
    for(int pos = _sparsitypattern.rowstart(i); pos < ende; pos++)
    {
      int j = _sparsitypattern.col(pos);
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _ncomp; jcomp++)
        {
          x[icomp][i] -= _fullmatrix(icomp, jcomp)[pos]*x[jcomp][j];
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_backward(int icomp, Alat::DoubleVector& x) const
{
  if(_type == "simple")
  {
    for(int i = _sparsitypattern.n()-1; i >= 0; i--)
    {
      int ende = _diag[i];
      for(int pos = _sparsitypattern.rowstop(i)-1; pos > ende; pos--)
      {
        int j = _sparsitypattern.col(pos);
        x[i] -= _simplematrix[pos]*x[j];
      }
      x[i] *= _simplematrix[ende];
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = _sparsitypattern.n()-1; i >= 0; i--)
    {
      int ende = _diag[i];
      for(int pos = _sparsitypattern.rowstop(i)-1; pos > ende; pos--)
      {
        int j = _sparsitypattern.col(pos);
        x[i] -= _diagonalmatrix[icomp][pos]*x[j];
      }
      x[i] *= _diagonalmatrix[icomp][ende];
    }
  }
  else
  {
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_backwardfull(Alat::Vector<Alat::DoubleVector>& x) const
{
  assert(_type == "full");
  arma::vec help(_ncomp);
  for(int i = _sparsitypattern.n()-1; i >= 0; i--)
  {
    int ende = _diag[i];
    for(int pos = _sparsitypattern.rowstop(i)-1; pos > ende; pos--)
    {
      int j = _sparsitypattern.col(pos);
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _ncomp; jcomp++)
        {
          x[icomp][i] -= _fullmatrix(icomp, jcomp)[pos]*x[jcomp][j];
        }
      }
    }
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      help[icomp] = x[icomp][i];
      x[icomp][i] = 0.0;
    }
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _ncomp; jcomp++)
      {
        x[icomp][i] += _fullmatrix(icomp, jcomp)[ende]*help[jcomp];
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_modify(double ilum)
{
  if(_type == "simple")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      double sum = 0.0;
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        if( pos != _diag[i] )
        {
          sum += fabs(_simplematrix[pos]);
        }
      }
      double test = fmax(_simplematrix[ _diag[i] ], 0.0);
      double d =  fmax( 0.0, sum - test );
      _simplematrix[ _diag[i] ] += ilum*d;
    }
  }
  else if(_type == "diagonal")
  {
    Alat::DoubleVector sum(_ncomp);
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      sum.zeros();
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        if( pos != _diag[i] )
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            sum[icomp] += fabs(_diagonalmatrix[icomp][pos]);
          }
        }
      }
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        double test = fmax(_diagonalmatrix[icomp][ _diag[i] ], 0.0);
        double d =  fmax( 0.0, sum[icomp] - test );
        _diagonalmatrix[icomp][ _diag[i] ] += ilum*d;
      }
    }
  }
  else
  {
    Alat::DoubleVector sum(_ncomp);
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      sum.zeros();
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        if( pos != _diag[i] )
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < _ncomp; jcomp++)
            {
              sum[icomp] += fabs(_fullmatrix(icomp, jcomp)[pos]);
            }
          }
        }
      }
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        double test = fmax(_fullmatrix(icomp, icomp)[_diag[i]], 0.0);
        double d =  fmax( 0.0, sum[icomp] - test );
        _fullmatrix(icomp, icomp)[ _diag[i] ] += ilum*d;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void IluVariableMatrix::_copy_entries(const Alat::IntVector& p, const Alat::IntVector& pinv)
{
  zeros();
  const Alat::SparsityPattern& sparsitypatternA = *_variablematrix->getSparsityPattern();
  if(_type == "simple")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      int pi = p[i];
      for(int posA = sparsitypatternA.rowstart(pi); posA < sparsitypatternA.rowstop(pi); posA++)
      {
        int j   = sparsitypatternA.col(posA);
        int qj  = pinv[j];
        bool found = 0;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int k = _sparsitypattern.col(pos);
          if(k == qj)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _simplematrix[pos] +=  _variablematrix->getValue(icomp, icomp, posA)/(double) _ncomp;
            }
            found = 1;
            break;
          }
        }
        if(!found)
        {
          std::cerr<<"*** IluBlockmatrix::_copy_entries(): entry not found "<<qj<<std::endl<<"in row:";
          for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
          {
            std::cerr<<_sparsitypattern.col(pos)<<" ";
          }
          std::cerr<<std::endl;
          assert(0);
          exit(1);
        }
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      int pi = p[i];
      for(int posA = sparsitypatternA.rowstart(pi); posA < sparsitypatternA.rowstop(pi); posA++)
      {
        int j   = sparsitypatternA.col(posA);
        int qj  = pinv[j];
        bool found = 0;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int k = _sparsitypattern.col(pos);
          if(k == qj)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _diagonalmatrix[icomp][pos] +=  _variablematrix->getValue(icomp, icomp, posA);
            }
            found = 1;
            break;
          }
        }
        if(!found)
        {
          std::cerr<<"*** IluBlockmatrix::_copy_entries(): entry not found "<<qj<<std::endl<<"in row:";
          for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
          {
            std::cerr<<_sparsitypattern.col(pos)<<" ";
          }
          std::cerr<<std::endl;
          assert(0);
          exit(1);
        }
      }
    }
  }
  else if(_type == "full")
  {
    for(int i = 0; i < _sparsitypattern.n(); i++)
    {
      int pi = p[i];
      for(int posA = sparsitypatternA.rowstart(pi); posA < sparsitypatternA.rowstop(pi); posA++)
      {
        int j   = sparsitypatternA.col(posA);
        int qj  = pinv[j];
        bool found = 0;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int k = _sparsitypattern.col(pos);
          if(k == qj)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              for(int jcomp = 0; jcomp < _ncomp; jcomp++)
              {
                _fullmatrix(icomp, jcomp)[pos] +=  _variablematrix->getValue(icomp, jcomp, posA);
              }
            }
            found = 1;
            break;
          }
        }
        if(!found)
        {
          std::cerr<<"*** IluBlockmatrix::_copy_entries(): entry not found "<<qj<<std::endl<<"in row:";
          for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
          {
            std::cerr<<_sparsitypattern.col(pos)<<" ";
          }
          std::cerr<<std::endl;
          assert(0);
          exit(1);
        }
      }
    }
  }
}
