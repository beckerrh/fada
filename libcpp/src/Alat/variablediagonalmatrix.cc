// #include  "Fada/dofinformationinterface.h"
// #include  "Fada/localdofinformation.h"
// #include  "FadaMesh/meshinterface.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/doublevector.h"
#include  "Alat/variablediagonalmatrix.h"
#include  "Alat/variablematrix.h"
#include  <cassert>
#include  "Alat/armadillo.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableDiagonalMatrix::~VariableDiagonalMatrix(){}
VariableDiagonalMatrix::VariableDiagonalMatrix(int ncomp, int mcomp, std::string type) : Alat::VariableMatrixInterface(), _type(type)
{
  _ncomp = ncomp;
  _mcomp = mcomp;
  if(_type == "simple")
  {
    assert(ncomp == mcomp);
  }
  else if(_type == "diagonal")
  {
    assert(ncomp == mcomp);
    _diagonalmatrix.set_size(ncomp);
  }
  else
  {
    assert(_type == "full");
    _fullmatrix.set_size(ncomp, mcomp);
  }
}
VariableDiagonalMatrix::VariableDiagonalMatrix( const VariableDiagonalMatrix& variablematrix) : Alat::VariableMatrixInterface(variablematrix)
{
  assert(0);
}
VariableDiagonalMatrix& VariableDiagonalMatrix::operator=( const VariableDiagonalMatrix& variablematrix)
{
  Alat::VariableMatrixInterface::operator=(variablematrix);
  assert(0);
  return *this;
}
std::string VariableDiagonalMatrix::getName() const
{
  if(_type == "simple")
  {
    return "VariableDiagonalMatrix_Simple";
  }
  else if(_type == "diagonal")
  {
    return "VariableDiagonalMatrix_Diagonal";
  }
  return "VariableDiagonalMatrix_Full";
}
VariableDiagonalMatrix* VariableDiagonalMatrix::clone() const
{
  assert(0);
//return new VariableDiagonalMatrix(*this);
}

/*--------------------------------------------------------------------------*/
std::string VariableDiagonalMatrix::getType() const
{
  return _type;
}

const Alat::DoubleVector& VariableDiagonalMatrix::getFullMatrix(int icomp, int jcomp) const
{
  assert(_type == "full");
  return _fullmatrix(icomp, jcomp);
}

const Alat::DoubleVector& VariableDiagonalMatrix::getDiagonalMatrix(int icomp) const
{
  assert(_type == "diagonal");
  return _diagonalmatrix[icomp];
}

const Alat::DoubleVector& VariableDiagonalMatrix::getSimpleMatrix() const
{
  assert(_type == "simple");
  return _simplematrix;
}

Alat::DoubleVector& VariableDiagonalMatrix::getFullMatrix(int icomp, int jcomp)
{
  assert(_type == "full");
  return _fullmatrix(icomp, jcomp);
}

Alat::DoubleVector& VariableDiagonalMatrix::getDiagonalMatrix(int icomp)
{
  assert(_type == "diagonal");
  return _diagonalmatrix[icomp];
}

Alat::DoubleVector& VariableDiagonalMatrix::getSimpleMatrix()
{
  assert(_type == "simple");
  return _simplematrix;
}

const Alat::Vector<Alat::DoubleVector>& VariableDiagonalMatrix::getDiagonalMatrix() const
{
  assert(_type == "diagonal");
  return _diagonalmatrix;
}

const Alat::Matrix<Alat::DoubleVector>& VariableDiagonalMatrix::getFullMatrix() const
{
  assert(_type == "full");
  return _fullmatrix;
}

int VariableDiagonalMatrix::getN() const
{
  if(_type == "simple")
  {
    return _simplematrix.size();
  }
  else if(_type == "diagonal")
  {
    return _diagonalmatrix[0].size();
  }
  return _fullmatrix(0, 0).size();
}

int VariableDiagonalMatrix::getNComponents() const
{
  return _ncomp;
}

int VariableDiagonalMatrix::getMComponents() const
{
  return _mcomp;
}

const double& VariableDiagonalMatrix::getValue(int icomp, int jcomp, int pos) const
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

double& VariableDiagonalMatrix::getValue(int icomp, int jcomp, int pos)
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

/*-------------------------------------------------------------*/

bool VariableDiagonalMatrix::greaterThan(int pos1, int pos2) const
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

std::ostream& Alat::operator<<(std::ostream& os, const VariableDiagonalMatrix& A)
{
  return A.write(os);
}

std::ostream& VariableDiagonalMatrix::write(std::ostream& os) const
{
  if(_type == "simple")
  {
    os << _simplematrix;
  }
  else if(_type == "diagonal")
  {
    os << _diagonalmatrix;
  }
  else
  {
    os << _fullmatrix;
  }
  os << "\n";
  return os;
}

/*--------------------------------------------------------------------------*/

void VariableDiagonalMatrix::zeros()
{
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
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _ncomp; jcomp++)
      {
        _fullmatrix(icomp, jcomp).zeros();
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::set_size(int n)
{
  std::cerr << "VariableDiagonalMatrix::reInit( n="<<n<<"\n";
  if(_type == "simple")
  {
    _simplematrix.set_size( n );
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _diagonalmatrix[icomp].set_size(n);
    }
  }
  else
  {
    for(Alat::Matrix<Alat::DoubleVector>::iterator p = _fullmatrix.begin(); p != _fullmatrix.end(); p++)
    {
      p->set_size( n );
    }
  }
}
//
// void VariableDiagonalMatrix::set_size(const FadaMesh::MeshInterface* mesh, const Fada::DofInformationInterface* dofinformationi, const Fada::DofInformationInterface* dofinformationj)
// {
//   const Fada::LocalDofInformation* localdofinformationi = dofinformationi->getLocalDofInformation();
//   assert(localdofinformationi);
//   const Fada::LocalDofInformation* localdofinformationj = dofinformationj->getLocalDofInformation();
//   assert(localdofinformationj);
//
//   int n;
//   if( ( localdofinformationi->nodes() == 1 ) &&( localdofinformationj->nodes() == 1 ) )
//   {
//     assert(localdofinformationi->cells() == 0);
//     assert(localdofinformationj->cells() == 0);
//     n = mesh->getNNodes();
//   }
//   else if( ( localdofinformationi->cells() == 1 ) &&( localdofinformationj->cells() == 1 ) )
//   {
//     assert(localdofinformationi->nodes() == 0);
//     assert(localdofinformationj->nodes() == 0);
//     assert(localdofinformationi->sides() == 0);
//     assert(localdofinformationj->sides() == 0);
//     n = mesh->getNCells();
//   }
//   else if( ( localdofinformationi->cells() == 4 ) &&( localdofinformationj->cells() == 4 ) )
//   {
//     assert(localdofinformationi->nodes() == 0);
//     assert(localdofinformationj->nodes() == 0);
//     assert(localdofinformationi->sides() == 0);
//     assert(localdofinformationj->sides() == 0);
//     n = 4*mesh->getNCells();
//   }
//   else if( ( localdofinformationi->sides() == 1 ) &&( localdofinformationj->sides() == 1 ) )
//   {
//     assert(localdofinformationi->nodes() == 0);
//     assert(localdofinformationj->nodes() == 0);
//     n = mesh->getNSides();
//   }
//   else
//   {
//     assert(0);
//   }
//   this->set_size(n);
// }

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::addMatrix(const Alat::VariableMatrixInterface* sparsematrix, double d)
{
  // std::cerr << "VariableDiagonalMatrix::addMatrix() d="<<d<<"\n";
  assert(_ncomp == _mcomp);
  const VariableDiagonalMatrix* variablediagonalmatrix = dynamic_cast<const VariableDiagonalMatrix*>( sparsematrix );
  assert(variablediagonalmatrix);
  assert(variablediagonalmatrix->getNComponents() == _ncomp);
  assert(variablediagonalmatrix->getMComponents() == _mcomp);
  assert(variablediagonalmatrix->getType() == _type);
  if(_type == "simple")
  {
    _simplematrix.add( d, variablediagonalmatrix->getSimpleMatrix() );
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      _diagonalmatrix[icomp].add ( d, variablediagonalmatrix->getDiagonalMatrix(icomp) );
    }
  }
  else
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _ncomp; jcomp++)
      {
        _fullmatrix(icomp, jcomp).add( d, variablediagonalmatrix->getFullMatrix(icomp, jcomp) );
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::zeroLine(int index, int icomp)
{
  if(_type == "simple")
  {
    _simplematrix[index] = 0.0;
  }
  else if(_type == "diagonal")
  {
    _diagonalmatrix[icomp][index] = 0.0;
  }
  else
  {
    for(int jcomp = 0; jcomp < _ncomp; jcomp++)
    {
      _fullmatrix(icomp, jcomp)[index] = 0.0;
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::setElement(int index, int icomp, int jcomp, double d)
{
  if(_type == "simple")
  {
    _simplematrix[index] = d;
  }
  else if(_type == "diagonal")
  {
    _diagonalmatrix[icomp][index] = d;
  }
  else
  {
    _fullmatrix(icomp, jcomp)[index] = d;
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  // std::cerr << "VariableDiagonalMatrix::matrixVectorProduct() type="<<_type << " d="<<d<<" _ncomp="<<_ncomp<<"\n";

  Alat::VariableVector* vf = dynamic_cast<Alat::VariableVector*>( f );
  assert(vf);
  const Alat::VariableVector* vu = dynamic_cast<const Alat::VariableVector*>( u );
  assert(vu);
  // std::cerr << "VariableDiagonalMatrix::matrixVectorProduct() u="<<_type << " d="<<vu->writeAscii(std::cerr)<<"\n";

  int n = getN();
  int nf = vf->n();
  int nu = vu->n();
  assert(n == nf);
  assert(n == nu);

  if(_type == "simple")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int i = 0; i < n; i++)
      {
        ( *vf )(icomp, i) += d * _simplematrix[i]*( *vu )(icomp, i);
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int i = 0; i < n; i++)
      {
        ( *vf )(icomp, i) += d * _diagonalmatrix[icomp][i]*( *vu )(icomp, i);
      }
    }
  }
  else
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < _ncomp; jcomp++)
      {
        for(int i = 0; i < n; i++)
        {
          ( *vf )(icomp, i) += d * _fullmatrix(icomp, jcomp)[i]*( *vu )(jcomp, i);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  int n = getN();
  if( ( _type == "simple" )or(_type == "diagonal") )
  {
    for(int i = 0; i < n; i++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        int jndex = offsetjvar + i*_ncomp + icomp;
        sparsitypatternsoft[index].insert(jndex);
      }
    }
  }
  else
  {
    for(int i = 0; i < n; i++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          int jndex = offsetjvar + i*_mcomp + jcomp;
          sparsitypatternsoft[index].insert(jndex);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const
{
  int n = getN();

  if(_type == "simple")
  {
    for(int i = 0; i < n; i++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        int jndex = offsetjvar + i*_ncomp + icomp;
        for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
        {
          int j = sparsitypattern->col(pos2);
          if(j == jndex)
          {
            matrixvalues[pos2] +=  _simplematrix[i];
          }
        }
      }
    }
  }
  else if(_type == "diagonal")
  {
    for(int i = 0; i < n; i++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        int jndex = offsetjvar + i*_ncomp + icomp;
        for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
        {
          int j = sparsitypattern->col(pos2);
          if(j == jndex)
          {
            matrixvalues[pos2] +=  _diagonalmatrix[icomp][i];
          }
        }
      }
    }
  }
  else
  {
    for(int i = 0; i < n; i++)
    {
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          int jndex = offsetjvar + i*_mcomp + jcomp;
          for(int pos2 = sparsitypattern->rowstart(index); pos2 < sparsitypattern->rowstop(index); pos2++)
          {
            int j = sparsitypattern->col(pos2);
            if(j == jndex)
            {
              matrixvalues[pos2] +=  _fullmatrix(icomp, jcomp)[i];
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej)
{
  assert( _ncomp == Aloc.ncomp() );
  assert( _mcomp == Aloc.mcomp() );
  if(_type == "simple")
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      double dmean = 0.0;
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        double scale = scalej[icomp]/scalei[icomp];        
        dmean += scale*Aloc(icomp, icomp, ii, ii);
      }
      dmean /= (double) _ncomp;
      _simplematrix[i] += dmean;
    }
  }
  else if(_type == "diagonal")
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        double scale = scalej[icomp]/scalei[icomp];        
        _diagonalmatrix[icomp][i] += scale*Aloc(icomp, icomp, ii, ii);
      }
    }
  }
  else
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      int i = indices[ii];
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          double scale = scalej[jcomp]/scalei[icomp];        
          _fullmatrix(icomp, jcomp)[i] += scale*Aloc(icomp, jcomp, ii, ii);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* VariableDiagonalMatrix::newSchurComplementMatrix(const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, std::string type, std::string storage) const
{
  const Alat::VariableMatrix* BSFM = dynamic_cast<const Alat::VariableMatrix*>( B );
  const Alat::VariableMatrix* CSFM = dynamic_cast<const Alat::VariableMatrix*>( C );
  assert(BSFM);
  assert(CSFM);
  int ncomp = BSFM->getNComponents();
  // std::cerr << "VariableDiagonalMatrix::newSchurComplementMatrix() this: _ncomp | _mcomp " << _ncomp << " | " << _mcomp <<"\n";
  // std::cerr << "VariableDiagonalMatrix::newSchurComplementMatrix() B:    _ncomp | _mcomp " << BSFM->getNComponents() << " | " << BSFM->getMComponents() <<"\n";
  // std::cerr << "VariableDiagonalMatrix::newSchurComplementMatrix() C:    _ncomp | _mcomp " << CSFM->getNComponents() << " | " << CSFM->getMComponents() <<"\n";
  assert(_ncomp == _mcomp);
  assert(_ncomp == ncomp);
  assert(CSFM->getMComponents() == ncomp);
  assert( CSFM->getNComponents() == BSFM->getMComponents() );
  return new VariableDiagonalMatrix(ncomp, ncomp, storage);
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::reInitSchurMatrix(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil)
{
  set_size( B->getSparsityPattern()->n() );
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::computeSchurComplement(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D)
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
  }
  assert(_ncomp == _mcomp);
  int ncompA = AV->getNComponents();
  assert(AV->getMComponents() == ncompA);
  assert(BV->getNComponents() == _ncomp);
  assert(BV->getMComponents() == ncompA);
  assert(CV->getNComponents() == ncompA);
  assert(CV->getMComponents() == _ncomp);

  const Alat::SparsityPattern& SPA = *A->getSparsityPattern();
  const Alat::SparsityPattern& SPB = *B->getSparsityPattern();
  const Alat::SparsityPattern& SPC = *C->getSparsityPattern();

  zeros();

  if(_type == "simple")
  {
    assert( AV->getType() == "simple" );
    assert( DV->getType() == "simple" );
    if(D)
    {
      const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
      for(int i = 0; i < SPD.n(); i++)
      {
        for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
        {
          if( SPD.col(posD) == i )
          {
            _simplematrix[i] += DV->getSimpleMatrix()[posD];
          }
        }
      }
    }
    arma::mat BB(_ncomp, ncompA), CC(ncompA, _ncomp);
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
        double F = 1.0/AV->getSimpleMatrix()[posF];
        // F = arma::inv(F);
        // H = BB*F;
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
          // P = H*CC;
          // P = BB*F^{-1}*CC;
          double P = 0.0;
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            for(int kcomp = 0; kcomp < ncompA; kcomp++)
            {
              for(int jcomp = 0; jcomp < _ncomp; jcomp++)
              {
                P += BB(icomp, kcomp)*CC(kcomp, jcomp)*F;
              }
            }
          }
          if(i == k)
          {
            _simplematrix[i] -= P;
          }
        }
      }
    }
  }
  else if(_type == "diagonal")
  {
    assert( AV->getType() == "diagonal" );
    assert( DV->getType() == "diagonal" );
    if(D)
    {
      const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
      for(int i = 0; i < SPD.n(); i++)
      {
        for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
        {
          if( SPD.col(posD) == i )
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _diagonalmatrix[icomp][i] += DV->getDiagonalMatrix(icomp)[posD];
            }
          }
        }
      }
    }
    arma::mat BB(_ncomp, ncompA), CC(ncompA, _ncomp), F(ncompA, ncompA), H(_ncomp, ncompA), P(_ncomp, _ncomp);
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
          F(icomp, icomp) = AV->getDiagonalMatrix(icomp)[posF];
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
          if(i == k)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              _diagonalmatrix[icomp][i] -= P(icomp, icomp);
            }
          }
        }
      }
    }
  }
  else
  {
    if(D)
    {
      const Alat::SparsityPattern& SPD = *D->getSparsityPattern();
      for(int i = 0; i < SPD.n(); i++)
      {
        for(int posD = SPD.rowstart(i); posD < SPD.rowstop(i); posD++)
        {
          if( SPD.col(posD) == i )
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              for(int jcomp = 0; jcomp < _mcomp; jcomp++)
              {
                _fullmatrix(icomp, jcomp)[i] += DV->getFullMatrix(icomp, jcomp)[posD];
              }
            }
          }
        }
      }
    }
    arma::mat BB(_ncomp, ncompA), CC(ncompA, _ncomp), F(ncompA, ncompA), H(_ncomp, ncompA), P(_ncomp, _ncomp);
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
          if(i == k)
          {
            for(int icomp = 0; icomp < _ncomp; icomp++)
            {
              for(int jcomp = 0; jcomp < _ncomp; jcomp++)
              {
                _fullmatrix(icomp, jcomp)[i] -= P(icomp, jcomp);
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
Alat::VariableMatrixInterface* VariableDiagonalMatrix::newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const Alat::VariableMatrixInterface* matrix) const
{
  Alat::VariableMatrixInterface* matrixspai;
  matrixspai = new Alat::VariableDiagonalMatrix(ncomp, ncomp, storage);
  matrixspai->set_size( getN() );
  return matrixspai;
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::computeSpai(double relax, const Alat::VariableMatrixInterface* variablematrixinterface)
{
  const Alat::VariableMatrix* variablematrix = dynamic_cast<const Alat::VariableMatrix*>( variablematrixinterface );
  if(variablematrix)
  {
    if(_type == "simple")
    {
      _computeSpaiSimple(relax, variablematrix);
    }
    else if(_type == "diagonal")
    {
      _computeSpaiDiagonal(relax, variablematrix);
    }
    else
    {
      _computeSpaiFull(relax, variablematrix);
    }
  }
  else
  {
    const Alat::VariableDiagonalMatrix* variablematrix = dynamic_cast<const Alat::VariableDiagonalMatrix*>( variablematrixinterface );
    assert(variablematrix);
    if(_type == "simple")
    {
      _computeSpaiDiagonalSimple(relax, variablematrix);
    }
    else if(_type == "diagonal")
    {
      _computeSpaiDiagonalDiagonal(relax, variablematrix);
    }
    else
    {
      _computeSpaiDiagonalFull(relax, variablematrix);
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiSimple(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype == "simple" or matrixtype == "full");

  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  zeros();

  int ncomp = variablematrix->getNComponents();
  double dncomp = 1.0/(double)ncomp;
  assert( variablematrix->getMComponents() == ncomp);

  Alat::DoubleVector& mysimplematrix = getSimpleMatrix();
  int n = mysimplematrix.size();
  assert( n == sparsitypattern->n() );
  const Alat::DoubleVector* simplematrix;
  const Alat::Matrix<Alat::DoubleVector>* fullmatrix;
  if(matrixtype == "simple")
  {
    simplematrix = &variablematrix->getSimpleMatrix();
  }
  else
  {
    fullmatrix = &variablematrix->getFullMatrix();
  }
  for(int i = 0; i < n; i++)
  {
    double sum = 0.0,  diag;
    for(int pos = sparsitypattern->rowstart(i); pos != sparsitypattern->rowstop(i); pos++)
    {
      int j = sparsitypattern->col(pos);
      if(j == i)
      {
        if(matrixtype == "simple")
        {
          diag = ( *simplematrix )[pos];
        }
        else
        {
          diag = 0.0;
          for(int icomp = 0; icomp < ncomp; icomp++)
          {
            diag += dncomp*fabs( ( *fullmatrix )(icomp, icomp)[pos] );
          }
        }
      }
      if(matrixtype == "simple")
      {
        double d = ( *simplematrix )[pos];
        sum += d*d;
      }
      else
      {
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < ncomp; jcomp++)
          {
            double d = ( *fullmatrix )(icomp, jcomp)[pos];
            sum += d*d;
          }
        }
      }
    }
    mysimplematrix[i] = diag/sum;
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiDiagonal(double relax, const Alat::VariableMatrix* variablematrix)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiFull(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype == "full");

  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  zeros();

  int ncomp = variablematrix->getNComponents();
  double dncomp = 1.0/(double)ncomp;
  assert( variablematrix->getMComponents() == ncomp);

  int n = _fullmatrix(0, 0).size();
  assert( n == sparsitypattern->n() );
  const Alat::Matrix<Alat::DoubleVector>* fullmatrix = &variablematrix->getFullMatrix();
  Alat::Vector<Alat::DoubleMatrix> ligneDeA;
  for(int i = 0; i < n; i++)
  {
    ligneDeA.set_size( sparsitypattern->rowsize(i) );
    for(int ii = 0; ii < ligneDeA.size(); ii++)
    {
      ligneDeA[ii].set_size(ncomp, ncomp);
    }
    int count = 0, diagonalindex;
    for(int pos = sparsitypattern->rowstart(i); pos != sparsitypattern->rowstop(i); pos++)
    {
      int j = sparsitypattern->col(pos);
      if(j == i)
      {
        diagonalindex = count;
      }
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < ncomp; jcomp++)
        {
          ligneDeA[count](icomp, jcomp) =  ( *fullmatrix )(icomp, jcomp)[pos];
        }
      }
      count++;
    }
    double sum = 0.0;
    for(int ii = 0; ii < ligneDeA.size(); ii++)
    {
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < ncomp; jcomp++)
        {
          double d = ligneDeA[ii](icomp, jcomp);
          sum += d*d;
        }
      }
    }
    for(int icomp = 0; icomp < ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < ncomp; jcomp++)
      {
        _fullmatrix(icomp, jcomp)[i] = ligneDeA[diagonalindex](jcomp, icomp)/sum;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiDiagonalSimple(double relax, const Alat::VariableDiagonalMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype == "full");
  const Alat::Matrix<Alat::DoubleVector>* fullmatrix = &variablematrix->getFullMatrix();
  int ncomp = variablematrix->getNComponents();
  for(int i = 0; i < getN(); i++)
  {
    double diag = 0.0, sum = 0.0;
    for(int icomp = 0; icomp < ncomp; icomp++)
    {
      for(int jcomp = 0; jcomp < ncomp; jcomp++)
      {
        double d = ( *fullmatrix )(icomp, jcomp)[i];
        sum += d*d;
        if(jcomp == icomp)
        {
          diag += fabs(d);
        }
      }
    }
    _simplematrix[i] = diag/sum;
  }
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiDiagonalDiagonal(double relax, const Alat::VariableDiagonalMatrix* variablematrix)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void VariableDiagonalMatrix::_computeSpaiDiagonalFull(double relax, const Alat::VariableDiagonalMatrix* variablematrix)
{
  assert(0);
}
