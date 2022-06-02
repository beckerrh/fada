#include  "Alat/sparsitypatternwithrowindex.h"
#include  "Alat/systemassemblematrix.h"
// #include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Alat/vector.h"
#include  "Alat/variablematrixwithrowindex.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableMatrixWithRowIndex::~VariableMatrixWithRowIndex(){}
VariableMatrixWithRowIndex::VariableMatrixWithRowIndex(int ncomp, int mcomp) : Alat::VariableMatrixWithRowIndexInterface(), _ncomp(ncomp), _mcomp(mcomp), _nmcomp(ncomp*mcomp){}
VariableMatrixWithRowIndex::VariableMatrixWithRowIndex( const VariableMatrixWithRowIndex& sparsefixmatrix) : Alat::VariableMatrixWithRowIndexInterface(sparsefixmatrix)
{
  assert(0);
}
VariableMatrixWithRowIndex& VariableMatrixWithRowIndex::operator=( const VariableMatrixWithRowIndex& sparsefixmatrix)
{
  Alat::VariableMatrixWithRowIndexInterface::operator=(sparsefixmatrix);
  assert(0);
  return *this;
}
std::string VariableMatrixWithRowIndex::getName() const
{
  return "VariableMatrixWithRowIndex";
}

/*--------------------------------------------------------------------------*/
int VariableMatrixWithRowIndex::getNComponents() const
{
  return _ncomp;
}
int VariableMatrixWithRowIndex::getMComponents() const
{
  return _mcomp;
}
Alat::SparsityPatternWithRowIndex* VariableMatrixWithRowIndex::getSparsityPatternWithRowIndex()
{
  return &_sparsitypattern;
}
const Alat::SparsityPatternWithRowIndex* VariableMatrixWithRowIndex::getSparsityPatternWithRowIndex() const
{
  return &_sparsitypattern;
}

/*--------------------------------------------------------------------------*/

const double& VariableMatrixWithRowIndex::getEntry(int pos, int icomp, int jcomp) const
{
  return _fullmatrix[_nmcomp*pos + icomp*_mcomp+ jcomp];
}
const double& VariableMatrixWithRowIndex::getValue(int icomp, int jcomp, int pos) const
{
  return _fullmatrix[_nmcomp*pos + icomp*_mcomp+ jcomp];
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::zeros()
{
  _fullmatrix.zeros();
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::set_size(const Alat::SparsityPatternWithRowIndexSoft& sparsitypattern)
{
  getSparsityPatternWithRowIndex()->set_size(sparsitypattern);
  _fullmatrix.set_size(sparsitypattern.ntotal()*_nmcomp);
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::set_size(const Alat::SparsityPatternWithRowIndex& sparsitypattern)
{
  getSparsityPatternWithRowIndex()->set_size(sparsitypattern);
  _fullmatrix.set_size(sparsitypattern.ntotal()*_nmcomp);
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  Alat::VariableVector* vf = dynamic_cast<Alat::VariableVector*>( f );
  assert(vf);
  const Alat::VariableVector* vu = dynamic_cast<const Alat::VariableVector*>( u );
  assert(vu);
  const Alat::SparsityPatternWithRowIndex* sparsitypattern =  getSparsityPatternWithRowIndex();
  int n = sparsitypattern->n();
  for(int k = 0; k < n; k++)
  {
    int i = sparsitypattern->row(k);
    for(int pos = sparsitypattern->rowstart(k); pos < sparsitypattern->rowstop(k); pos++)
    {
      int j = sparsitypattern->col(pos);
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          (*vf)(icomp,i)+= d*getValue(icomp, jcomp, pos)*(*vu)(jcomp,j);
        }
      }
    }
  }
}
/*--------------------------------------------------------------------------*/
void VariableMatrixWithRowIndex::assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej)
{
  const Alat::SparsityPatternWithRowIndex* SP =  getSparsityPatternWithRowIndex();
  assert( _ncomp == Aloc.ncomp() );
  assert( _mcomp == Aloc.mcomp() );

  for(int ii = 0; ii < indices.size(); ii++)
  {
    int i = indices[ii];
    int k = SP->rowinverse(i);
    for(int jj = 0; jj < jndices.size(); jj++)
    {
      int j = jndices[jj];
      bool found = false;
      for(int pos = SP->rowstart(k); pos < SP->rowstop(k); pos++)
      {
        if(SP->col(pos) == j)
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < _mcomp; jcomp++)
            {
              double scale = scalej[jcomp]/scalei[icomp];
              // std::cerr << "adding " << icomp << "," <<jcomp << " in " << icompglobal << ";" << jcompglobal << " -> " << Aloc(ivar, jvar) (icomp, jcomp, ii, jj) << "\n";
              // _fullmatrix(icomp, jcomp)[pos] += Aloc(icomp, jcomp, ii, jj);
              _fullmatrix[_nmcomp*pos + icomp*_mcomp+ jcomp] += scale*Aloc(icomp, jcomp, ii, jj);
            }
          }
          // std::cerr << "found\n";
          found = true;
          continue;
        }
      }
      if(not found)
      {
        std::cerr << " did not find " << j << " in row " << i << " (k) " << k << "\n";
        _error_string("addMatrixForDirectSolver", "did not find entry" );
        assert(0);
      }
    }
  }  
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const
{
  const Alat::SparsityPatternWithRowIndex* SP =  getSparsityPatternWithRowIndex();
  for(int k = 0; k < SP->n(); k++)
  {
    int i = SP->row(k);
    for(int pos = SP->rowstart(k); pos < SP->rowstop(k); pos++)
    {
      int j = SP->col(pos);
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
              matrixvalues[pos2] +=  _fullmatrix[_nmcomp*pos + icomp*_mcomp+ jcomp];
              // matrixvalues[pos2] +=  _fullmatrix(icomp, jcomp)[pos];
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/

void VariableMatrixWithRowIndex::addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  const Alat::SparsityPatternWithRowIndex* SP =  getSparsityPatternWithRowIndex();
  for(int k = 0; k < SP->n(); k++)
  {
    int i = SP->row(k);
    for(int pos = SP->rowstart(k); pos < SP->rowstop(k); pos++)
    {
      int j = SP->col(pos);
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        int index = offsetivar + i*_ncomp + icomp;
        assert( index < sparsitypatternsoft.size() );
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          int jndex = offsetjvar + j*_mcomp + jcomp;
          sparsitypatternsoft[index].insert(jndex);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
std::ostream& VariableMatrixWithRowIndex::write(std::ostream& os) const
{
  os << *this;
  return os;
}

std::ostream& Alat::operator<<(std::ostream& s, const VariableMatrixWithRowIndex& A)
{
  const Alat::SparsityPatternWithRowIndex* SP =  A.getSparsityPatternWithRowIndex();
  int n = SP->n();
  for(int k = 0; k < n; k++)
  {
    int i = SP->row(k);
    for(int pos = SP->rowstart(k); pos < SP->rowstop(k); pos++)
    {
      int j = SP->col(pos);
      s << i << "," << j << "\n";
      for(int icomp = 0; icomp < A.getNComponents(); icomp++)
      {
        for(int jcomp = 0; jcomp < A.getMComponents(); jcomp++)
        {
          s << icomp << "," << jcomp << " -> " << A.getValue(icomp, jcomp, pos) << "\n";
        }
      }
    }
  }
  return s;
}
