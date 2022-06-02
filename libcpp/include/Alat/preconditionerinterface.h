#ifndef __Alat_PreconditionerInterface_h
#define __Alat_PreconditionerInterface_h

#include  "enums.h"
#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class GhostMatrix;
  class GhostVector;
  class IntVector;
  class ParameterFile;
  class StringVector;
}
namespace Alat
{
  class IterativeSolverVisitorInterface;

  class PreconditionerInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~PreconditionerInterface();
    PreconditionerInterface();
    PreconditionerInterface( const PreconditionerInterface& vectorinterface);
    PreconditionerInterface& operator=( const PreconditionerInterface& vectorinterface);
    std::string getName() const;
    virtual std::ostream& printLoopInformation(std::ostream& os) const;

    virtual Alat::IterativeSolverVisitorInterface* getVisitor();
    virtual const Alat::IterativeSolverVisitorInterface* getVisitor() const;
    virtual int getNVectors() const = 0;
    virtual void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    virtual void reInit();
    virtual void computePreconditioner();
    virtual void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    virtual std::ostream& write(std::ostream& os) const;
    virtual void zeros() const;
    virtual void setsmoothtype(std::string smoothtype) = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
