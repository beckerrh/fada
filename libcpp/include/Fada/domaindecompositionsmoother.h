#ifndef __Fada_DomainDecompositionSmoother_h
#define __Fada_DomainDecompositionSmoother_h

#include  "Alat/doublevector.h"
#include  "Alat/matrix.h"
#include  "Alat/sparsitypatternwithrowindex.h"
#include  "Alat/vector.h"
#include  "Alat/fixarray.h"
#include  "Alat/preconditionerwithsorting.h"
#include  "Alat/umfmatrixbase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class VectorInterface;
}

namespace Fada
{
  class MeshInterface;
  class ParameterFile;
}

namespace Fada
{
  class DomainDecompositionSmoother : public Alat::PreconditionerWithSorting
  {
private:
  int _ncomp;
  int _patchsize;
  Alat::IntVector _patchsizes;
  Alat::Vector<Alat::Vector<Alat::FixArray<3, int> > > _newpos;
  Alat::Vector<Alat::UmfMatrixBase> _umf;
  mutable Alat::Vector<Alat::DoubleVector> _upatch, _fpatch;
  mutable Alat::DoubleVector _u, _f;

public:
    ~DomainDecompositionSmoother();
    DomainDecompositionSmoother(const Alat::VariableMatrixInterface* matrix, int patchsize);
    DomainDecompositionSmoother( const DomainDecompositionSmoother& domaindecompositionsmoother);
    DomainDecompositionSmoother& operator=( const DomainDecompositionSmoother& domaindecompositionsmoother);
    std::string getName() const;
    DomainDecompositionSmoother* clone() const;
    
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void reInit();
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
