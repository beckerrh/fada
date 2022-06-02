#include  "Alat/systemassemblematrix.h"
#include  "Alat/sparsitypatternwithrowindex.h"
#include  "Alat/variablematrixwithrowindexinterface.h"
#include  "Alat/variablevector.h"
#include  "Alat/doublevector.h"
#include  "Alat/gaussseidelvariablediagonalmatrix.h"
#include  "Alat/iluvariablematrix.h"
#include  "Alat/spaivariablematrix.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/preconditionerwithsorting.h"
#include  "Alat/variablediagonalmatrix.h"
#include  "Alat/variablematrix.h"
#include  <cassert>
#include  "Alat/armadillo.h"
#include  "Alat/map.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableMatrix::~VariableMatrix()
{}

VariableMatrix::VariableMatrix(int ncomp, int mcomp, std::string type) : Alat::VariableMatrixInterface(), _type(type)
{
  _ncomp = ncomp;
  _mcomp = mcomp;
  if(type == "simple")
  {
    assert(ncomp == mcomp);
  }
  else if(type == "diagonal")
  {
    assert(ncomp == mcomp);
    _diagonalmatrix.set_size(ncomp);
  }
  else
  {
    if(type != "full")
		{
			std::cerr << "give type for VariableMatrix \'"<<type<<"\' is unknown\n";
			assert(0);
		}
    _fullmatrix.set_size(ncomp, mcomp);
  }
}

VariableMatrix::VariableMatrix( const VariableMatrix& variablematrix) : Alat::VariableMatrixInterface(variablematrix)
{
  assert(0);
}

VariableMatrix& VariableMatrix::operator=( const VariableMatrix& variablematrix)
{
  Alat::VariableMatrixInterface::operator=(variablematrix);
  assert(0);
  return *this;
}

std::string VariableMatrix::getName() const
{
  if(_type == "simple")
  {
    return "VariableMatrix_Simple";
  }
  else if(_type == "diagonal")
  {
    return "VariableMatrix_Diagonal";
  }
  return "VariableMatrix_Full";
}

VariableMatrix* VariableMatrix::clone() const
{
  assert(0);
//return new VariableMatrix(*this);
}

/*--------------------------------------------------------------------------*/
std::string VariableMatrix::getType() const
{
  return _type;
}

const Alat::DoubleVector& VariableMatrix::getFullMatrix(int icomp, int jcomp) const
{
  assert(_type == "full");
  return _fullmatrix(icomp, jcomp);
}

const Alat::DoubleVector& VariableMatrix::getDiagonalMatrix(int icomp) const
{
  assert(_type == "diagonal");
  return _diagonalmatrix[icomp];
}

const Alat::DoubleVector& VariableMatrix::getSimpleMatrix() const
{
  assert(_type == "simple");
  return _simplematrix;
}

Alat::DoubleVector& VariableMatrix::getFullMatrix(int icomp, int jcomp)
{
  assert(_type == "full");
  return _fullmatrix(icomp, jcomp);
}

Alat::DoubleVector& VariableMatrix::getDiagonalMatrix(int icomp)
{
  assert(_type == "diagonal");
  return _diagonalmatrix[icomp];
}

Alat::DoubleVector& VariableMatrix::getSimpleMatrix()
{
  assert(_type == "simple");
  return _simplematrix;
}

const Alat::Vector<Alat::DoubleVector>& VariableMatrix::getDiagonalMatrix() const
{
  assert(_type == "diagonal");
  return _diagonalmatrix;
}

const Alat::Matrix<Alat::DoubleVector>& VariableMatrix::getFullMatrix() const
{
  assert(_type == "full");
  return _fullmatrix;
}

int VariableMatrix::getN() const
{
  return _sparsitypattern.n();
}

int VariableMatrix::getNComponents() const
{
  return _ncomp;
}

int VariableMatrix::getMComponents() const
{
  return _mcomp;
}

const double& VariableMatrix::getValueSimple(int pos) const
{
  return _simplematrix[pos];
}

const double& VariableMatrix::getValue(int icomp, int jcomp, int pos) const
{
  if(_type == "simple")
  {
    return _simplematrix[pos];
  }
  else if(_type == "diagonal")
  {
    assert(icomp == jcomp);
    return _diagonalmatrix[icomp][pos];
  }
  return _fullmatrix(icomp, jcomp)[pos];
}

double& VariableMatrix::getValue(int icomp, int jcomp, int pos)
{
  if(_type == "simple")
  {
    return _simplematrix[pos];
  }
  else if(_type == "diagonal")
  {
    assert(icomp == jcomp);
    return _diagonalmatrix[icomp][pos];
  }
  return _fullmatrix(icomp, jcomp)[pos];
}

Alat::SparsityPattern* VariableMatrix::getSparsityPattern()
{
  return &_sparsitypattern;
}

const Alat::SparsityPattern* VariableMatrix::getSparsityPattern() const
{
  return &_sparsitypattern;
}

/*-------------------------------------------------------------*/
bool VariableMatrix::greaterThan(int pos1, int pos2) const
{
  if(_type == "simple")
  {
    return _simplematrix[pos1] > _simplematrix[pos2];
  }
  else if(_type == "diagonal")
  {
    double d1 = 0.0, d2 = 0.0;
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      d1 +=  _diagonalmatrix[icomp][pos1];
      d2 +=  _diagonalmatrix[icomp][pos2];
    }
    return d1 > d2;
  }
  double d1 = 0.0, d2 = 0.0;
  for(int icomp = 0; icomp < _ncomp; icomp++)
  {
    d1 +=  _fullmatrix(icomp, icomp)[pos1];
    d2 +=  _fullmatrix(icomp, icomp)[pos2];
  }
  return d1 > d2;
}

/*--------------------------------------------------------------------------*/
std::ostream& Alat::operator<<(std::ostream& os, const VariableMatrix& A)
{
  return A.write(os);
}

std::ostream& VariableMatrix::write(std::ostream& os) const
{
  int n = _sparsitypattern.n();
  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      int j = _sparsitypattern.col(pos);
      os << i << "," << j << " -> ";
      if(_type == "simple")
      {
        os << _simplematrix[pos] << " ";
      }
      else if(_type == "diagonal")
      {
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          os << _diagonalmatrix[icomp][pos] << " ";
        }
        os << "\n";
      }
      else
      {
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < _mcomp; jcomp++)
          {
            os << _fullmatrix(icomp, jcomp)[pos] << " ";
          }
        }
        os << "\n";
      }
    }
  }
  return os;
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::zeros()
{
  // std::cerr << "@@@@@@@@@@@@@@@@@@@@@@@@ VariableMatrix::zeros() _type="<<_type<<"\n";
  if(_type == "simple")
  {
    _simplematrix.zeros();
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _diagonalmatrix[icomp].zeros();
    }
  }
  else
  {
    for(Alat::Matrix<Alat::DoubleVector>::iterator p = _fullmatrix.begin(); p != _fullmatrix.end(); p++)
    {
      p->zeros();
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::set_size(int n, int ntotal)
{
  if(_type == "simple")
  {
    _simplematrix.set_size( ntotal );
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _diagonalmatrix[icomp].set_size(ntotal);
    }
  }
  else
  {
    for(Alat::Matrix<Alat::DoubleVector>::iterator p = _fullmatrix.begin(); p != _fullmatrix.end(); p++)
    {
      p->set_size( ntotal );
    }
  }
}

void VariableMatrix::set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft)
{
  _sparsitypattern.set_size(sparsitypatternsoft);
  this->set_size( _sparsitypattern.n(), sparsitypatternsoft.ntotal() );
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::addMatrix(const Alat::VariableMatrixInterface* sparsematrix, double d)
{
  // std::cerr << "VariableMatrix::addMatrix() d="<<d<<"\n";
  assert(_ncomp == sparsematrix->getNComponents());
  assert(_mcomp == sparsematrix->getMComponents());
  const VariableDiagonalMatrix* diagonalmatrix = dynamic_cast<const VariableDiagonalMatrix*>( sparsematrix );
  const VariableMatrix* variablematrix = dynamic_cast<const VariableMatrix*>( sparsematrix );
  if(diagonalmatrix)
  {
    _addMatrix(diagonalmatrix, d);
  }
  else if(variablematrix)
  {
    _addMatrix(variablematrix, d);
  }
  else
  {
    const Alat::VariableMatrixWithRowIndexInterface* sparsematrixwri = dynamic_cast<const Alat::VariableMatrixWithRowIndexInterface*>( sparsematrix );
    assert(sparsematrixwri);
    _addMatrix(sparsematrixwri, d);
  }
}

void VariableMatrix::_addMatrix(const Alat::VariableMatrixWithRowIndexInterface* sparsematrixwri, double d)
{
  assert(_type == "full");
  assert(d==1.0);
  assert(_ncomp==sparsematrixwri->getNComponents());
  assert(_mcomp==sparsematrixwri->getMComponents());
  const Alat::SparsityPattern* mysparsitypattern =  getSparsityPattern();
  const Alat::SparsityPatternWithRowIndex* sparsitypattern = sparsematrixwri->getSparsityPatternWithRowIndex();

  int n = sparsitypattern->n();
  for(int k = 0; k < n; k++)
  {
    int i = sparsitypattern->row(k);
    for(int pos = sparsitypattern->rowstart(k); pos < sparsitypattern->rowstop(k); pos++)
    {
      int j = sparsitypattern->col(pos);
      for(int mypos = mysparsitypattern->rowstart(i); mypos < mysparsitypattern->rowstop(i); mypos++)
      {
        int myj = mysparsitypattern->col(mypos);
        if(myj==j)
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
           {
             for(int jcomp = 0; jcomp < _mcomp; jcomp++)
             {
               _fullmatrix(icomp, jcomp)[mypos] += d *sparsematrixwri->getValue(icomp, jcomp, pos);
             }
           }
        }
      }
    }
  }
}

void VariableMatrix::_addMatrix(const VariableDiagonalMatrix* diagonalmatrix, double d)
{
  const Alat::SparsityPattern* SP =  getSparsityPattern();
  int n = SP->n();

  if(diagonalmatrix->getType() == "simple")
  {
    const Alat::DoubleVector& massmatrix = diagonalmatrix->getSimpleMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        if(j == i)
        {
          if(_type == "simple")
          {
            _simplematrix[pos] += d *massmatrix[i];
          }
          else if(_type == "diagonal")
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _diagonalmatrix[icomp][pos] += d *massmatrix[i];
            }
          }
          else
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _fullmatrix(icomp, icomp)[pos] += d *massmatrix[i];
            }
          }
        }
      }
    }
  }
  else if(diagonalmatrix->getType() == "diagonal")
  {
    const Alat::Vector<Alat::DoubleVector>& massmatrix = diagonalmatrix->getDiagonalMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        if(j == i)
        {
          if(_type == "simple")
          {
            assert(0);
          }
          else if(_type == "diagonal")
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _diagonalmatrix[icomp][pos] += d *massmatrix[icomp][i];
            }
          }
          else
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _fullmatrix(icomp, icomp)[pos] += d *massmatrix[icomp][i];
            }
          }
        }
      }
    }
  }
  else
  {
    const Alat::Matrix<Alat::DoubleVector>& massmatrix = diagonalmatrix->getFullMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        if(j == i)
        {
          if(_type == "simple")
          {
            assert(0);
          }
          else if(_type == "diagonal")
          {
            assert(0);
          }
          else
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              for(int jcomp = 0; jcomp < _ncomp; jcomp++)
              {
                _fullmatrix(icomp, jcomp)[pos] += d *massmatrix(icomp, jcomp)[i];
              }
            }
          }
        }
      }
    }
  }
}

void VariableMatrix::_addMatrix(const VariableMatrix* variablematrix, double d)
{
  const Alat::SparsityPattern* SP =  getSparsityPattern();
  const Alat::SparsityPattern* SP2 =  variablematrix->getSparsityPattern();
  int n = SP->n();
  assert( n == SP2->n() );
  if(variablematrix->getType() == "simple")
  {
    const Alat::DoubleVector& massmatrix = variablematrix->getSimpleMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        assert(SP2->col(pos) == j);
        if(_type == "simple")
        {
          _simplematrix[pos] += d *massmatrix[pos];
        }
        else if(_type == "diagonal")
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            _diagonalmatrix[icomp][pos] += d *massmatrix[pos];
          }
        }
        else
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            _fullmatrix(icomp, icomp)[pos] += d *massmatrix[pos];
          }
        }
      }
    }
  }
  else if(variablematrix->getType() == "diagonal")
  {
    const Alat::Vector<Alat::DoubleVector>& massmatrix = variablematrix->getDiagonalMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        if(_type == "simple")
        {
          assert(0);
        }
        else if(_type == "diagonal")
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            _diagonalmatrix[icomp][pos] += d *massmatrix[icomp][pos];
          }
        }
        else
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            _fullmatrix(icomp, icomp)[pos] += d *massmatrix[icomp][pos];
          }
        }
      }
    }
  }
  else
  {
    const Alat::Matrix<Alat::DoubleVector>& massmatrix = variablematrix->getFullMatrix();
    for(int i = 0; i < n; i++)
    {
      for(int pos = SP->rowstart(i); pos < SP->rowstop(i); pos++)
      {
        int j = SP->col(pos);
        if(_type == "simple")
        {
          assert(0);
        }
        else if(_type == "diagonal")
        {
          assert(0);
        }
        else
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < _ncomp; jcomp++)
            {
              _fullmatrix(icomp, jcomp)[pos] += d *massmatrix(icomp, jcomp)[pos];
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::zeroLine(int index, int icomp)
{
  const Alat::SparsityPattern& sparsitypattern = *getSparsityPattern();
  for(int pos = sparsitypattern.rowstart(index); pos < sparsitypattern.rowstop(index); pos++)
  {
    for(int jcomp = 0; jcomp < getMComponents(); jcomp++)
    {
      // getValue (pos)(icomp, jcomp) = 0.0;
      // getEntry( pos,  icomp,  jcomp) = 0.0;
      getValue( icomp,  jcomp, pos) = 0.0;
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::setElement(int index, int icomp, int jcomp, double d)
{
  const Alat::SparsityPattern& sparsitypattern = *getSparsityPattern();
  for(int pos = sparsitypattern.rowstart(index); pos < sparsitypattern.rowstop(index); pos++)
  {
    if(sparsitypattern.col(pos) == index)
    {
      // getValue (pos)(icomp, jcomp) = d;
      // getEntry( pos,  icomp,  jcomp) = d;
      getValue( icomp,  jcomp, pos) = 0.0;
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  // std::cerr << "VariableMatrix::matrixVectorProduct() type="<<_type << " d="<<d<<" _ncomp="<<_ncomp<<"\n";

  Alat::VariableVector* vf = dynamic_cast<Alat::VariableVector*>( f );
  assert(vf);
  const Alat::VariableVector* vu = dynamic_cast<const Alat::VariableVector*>( u );
  assert(vu);
  // std::cerr << "VariableMatrix::matrixVectorProduct() u="<<_type << " d="<<vu->writeAscii(std::cerr)<<"\n";

  int n = _sparsitypattern.n();
  int nf = vf->n();
  int nu = vu->n();
	if (n != nf)
	{
		std::cerr << "VariableMatrix::matrixVectorProduct() n="<<n << " nu="<< nu << " nf="<<nf<< " type=" << _type << "\n";
		assert(0);
	}

  if(_type == "simple")
  {
    Alat::DoubleVector::const_iterator a = _simplematrix.begin();
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      Alat::VariableVector::iterator p = vf->begin(icomp);
      Alat::VariableVector::const_iterator q = vu->begin(icomp);
      for(int i = 0; i < n; i++)
      {
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int j = _sparsitypattern.col(pos);
          *p += d*  *( a+pos ) * *( q+j );
        }
        p++;
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      Alat::DoubleVector::const_iterator a = _diagonalmatrix[icomp].begin();
      Alat::VariableVector::iterator p = vf->begin(icomp);
      Alat::VariableVector::const_iterator q = vu->begin(icomp);
      for(int i = 0; i < n; i++)
      {
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int j = _sparsitypattern.col(pos);
          *p += d*  *( a+pos ) * *( q+j );
        }
        p++;
      }
    }
  }
  else
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _mcomp; jcomp++)
      {
        Alat::DoubleVector::const_iterator a = _fullmatrix(icomp, jcomp).begin();
        Alat::VariableVector::iterator p = vf->begin(icomp);
        Alat::VariableVector::const_iterator q = vu->begin(jcomp);
        for(int i = 0; i < n; i++)
        {
          for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
          {
            int j = _sparsitypattern.col(pos);
            *p += d*  *( a+pos ) * *( q+j );
          }
          p++;
        }
      }
    }
  }
// // marche
// for(int i = 0; i < n; i++)
// {
//   for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
//   {
//     int j = _sparsitypattern.col(pos);
//     for(int icomp = 0; icomp < ncomp; icomp++)
//     {
//       for(int jcomp = 0; jcomp < mcomp; jcomp++)
//       {
//         (* vf)(icomp, i) += d*_matrix(icomp, jcomp)[pos]*(* vu)(jcomp, j);
//       }
//     }
//   }
// }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::matrixVectorProductTransposed(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  // std::cerr << "VariableMatrix::matrixVectorProduct() type="<<_type << " d="<<d<<" _ncomp="<<_ncomp<<"\n";

  Alat::VariableVector* vf = dynamic_cast<Alat::VariableVector*>( f );
  assert(vf);
  const Alat::VariableVector* vu = dynamic_cast<const Alat::VariableVector*>( u );
  assert(vu);
  // std::cerr << "VariableMatrix::matrixVectorProduct() u="<<_type << " d="<<vu->writeAscii(std::cerr)<<"\n";

  int n = _sparsitypattern.n();
  int nf = vf->n();
  int nu = vu->n();
	if (n != nu)
	{
		std::cerr << "VariableMatrix::matrixVectorProductTransposed() n="<<n << " nu="<< nu << " nf="<<nf<< " type=" << _type << "\n";
		assert(0);
	}

  if(_type == "simple")
  {
    Alat::DoubleVector::const_iterator a = _simplematrix.begin();
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      Alat::VariableVector::iterator p = vf->begin(icomp);
      Alat::VariableVector::const_iterator q = vu->begin(icomp);
      for(int i = 0; i < n; i++)
      {
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int j = _sparsitypattern.col(pos);
          *(p+j) += d*  *( a+pos ) * *( q );
        }
        q++;
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      Alat::DoubleVector::const_iterator a = _diagonalmatrix[icomp].begin();
      Alat::VariableVector::iterator p = vf->begin(icomp);
      Alat::VariableVector::const_iterator q = vu->begin(icomp);
      for(int i = 0; i < n; i++)
      {
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          int j = _sparsitypattern.col(pos);
          *(p+j) += d*  *( a+pos ) * *( q );
        }
        q++;
      }
    }
  }
  else
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _mcomp; jcomp++)
      {
        Alat::DoubleVector::const_iterator a = _fullmatrix(icomp, jcomp).begin();
        Alat::VariableVector::iterator p = vf->begin(icomp);
        Alat::VariableVector::const_iterator q = vu->begin(jcomp);
        for(int i = 0; i < n; i++)
        {
          for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
          {
            int j = _sparsitypattern.col(pos);
            *(p+j) += d*  *( a+pos ) * *( q );
          }
          q++;
        }
      }
    }
  }
// // marche
// for(int i = 0; i < n; i++)
// {
//   for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
//   {
//     int j = _sparsitypattern.col(pos);
//     for(int icomp = 0; icomp < ncomp; icomp++)
//     {
//       for(int jcomp = 0; jcomp < mcomp; jcomp++)
//       {
//         (* vf)(icomp, i) += d*_matrix(icomp, jcomp)[pos]*(* vu)(jcomp, j);
//       }
//     }
//   }
// }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  int n = _sparsitypattern.n();
  if( ( _type == "simple" )or(_type == "diagonal") )
  {
    for(int i = 0; i < n; i++)
    {
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          int index = offsetivar + i*_ncomp + icomp;
          int jndex = offsetjvar + j*_ncomp + icomp;
          sparsitypatternsoft[index].insert(jndex);
        }
      }
    }
  }
  else
  {
    for(int i = 0; i < n; i++)
    {
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          int index = offsetivar + i*_ncomp + icomp;
          for(int jcomp = 0; jcomp < _mcomp; jcomp++)
          {
            int jndex = offsetjvar + j*_mcomp + jcomp;
            sparsitypatternsoft[index].insert(jndex);
          }
        }
      }
    }
  }
}

void VariableMatrix::addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const
{
  int n = _sparsitypattern.n();

  if(_type == "simple")
  {
    for(int i = 0; i < n; i++)
    {
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          int index = offsetivar + i*_ncomp + icomp;
          int jndex = offsetjvar + j*_ncomp + icomp;
          for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
          {
            int j = sparsitypattern->col(pos2);
            if(j == jndex)
            {
              matrixvalues[pos2] +=  _simplematrix[pos];
            }
          }
        }
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = 0; i < n; i++)
    {
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          int index = offsetivar + i*_ncomp + icomp;
          int jndex = offsetjvar + j*_ncomp + icomp;
          for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
          {
            int j = sparsitypattern->col(pos2);
            if(j == jndex)
            {
              matrixvalues[pos2] +=  _diagonalmatrix[icomp][pos];
            }
          }
        }
      }
    }
  }
  else
  {
    for(int i = 0; i < n; i++)
    {
      for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
      {
        int j = _sparsitypattern.col(pos);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          int index = offsetivar + i*_ncomp + icomp;
          for(int jcomp = 0; jcomp < _mcomp; jcomp++)
          {
            int jndex = offsetjvar + j*_mcomp + jcomp;
            for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
            {
              int j = sparsitypattern->col(pos2);
              if(j == jndex)
              {
                matrixvalues[pos2] +=  _fullmatrix(icomp, jcomp)[pos];
              }
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableMatrix::assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej)
{
  assert( _ncomp == Aloc.ncomp() );
  assert( _mcomp == Aloc.mcomp() );
  assert( _ncomp == scalei.size() );
  assert( _mcomp == scalej.size() );
  if(_type == "simple")
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      for(int jj = 0; jj < jndices.size(); jj++)
      {
        int j = jndices[jj];
        bool found = false;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          if(_sparsitypattern.col(pos) == j)
          {
            double dmean = 0.0;
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              double scale = scalej[icomp]/scalei[icomp];
              dmean += scale*Aloc(icomp, icomp, ii, jj);
            }
            dmean /= (double) _ncomp;
            _simplematrix[pos] += dmean;
            found = true;
            continue;
          }
        }
        if(not found)
        {
          std::cerr << " VariableMatrix::assemble(): did not find column " << j << " in row " << i << "\n";
          std::cerr << "indices jndices " << indices << "  ==>  " << jndices << "\n";
          assert(0);
        }
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      for(int jj = 0; jj < jndices.size(); jj++)
      {
        int j = jndices[jj];
        bool found = false;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          if(_sparsitypattern.col(pos) == j)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              double scale = scalej[icomp]/scalei[icomp];
              _diagonalmatrix[icomp][pos] += scale*Aloc(icomp, icomp, ii, jj);
            }
            found = true;
            continue;
          }
        }
        if(not found)
        {
          std::cerr << " VariableMatrix::assemble(): did not find column " << j << " in row " << i << "\n";
          std::cerr << "indices jndices " << indices << "  ==>  " << jndices << "\n";
          assert(0);
        }
      }
    }
  }
  else
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      for(int jj = 0; jj < jndices.size(); jj++)
      {
        int j = jndices[jj];
        bool found = false;
        for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
        {
          if(_sparsitypattern.col(pos) == j)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              for(int jcomp = 0; jcomp < _mcomp; jcomp++)
              {
                double scale = scalej[jcomp]/scalei[icomp];
                _fullmatrix(icomp, jcomp)[pos] += scale*Aloc(icomp, jcomp, ii, jj);
              }
            }
            found = true;
            continue;
          }
        }
        if(not found)
        {
          std::cerr << " VariableMatrix::assemble(): did not find column " << j << " in row " << i << "\n";
          std::cerr << "indices jndices " << indices << "  ==>  " << jndices << "\n";
          std::cerr << "_ncomp=" << _ncomp << " _mcomp" << _mcomp << "\n";
          std::cerr << "_sparsitypattern=" << _sparsitypattern << "\n";
          assert(0);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* VariableMatrix::newSchurComplementMatrix(const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, std::string type, std::string storage) const
{
  const Alat::VariableMatrix* BSFM = dynamic_cast<const Alat::VariableMatrix*>( B );
  const Alat::VariableMatrix* CSFM = dynamic_cast<const Alat::VariableMatrix*>( C );
  assert(BSFM);
  assert(CSFM);
  int ncomp = BSFM->getNComponents();
  // std::cerr << "VariableMatrix::newSchurComplementMatrix() this: _ncomp | _mcomp " << _ncomp << " | " << _mcomp <<"\n";
  // std::cerr << "VariableMatrix::newSchurComplementMatrix() B:    _ncomp | _mcomp " << BSFM->getNComponents() << " | " << BSFM->getMComponents() <<"\n";
  // std::cerr << "VariableMatrix::newSchurComplementMatrix() C:    _ncomp | _mcomp " << CSFM->getNComponents() << " | " << CSFM->getMComponents() <<"\n";
  assert(_ncomp == _mcomp);
  assert(_ncomp == ncomp);
  assert(CSFM->getMComponents() == ncomp);
  assert( CSFM->getNComponents() == BSFM->getMComponents() );
  // std::cerr << "##### VariableMatrix::newSchurComplementMatrix() type="<<type<<" storage="<<storage<<"\n";
  // assert(0);
  if(type == "diagonal")
  {
    return new VariableDiagonalMatrix(ncomp, ncomp, storage);
  }
  else
  {
    return new VariableMatrix(ncomp, ncomp, storage);
  }
}

void VariableMatrix::reInitSchurMatrix(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil)
{
  Alat::SparsityPatternSoft sparsitypatternsoft( B->getSparsityPattern()->n() );
  computeSchurSparsityPatternSoft(sparsitypatternsoft, A, B, C, D, fullStencil);
  set_size(sparsitypatternsoft);
}

void VariableMatrix::computeSchurSparsityPatternSoft(Alat::SparsityPatternSoft& sparsitypatternsoft, const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil) const
{
  if(D)
  {
    const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
    for(int i = 0; i < SPD.n(); i++)
    {
      for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
      {
        sparsitypatternsoft[i].insert( SPD.col(posD) );
      }
    }
  }
  if(not fullStencil)
  {
    assert(D);
    return;
  }
  const Alat::SparsityPattern& SPB = *B->getSparsityPattern();
  const Alat::SparsityPattern& SPC = *C->getSparsityPattern();
  for(int i = 0; i < SPB.n(); i++)
  {
    for(int pos1 = SPB.rowstart(i); pos1 < SPB.rowstop(i); pos1++)
    {
      int j = SPB.col(pos1);
      for(int pos2 = SPC.rowstart(j); pos2 < SPC.rowstop(j); pos2++)
      {
        sparsitypatternsoft[i].insert( SPC.col(pos2) );
      }
    }
  }
}

void VariableMatrix::computeSchurComplement(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D)
{
  const VariableMatrix* AV = dynamic_cast<const VariableMatrix*>( A );
  assert(AV);
  const VariableMatrix* BV = dynamic_cast<const VariableMatrix*>( B );
  assert(BV);
  const VariableMatrix* CV = dynamic_cast<const VariableMatrix*>( C );
  assert(CV);
  const VariableMatrix* DV = dynamic_cast<const VariableMatrix*>( D );
  if(D)
  {
    assert(DV);
    assert(DV->getNComponents() == _ncomp);
    assert(DV->getMComponents() == _ncomp);
  }
  assert(_ncomp == _mcomp);
  int ncompA = AV->getNComponents();
  assert(AV->getMComponents() == ncompA);
  assert(BV->getNComponents() == _ncomp);
  assert(BV->getMComponents() == ncompA);
  assert(CV->getNComponents() == ncompA);
  assert(CV->getMComponents() == _ncomp);

  const Alat::SparsityPattern& SPS = *getSparsityPattern();
  const Alat::SparsityPattern& SPA = *A->getSparsityPattern();
  const Alat::SparsityPattern& SPB = *B->getSparsityPattern();
  const Alat::SparsityPattern& SPC = *C->getSparsityPattern();

  zeros();
  std::string type = AV->getType();
  assert(type == "full");
  assert( DV->getType() == type );
  if(_type == "simple")
  {
    arma::mat BB(_ncomp, ncompA), CC(ncompA, _ncomp), F(ncompA, ncompA), H(_ncomp, ncompA), P(_ncomp, _ncomp);
    if(D)
    {
      const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
      for(int i = 0; i < SPD.n(); i++)
      {
        for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
        {
          for(int pos = SPS.rowstart(i); pos < SPS.rowstop(i); pos++)
          {
            if( SPD.col(posD) == SPS.col(pos) )
            {
              double d = 0.0;
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                d += DV->getFullMatrix(icomp, icomp)[posD];
              }
              _simplematrix[pos] += d;
            }
          }
        }
      }
    }
    for(int i = 0; i < SPB.n(); i++)
    {
      for(int posB = SPB.rowstart(i); posB < SPB.rowstop(i); posB++)
      {
        int j = SPB.col(posB);
        int posF;
        bool found = 0;
        for(posF = SPA.rowstart(j); posF < SPA.rowstop(j); posF++)
        {
          if(SPA.col(posF) == j)
          {
            found = 1;
            break;
          }
        }
        assert(found);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < ncompA; jcomp++)
          {
            BB(icomp, jcomp) = BV->getFullMatrix(icomp, jcomp)[posB];
          }
        }
        for(int icomp = 0; icomp < ncompA; icomp++)
        {
          for(int jcomp = 0; jcomp < ncompA; jcomp++)
          {
            F(icomp, jcomp) = AV->getFullMatrix(icomp, jcomp)[posF];
          }
        }
        F = arma::inv(F);
        H = BB*F;
        for(int posC = SPC.rowstart(j); posC < SPC.rowstop(j); posC++)
        {
          int k = SPC.col(posC);
          for(int icomp = 0; icomp < ncompA; icomp++)
          {
            for(int jcomp = 0; jcomp < _ncomp; jcomp++)
            {
              CC(icomp, jcomp) = CV->getFullMatrix(icomp, jcomp)[posC];
            }
          }
          P = H*CC;
          for(int pos = SPS.rowstart(i); pos < SPS.rowstop(i); pos++)
          {
            if(SPS.col(pos) == k)
            {
              double d = 0.0;
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                d += P(icomp, icomp);
              }
              _simplematrix[pos] -= d;
            }
          }
        }
      }
    }
  }

  else if(_type == "diagonal")
  {
    assert(0);
  }
  else
  {
    arma::mat BB(_ncomp, ncompA), CC(ncompA, _ncomp), F(ncompA, ncompA), H(_ncomp, ncompA), P(_ncomp, _ncomp);
    if(D)
    {
      const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
      for(int i = 0; i < SPD.n(); i++)
      {
        for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
        {
          for(int pos = SPS.rowstart(i); pos < SPS.rowstop(i); pos++)
          {
            if( SPD.col(posD) == SPS.col(pos) )
            {
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                for(int jcomp = 0; jcomp < _mcomp; jcomp++)
                {
                  _fullmatrix(icomp, jcomp)[pos] += DV->getFullMatrix(icomp, jcomp)[posD];
                }
              }
            }
          }
        }
      }
    }
    for(int i = 0; i < SPB.n(); i++)
    {
      for(int posB = SPB.rowstart(i); posB < SPB.rowstop(i); posB++)
      {
        int j = SPB.col(posB);
        int posF;
        bool found = 0;
        for(posF = SPA.rowstart(j); posF < SPA.rowstop(j); posF++)
        {
          if(SPA.col(posF) == j)
          {
            found = 1;
            break;
          }
        }
        assert(found);
        for(int icomp = 0; icomp < _ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < ncompA; jcomp++)
          {
            BB(icomp, jcomp) = BV->getFullMatrix(icomp, jcomp)[posB];
          }
        }
        for(int icomp = 0; icomp < ncompA; icomp++)
        {
          for(int jcomp = 0; jcomp < ncompA; jcomp++)
          {
            F(icomp, jcomp) = AV->getFullMatrix(icomp, jcomp)[posF];
          }
        }
        F = arma::inv(F);
        H = BB*F;
        for(int posC = SPC.rowstart(j); posC < SPC.rowstop(j); posC++)
        {
          int k = SPC.col(posC);
          for(int icomp = 0; icomp < ncompA; icomp++)
          {
            for(int jcomp = 0; jcomp < _ncomp; jcomp++)
            {
              CC(icomp, jcomp) = CV->getFullMatrix(icomp, jcomp)[posC];
            }
          }
          P = H*CC;
          for(int pos = SPS.rowstart(i); pos < SPS.rowstop(i); pos++)
          {
            if(SPS.col(pos) == k)
            {
              for(int icomp = 0; icomp < _ncomp; icomp++)
              {
                for(int jcomp = 0; jcomp < _ncomp; jcomp++)
                {
                  _fullmatrix(icomp, jcomp)[pos] -= P(icomp, jcomp);
                }
              }
            }
          }
        }
      }
    }
  }
// std::cerr << "SCHUR\n";
// S->write(std::cerr);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* VariableMatrix::newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const Alat::VariableMatrixInterface* matrix) const
{
  Alat::VariableMatrixInterface* matrixspai;
  if(type == "diagonal")
  {
    matrixspai = new Alat::VariableDiagonalMatrix(ncomp, ncomp, storage);
    matrixspai->set_size( getSparsityPattern()->n() );
  }
  else
  {
    Alat::SpaiVariableMatrix* matrixspaivm = new Alat::SpaiVariableMatrix(ncomp, storage);
    // matrixspai->set_size( getSparsityPattern()->n(), getSparsityPattern()->ntotal() );
		const VariableMatrix* variablematrix = dynamic_cast<const VariableMatrix*>(matrix);
		assert(variablematrix);
    matrixspaivm->set_size( variablematrix );
		matrixspai = matrixspaivm;
  }
  return matrixspai;
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* VariableMatrix::newGaussSeidelMatrixAndReinit(int ncomp, std::string type, std::string storage) const
{
  GaussSeidelVariableDiagonalMatrix* matrixgs = new GaussSeidelVariableDiagonalMatrix(ncomp, storage);
  matrixgs->set_size( getSparsityPattern()->n() );
  return matrixgs;
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* VariableMatrix::newIluMatrixAndReinit(int ncomp, const Alat::PreconditionerInterface* preconditioner, std::string type, std::string storage) const
{
  const Alat::PreconditionerWithSorting* preconditionerwithsorting = dynamic_cast<const Alat::PreconditionerWithSorting*>(preconditioner);
  // std::cerr << "VariableMatrix::newIluMatrixAndReinit() type="<<type<<" storage="<<storage<<"\n";
	// storage="full";
  IluVariableMatrix* matrixilu = new IluVariableMatrix(ncomp, preconditionerwithsorting, storage);
  matrixilu->set_size( getSparsityPattern()->n(), getSparsityPattern()->ntotal() );
  return matrixilu;
}

/*-------------------------------------------------------------*/
void VariableMatrix::suppressZeros(double epsilon)
{
  assert(_type=="simple");
  int n = _sparsitypattern.n();
  Alat::SparsityPatternSoft sparsitypatternsoft(n);

  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      if(_simplematrix[pos] >= epsilon)
      {
        sparsitypatternsoft[i].insert( _sparsitypattern.col(pos) );
      }
    }
  }
  // std::cerr << "SparseMatrix::suppressZeros() " << _sparsitypattern.ntotal() << " ==> " << sparsitypatternsoft.ntotal() << "\n";
  Alat::DoubleVector newvalue;
  newvalue.set_size( sparsitypatternsoft.ntotal() );
  int count = 0;
  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      if(_simplematrix[pos] >= epsilon)
      {
        newvalue[count++] = _simplematrix[pos];
      }
    }
  }
  // std::cerr << "_value " << _value << "\n";
  _simplematrix.set_size(newvalue.size());
  _simplematrix = newvalue;
  // std::cerr << "newvalue " << newvalue << "\n";
  // std::cerr << "_value " << _value << "\n";
  _sparsitypattern.set_size(sparsitypatternsoft);
}
