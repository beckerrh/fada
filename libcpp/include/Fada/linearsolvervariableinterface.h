#ifndef __Fada_LinearSolverVariableInterface_h
#define __Fada_LinearSolverVariableInterface_h

#include  "Alat/enums.h"
#include  "Alat/linearsolverinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class IntVector;
}
namespace Alat
{
  class VariableMatrixInterface;
  class VariableVectorInterface;
}
namespace Fada
{
  class LinearSolverVariableInterface : public virtual Alat::LinearSolverInterface
  {
protected:
    std::string getInterfaceName() const;

public:
    ~LinearSolverVariableInterface();
    LinearSolverVariableInterface();
    LinearSolverVariableInterface( const LinearSolverVariableInterface& linearsolverinterface);
    LinearSolverVariableInterface& operator=( const LinearSolverVariableInterface& linearsolverinterface);

    virtual void solve(AlatEnums::iterationstatus& status, const Alat::VariableMatrixInterface* A, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
