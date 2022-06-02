#ifndef __Alat_VariableMatrix_h
#define __Alat_VariableMatrix_h

#include  "sparsitypattern.h"
#include  "variablematrixinterface.h"
#include  "doublevector.h"
#include  "matrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class PreconditionerInterface;
}
namespace Alat
{
  class VariableMatrixWithRowIndexInterface;
  class VariableDiagonalMatrix;

  class VariableMatrix : public Alat::VariableMatrixInterface
  {
protected:
    std::string _type;
    int _ncomp, _mcomp;
    Alat::DoubleVector _simplematrix;
    Alat::Vector<Alat::DoubleVector> _diagonalmatrix;
    Alat::Matrix<Alat::DoubleVector> _fullmatrix;
    Alat::SparsityPattern _sparsitypattern;
    void _addMatrix(const Alat::VariableMatrixWithRowIndexInterface* sparsematrixwri, double d);
    void _addMatrix(const VariableDiagonalMatrix* diagonalmatrix, double d);
    void _addMatrix(const VariableMatrix* variablematrix, double d);

public:
    ~VariableMatrix();
    VariableMatrix(int ncomp=1, int mcomp=1, std::string type = "simple");
    VariableMatrix( const VariableMatrix& variablematrix);
    VariableMatrix& operator=( const VariableMatrix& variablematrix);
    std::string getName() const;
    VariableMatrix* clone() const;

    std::string getType() const;
    const Alat::DoubleVector& getSimpleMatrix() const;
    const Alat::DoubleVector& getDiagonalMatrix(int icomp) const;
    const Alat::DoubleVector& getFullMatrix(int icomp, int jcomp) const;
    Alat::DoubleVector& getSimpleMatrix();
    Alat::DoubleVector& getDiagonalMatrix(int icomp);
    Alat::DoubleVector& getFullMatrix(int icomp, int jcomp);
    const Alat::Vector<Alat::DoubleVector>& getDiagonalMatrix() const;
    const Alat::Matrix<Alat::DoubleVector>& getFullMatrix() const;
    int getN() const;
    int getNComponents() const;
    int getMComponents() const;
    const double& getValueSimple(int pos) const;
    const double& getValue(int icomp, int jcomp, int pos) const;
    double& getValue(int icomp, int jcomp, int pos);
    Alat::SparsityPattern* getSparsityPattern();
    const Alat::SparsityPattern* getSparsityPattern() const;
    bool greaterThan(int pos1, int pos2) const;

    void set_size(int n, int ntotal);
    void set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft);
    void zeros();
    std::ostream& write(std::ostream& os) const;
    void matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;
    void matrixVectorProductTransposed(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;
    void addMatrix(const Alat::VariableMatrixInterface* sparsematrix, double d = 1.0);
    void addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const;
    void addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const;
    void assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej);    
    Alat::VariableMatrixInterface* newSchurComplementMatrix(const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, std::string type, std::string storage) const;
    void computeSchurSparsityPatternSoft(Alat::SparsityPatternSoft& sparsitypatternsoft, const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil = true) const;
    void computeSchurComplement(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D);
    void reInitSchurMatrix(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil = false);
    void zeroLine(int index, int comp);
    void setElement(int index, int compi, int compj, double d);
    void suppressZeros(double epsilon = 1.0e-14);    

    Alat::VariableMatrixInterface* newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const Alat::VariableMatrixInterface* matrix) const;
    Alat::VariableMatrixInterface* newGaussSeidelMatrixAndReinit(int ncomp, std::string type, std::string storage) const;
    Alat::VariableMatrixInterface* newIluMatrixAndReinit(int ncomp, const Alat::PreconditionerInterface* preconditioner, std::string type, std::string storage) const;
  };
  std::ostream& operator<<(std::ostream& s, const VariableMatrix& A);
}

/*--------------------------------------------------------------------------*/

#endif
