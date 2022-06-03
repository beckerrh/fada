#ifndef __Alat_PreconditionerWithVariableMatrix_h
#define __Alat_PreconditionerWithVariableMatrix_h

#include  "preconditioner.h"
#include  "iterativesolvervisitorwithvectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{ 
  class VariableMatrixInterface;
  class VariableVectorInterface;
  
  class PreconditionerWithVariableMatrix : public Alat::Preconditioner
  {
protected:
    Alat::SystemVector* getMemory(int i) const;
    const Alat::VariableMatrixInterface* _matrix;
    const Alat::IterativeSolverVisitorWithVectorInterface* _visitordomainsolver;

public:
    ~PreconditionerWithVariableMatrix();
    PreconditionerWithVariableMatrix(const Alat::VariableMatrixInterface* matrix);
    PreconditionerWithVariableMatrix( const PreconditionerWithVariableMatrix& preconditionerwithmatrix);
    PreconditionerWithVariableMatrix& operator=( const PreconditionerWithVariableMatrix& preconditionerwithmatrix);
    std::string getName() const;
    PreconditionerWithVariableMatrix* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    virtual void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
