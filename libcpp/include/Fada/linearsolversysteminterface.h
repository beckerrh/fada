#ifndef __Fada_LinearSolverSystemInterface_h
#define __Fada_LinearSolverSystemInterface_h

#include  "Alat/enums.h"
#include  "Alat/linearsolverinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class GhostMatrix;
  class GhostVector;
  class IntVector;
  class ParameterFile;
}
namespace Alat
{
  class IterationInfo;  
}
namespace Fada
{
  class LinearSolverSystemInterface : public virtual Alat::LinearSolverInterface
  {
protected:
    std::string getInterfaceName() const;

public:
    ~LinearSolverSystemInterface();
    LinearSolverSystemInterface();
    LinearSolverSystemInterface( const LinearSolverSystemInterface& linearsolverinterface);
    LinearSolverSystemInterface& operator=( const LinearSolverSystemInterface& linearsolverinterface);

    virtual void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
