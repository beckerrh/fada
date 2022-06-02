#ifndef __Alat_VariableMatrixWithRowIndex_h
#define __Alat_VariableMatrixWithRowIndex_h

#include  "variablematrixwithrowindexinterface.h"
#include  "sparsitypatternwithrowindex.h"
#include  "vector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternWithRowIndexSoft;
}
namespace Alat
{
  class VariableMatrixWithRowIndex : public Alat::VariableMatrixWithRowIndexInterface
  {
  protected:
    Alat::SparsityPatternWithRowIndex _sparsitypattern;
    int _ncomp, _mcomp, _nmcomp;
    // Alat::Matrix<Alat::DoubleVector> _fullmatrix;
    Alat::DoubleVector _fullmatrix;

public:
    ~VariableMatrixWithRowIndex();
    VariableMatrixWithRowIndex(int ncomp, int mcomp);
    VariableMatrixWithRowIndex( const VariableMatrixWithRowIndex& sparsefixmatrix);
    VariableMatrixWithRowIndex& operator=( const VariableMatrixWithRowIndex& sparsefixmatrix);
    std::string getName() const;

    Alat::SparsityPatternWithRowIndex* getSparsityPatternWithRowIndex();
    const Alat::SparsityPatternWithRowIndex* getSparsityPatternWithRowIndex() const;

    const double& getEntry(int pos, int icomp, int jcomp) const;
    void zeros();
    void set_size(const Alat::SparsityPatternWithRowIndexSoft& sparsitypattern);
    void set_size(const Alat::SparsityPatternWithRowIndex& sparsitypattern);
    void matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;

    const double& getValue(int icomp, int jcomp, int pos) const;
    void assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej);
    // void assembleCell(int iKL, int iKR, const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, double scale);
    // void assembleCell(int iK, const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, double scale);
    int getNComponents() const;
    int getMComponents() const;
    std::ostream& write(std::ostream& os) const;
    void addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const;
    void addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const;
  };
  std::ostream& operator<<(std::ostream& s, const VariableMatrixWithRowIndex& A);
}

/*--------------------------------------------------------------------------*/

#endif
