#ifndef __Alat_PreconditionerWithSystemMatrix_h
#define __Alat_PreconditionerWithSystemMatrix_h

#include  "preconditioner.h"
#include  "iterativesolvervisitorwithvectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{ 
  class SystemMatrixInterface;
  class SystemVector;
}
namespace Alat
{ 
  class PreconditionerWithSystemMatrix : public Alat::Preconditioner
  {
protected:
    Alat::SystemVector* getMemory(int i) const;
    const Alat::SystemMatrixInterface* _matrix;
    const Alat::IterativeSolverVisitorWithVectorInterface* _visitorsystem;

public:
    ~PreconditionerWithSystemMatrix();
    PreconditionerWithSystemMatrix(const Alat::SystemMatrixInterface* matrix);
    PreconditionerWithSystemMatrix( const PreconditionerWithSystemMatrix& preconditionerwithmatrix);
    PreconditionerWithSystemMatrix& operator=( const PreconditionerWithSystemMatrix& preconditionerwithmatrix);
    std::string getName() const;
    PreconditionerWithSystemMatrix* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    virtual void _solveApproximate(AlatEnums::iterationstatus& status, Alat::SystemVector* u, const Alat::SystemVector* f, int iteration) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
