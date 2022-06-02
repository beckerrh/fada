#ifndef __Alat_GaussSeidelVariableDiagonalMatrix_h
#define __Alat_GaussSeidelVariableDiagonalMatrix_h

#include  "variablediagonalmatrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VariableMatrix;

  class GaussSeidelVariableDiagonalMatrix : public Alat::VariableDiagonalMatrix
  {
protected:
    const Alat::VariableMatrix* _variablematrix;

public:
    ~GaussSeidelVariableDiagonalMatrix();
    GaussSeidelVariableDiagonalMatrix(int ncomp, std::string type = "full");
    GaussSeidelVariableDiagonalMatrix( const GaussSeidelVariableDiagonalMatrix& variablematrix);
    GaussSeidelVariableDiagonalMatrix& operator=( const GaussSeidelVariableDiagonalMatrix& variablematrix);
    std::string getName() const;
    GaussSeidelVariableDiagonalMatrix* clone() const;
    void computeGaussSeidel(double relax, const Alat::VariableMatrixInterface* variablematrix);
    void gaussSeidel(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const;
  };
  std::ostream& operator<<(std::ostream& s, const GaussSeidelVariableDiagonalMatrix& A);
}

/*--------------------------------------------------------------------------*/

#endif
