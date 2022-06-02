#ifndef  __Alat_SparseMatrix_h
#define  __Alat_SparseMatrix_h

#include  "doublevector.h"
#include  "sparsitypattern.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class DoubleMatrix;
}
namespace Alat
{
  class VariableVector;
}

namespace Alat
{
  class VariableVector;

  class SparseMatrix
  {
protected:
    Alat::SparsityPattern _sparsitypattern;
    Alat::DoubleVector _value;

public:
    ~SparseMatrix();
    SparseMatrix();
    SparseMatrix(const SparseMatrix& S);
    SparseMatrix& operator=(const SparseMatrix& S);
    std::string getName() const;
    void read(std::string filename);
    void write(std::string filename, std::string datatype = "binary") const;
    std::ostream& write(std::ostream& os) const;

    Alat::SparsityPattern* getSparsityPattern();
    const Alat::SparsityPattern* getSparsityPattern() const;
    const Alat::DoubleVector&  getValue() const;
    Alat::DoubleVector&  getValue();
    const double& getValue(int pos) const;
    double& getValue(int pos);
    void zeros();

    void set_size(int n, int nt);
    void set_size(const Alat::SparsityPattern& sparsitypattern);
    void set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft);
    void set_size(const Alat::SparseMatrix& matrix);

    void productVector(std::vector<double>::iterator fi, std::vector<double>::const_iterator ui, double d = 1.0) const;
    void productVectorTransposed(std::vector<double>::iterator fi, std::vector<double>::const_iterator ui, double d = 1.0) const;
    void productVector(Alat::DoubleVector& y, const Alat::DoubleVector& x, double d = 1.) const;
    void productVectorTransposed(Alat::DoubleVector& y, const Alat::DoubleVector& x, double d = 1.) const;


    void zeroLine(int i);
    void assemble(const Alat::IntVector& ind, const Alat::DoubleMatrix& Aloc, double coef);
    void assemble(const Alat::IntVector& ind, const Alat::IntVector& jnd, const Alat::DoubleMatrix& Aloc, double coef);

    double& operator() (int i, int j);
    bool greaterThan(int pos1, int pos2) const;
    const double& getEntry(int pos) const;
    double& getEntry(int pos);
    void suppressZeros(double epsilon = 1.0e-14);
  };
  std::ostream& operator<<( std::ostream& os, const SparseMatrix& matrix );
}

#endif
