#ifndef __Alat_SpaiPreconditioner_h
#define __Alat_SpaiPreconditioner_h

#include  "preconditionerwithvariablematrix.h"
#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VectorInterface;
}

namespace Alat
{
  class SpaiPreconditioner : public Alat::PreconditionerWithVariableMatrix
  {
protected:
    Alat::VariableMatrixInterface* _matrixspai;
    std::string _type;
    double _relax;

public:
    ~SpaiPreconditioner();
    SpaiPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax);
    SpaiPreconditioner( const SpaiPreconditioner& spaismoother);
    SpaiPreconditioner& operator=( const SpaiPreconditioner& spaismoother);
    std::string getName() const;
    SpaiPreconditioner* clone() const;

    int getNVectors() const;
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const;
    void reInit();
  };
}

/*--------------------------------------------------------------------------*/

#endif
