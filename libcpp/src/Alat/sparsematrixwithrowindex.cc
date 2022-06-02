#include  "Alat/sparsematrixwithrowindex.h"
#include  <fstream>

using namespace Alat;
using namespace std;

/*----------------------------------------------------------*/
SparseMatrixWithRowIndex::~SparseMatrixWithRowIndex() {}
SparseMatrixWithRowIndex::SparseMatrixWithRowIndex() :  _sparsitypattern(), _value() {}
SparseMatrixWithRowIndex::SparseMatrixWithRowIndex(const SparseMatrixWithRowIndex& S) :   _sparsitypattern( *S.getSparsityPatternWithRowIndex() ), _value( S.getValue() )
{
  // assert(0);
}

SparseMatrixWithRowIndex& SparseMatrixWithRowIndex::operator=(const SparseMatrixWithRowIndex& S)
{
  // assert(0);
  return *this;
}

std::string SparseMatrixWithRowIndex::getName() const
{
  return "SparseMatrixWithRowIndex";
}

/*----------------------------------------------------------*/
void SparseMatrixWithRowIndex::clear()
{
  _sparsitypattern.clear();
  _value.clear();
}

/*----------------------------------------------------------*/
std::ostream& Alat::operator<<( std::ostream& s, const SparseMatrixWithRowIndex& A )
{
  A.write(s);
  return s;
}

/*----------------------------------------------------------*/
std::ostream& SparseMatrixWithRowIndex::write(std::ostream& os) const
{
  const Alat::SparsityPatternWithRowIndex* SP =  getSparsityPatternWithRowIndex();
  int n = SP->n();
  for(int ii = 0; ii < n; ii++)
  {
    os << SP->row(ii) << endl;
    for(int pos = SP->rowstart(ii); pos < SP->rowstop(ii); pos++)
    {
      os << SP->col(pos) << ": " << _value[pos] << ", ";
    }
    os << endl;
  }
  return os;
}

/*----------------------------------------------------------*/
void SparseMatrixWithRowIndex::write(const std::string& basename, std::string datatype) const
{
  std::string name = basename + ".fadalightsmwri";
  std::ofstream file( name.c_str() );
  getSparsityPatternWithRowIndex()->saveFada(file, datatype);
  getValue().saveFada(file, datatype);
}

/*----------------------------------------------------------*/
void SparseMatrixWithRowIndex::read(const std::string& basename)
{
  std::string name = basename + ".fadalightsmwri";
  std::ifstream file( name.c_str() );
  getSparsityPatternWithRowIndex()->loadFada(file);
  getValue().loadFada(file);
}

/*----------------------------------------------------------*/
void SparseMatrixWithRowIndex::set_size(const Alat::SparsityPatternWithRowIndexSoft& SP)
{
  _sparsitypattern.set_size(SP);
  // std::cerr << "*** SparseMatrixWithRowIndex::reInit( _sparsitypattern = " << _sparsitypattern << "\n";
  // std::cerr << "*** SparseMatrixWithRowIndex::reInit( SP.ntotal() = " << SP.ntotal() << "\n";
  int ntotal = SP.ntotal();
  _value.set_size(ntotal);
}

/*----------------------------------------------------------*/
void SparseMatrixWithRowIndex::set_size(const Alat::SparsityPatternWithVectorOfSet& SP)
{
  _sparsitypattern.set_size(SP);
}

/*----------------------------------------------------------*/
Alat::SparsityPatternWithRowIndex* SparseMatrixWithRowIndex::getSparsityPatternWithRowIndex()
{
  return &_sparsitypattern;
}

const Alat::SparsityPatternWithRowIndex* SparseMatrixWithRowIndex::getSparsityPatternWithRowIndex() const
{
  return &_sparsitypattern;
}

const Alat::DoubleVector&  SparseMatrixWithRowIndex::getValue() const
{
  return _value;
}

Alat::DoubleVector&  SparseMatrixWithRowIndex::getValue()
{
  return _value;
}

const double& SparseMatrixWithRowIndex::getValue(int pos) const
{
  return _value[pos ];
}

double& SparseMatrixWithRowIndex::getValue(int pos)
{
  return _value[pos ];
}

void SparseMatrixWithRowIndex::zeros()
{
  _value.zeros();
}
