#ifndef __Alat_GaussSeidelPreconditioner_h
#define __Alat_GaussSeidelPreconditioner_h

#include  "preconditionerwithsorting.h"
#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class VectorInterface;
}

namespace Alat
{
  class GaussSeidelPreconditioner : public Alat::PreconditionerWithSorting
  {
protected:
    Alat::VariableMatrixInterface* _matrixgs;
    std::string _type;
    double _relax;

public:
    ~GaussSeidelPreconditioner();
    GaussSeidelPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax);
    GaussSeidelPreconditioner( const GaussSeidelPreconditioner& spaismoother);
    GaussSeidelPreconditioner& operator=( const GaussSeidelPreconditioner& spaismoother);
    std::string getName() const;
    GaussSeidelPreconditioner* clone() const;

    int getNVectors() const;
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const;
    void reInit();
  };
}

/*--------------------------------------------------------------------------*/

#endif
