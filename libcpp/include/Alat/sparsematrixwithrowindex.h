#ifndef  __Alat_SparseMatrixWithRowIndex_h
#define  __Alat_SparseMatrixWithRowIndex_h

#include  "doublevector.h"
#include  "sparsitypatternwithrowindex.h"
#include  "sparsitypatternwithvectorofset.h"

/*-----------------------------------------------------------------*/

namespace Alat
{
  class SparseMatrixWithRowIndex
  {
protected:
    Alat::SparsityPatternWithRowIndex _sparsitypattern;
    Alat::DoubleVector _value;

public:
    ~SparseMatrixWithRowIndex();
    SparseMatrixWithRowIndex();
    SparseMatrixWithRowIndex(const SparseMatrixWithRowIndex& S);
    SparseMatrixWithRowIndex& operator=(const SparseMatrixWithRowIndex& S);
    std::string getName() const;

    void clear();
    std::ostream& write(std::ostream& os) const;

    void write(const std::string& basename, std::string datatype = "binary") const;
    void read(const std::string& basename);

    Alat::SparsityPatternWithRowIndex* getSparsityPatternWithRowIndex();
    const Alat::SparsityPatternWithRowIndex* getSparsityPatternWithRowIndex() const;
    const Alat::DoubleVector&  getValue() const;
    Alat::DoubleVector&  getValue();
    const double& getValue(int pos) const;
    double& getValue(int pos);
    void zeros();

    void set_size(const Alat::SparsityPatternWithRowIndexSoft& SP);
    void set_size(const Alat::SparsityPatternWithVectorOfSet& SP);
  };
  std::ostream& operator<<( std::ostream& s, const SparseMatrixWithRowIndex& A );
}

#endif
