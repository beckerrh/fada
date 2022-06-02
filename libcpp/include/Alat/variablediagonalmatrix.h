#ifndef __Alat_VariableDiagonalMatrix_h
#define __Alat_VariableDiagonalMatrix_h

#include  "sparsitypattern.h"
#include  "doublevector.h"
#include  "matrix.h"
#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VariableMatrix;

  class VariableDiagonalMatrix : public Alat::VariableMatrixInterface
  {
protected:
    std::string _type;
    int _ncomp, _mcomp;
    Alat::DoubleVector _simplematrix;
    Alat::Vector<Alat::DoubleVector> _diagonalmatrix;
    Alat::Matrix<Alat::DoubleVector> _fullmatrix;
    void _computeSpaiSimple(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiDiagonal(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiFull(double relax, const Alat::VariableMatrix* variablematrix);
    void _computeSpaiDiagonalSimple(double relax, const Alat::VariableDiagonalMatrix* variablematrix);
    void _computeSpaiDiagonalDiagonal(double relax, const Alat::VariableDiagonalMatrix* variablematrix);
    void _computeSpaiDiagonalFull(double relax, const Alat::VariableDiagonalMatrix* variablematrix);

public:
    ~VariableDiagonalMatrix();
    VariableDiagonalMatrix(int ncomp, int mcomp, std::string type = "full");
    VariableDiagonalMatrix( const VariableDiagonalMatrix& variablematrix);
    VariableDiagonalMatrix& operator=( const VariableDiagonalMatrix& variablematrix);
    std::string getName() const;
    VariableDiagonalMatrix* clone() const;

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
    const double& getValue(int icomp, int jcomp, int pos) const;
    double& getValue(int icomp, int jcomp, int pos);
    bool greaterThan(int pos1, int pos2) const;

    void set_size(int ntotal);
    // void set_size(const FadaMesh::MeshInterface* mesh, const Fada::DofInformationInterface* dofinformationi, const Fada::DofInformationInterface* dofinformationj);
    void zeros();
    std::ostream& write(std::ostream& os) const; 
    void matrixVectorProduct(Alat::VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d = 1.0) const;
    void addMatrix(const Alat::VariableMatrixInterface* sparsematrix, double d = 1.0);
    void addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const;
    void addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const;
    void assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej);
    Alat::VariableMatrixInterface* newSchurComplementMatrix(const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, std::string type, std::string storage) const;
    void computeSchurComplement(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D);
    void reInitSchurMatrix(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil = false);
    void zeroLine(int index, int comp);
    void setElement(int index, int compi, int compj, double d);

    Alat::VariableMatrixInterface* newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const Alat::VariableMatrixInterface* matrix) const;
    void computeSpai(double relax, const Alat::VariableMatrixInterface* variablematrix);
  };
  std::ostream& operator<<(std::ostream& s, const VariableDiagonalMatrix& A);
}

/*--------------------------------------------------------------------------*/

#endif
