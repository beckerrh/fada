#ifndef __Alat_IluVariableMatrix_h
#define __Alat_IluVariableMatrix_h

#include  "variablematrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class PreconditionerWithSorting;
  class VariableMatrix;

  class IluVariableMatrix : public VariableMatrix
  {
protected:
    const Alat::PreconditionerWithSorting* _preconditionerwithsorting;
    const Alat::VariableMatrix* _variablematrix;
    Alat::IntVector _diag;

    mutable Alat::DoubleVector _h;
    mutable Alat::Vector<Alat::DoubleVector> _hfull;

    void _backward(int icomp, Alat::DoubleVector& x) const;
    void _forward (int icomp, Alat::DoubleVector& x) const;
    void _backwardfull(Alat::Vector<Alat::DoubleVector>& x) const;
    void _forwardfull (Alat::Vector<Alat::DoubleVector>& x) const;
    void _copy_entries(const Alat::IntVector& p, const Alat::IntVector& pinv);
    void _modify(double ilum);

public:
    ~IluVariableMatrix();
    IluVariableMatrix(int ncomp, const Alat::PreconditionerWithSorting* preconditionerwithsorting, std::string type = "full");
    IluVariableMatrix( const IluVariableMatrix& variablematrix);
    IluVariableMatrix& operator=( const IluVariableMatrix& variablematrix);
    std::string getName() const;
    IluVariableMatrix* clone() const;
    void set_size(int n, int ntotal);
    void computeIlu(double relax, const Alat::VariableMatrixInterface* variablematrix, const Alat::IntVector& p, const Alat::IntVector& pinv);
    void ilu(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const;
  };
  std::ostream& operator<<(std::ostream& s, const IluVariableMatrix& A);
}

/*--------------------------------------------------------------------------*/

#endif
