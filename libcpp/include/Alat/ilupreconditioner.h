#ifndef __Alat_IluPreconditioner_h
#define __Alat_IluPreconditioner_h

#include  "preconditionerwithsorting.h"
#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VectorInterface;
}

namespace Alat
{
  class IluPreconditioner : public Alat::PreconditionerWithSorting
  {
protected:
    Alat::VariableMatrixInterface* _matrixilu;
    std::string _type;
    double _relax;

public:
    ~IluPreconditioner();
    IluPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax);
    IluPreconditioner( const IluPreconditioner& spaismoother);
    IluPreconditioner& operator=( const IluPreconditioner& spaismoother);
    std::string getName() const;
    IluPreconditioner* clone() const;

    int getNVectors() const;
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const;
    void reInit();
  };
}

/*--------------------------------------------------------------------------*/

#endif
