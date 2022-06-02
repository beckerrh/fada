#ifndef __Alat_UmfMatrixBase_h
#define __Alat_UmfMatrixBase_h

#include  <string>
#include  "sparsematrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
  class SparsityPattern;
}

namespace Alat
{
  class UmfMatrixBase
  {
private:
    Alat::SparseMatrix _mysparsematrix;

protected:
    double* Control;
    double* Info;
    void* Symbolic, * Numeric;

public:
    ~UmfMatrixBase();
    UmfMatrixBase();
    UmfMatrixBase( const UmfMatrixBase& umfmatrixbase);
    UmfMatrixBase& operator=( const UmfMatrixBase& umfmatrixbase);
    std::string getName() const;
    UmfMatrixBase* clone() const;

    const Alat::SparseMatrix& getSparseMatrix() const;
    Alat::SparseMatrix& getSparseMatrix();

    std::ostream& write(std::ostream& os) const;
    void reInit();
    void computeLu();
    void solve(Alat::DoubleVector& x, const Alat::DoubleVector& b) const;
    void solveTranspose(Alat::DoubleVector& x, const Alat::DoubleVector& b) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
