#ifndef __Alat_NonlinearSolverInterface_h
#define __Alat_NonlinearSolverInterface_h

#include  "interfacebase.h"
#include  "enums.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class GhostLinearSolver;
  class GhostMatrix;
  class GhostVector;
  class IntVector;
  class ParameterFile;
}

namespace Alat
{
  class IterationInfo;
  class NewtonData;
  class NonlinearSolverVisitorInterface;

  class NonlinearSolverInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~NonlinearSolverInterface();
    NonlinearSolverInterface();
    NonlinearSolverInterface( const NonlinearSolverInterface& linearsolverinterface);
    NonlinearSolverInterface& operator=( const NonlinearSolverInterface& linearsolverinterface);

    virtual Alat::NonlinearSolverVisitorInterface*& newVisitorPointer();
    virtual void setVisitorPointer(Alat::NonlinearSolverVisitorInterface* visitor);

    virtual int getNVectors() const;
    virtual void reInit();
    virtual void solve(Alat::NewtonData& newtondata, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f)=0;
    virtual std::ostream& printLoopInformation(std::ostream& os) const;
    virtual void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    virtual std::ostream& write(std::ostream& os) const;
    virtual const IterationInfo& getIterationInfo() const=0;
    virtual IterationInfo& getIterationInfo()=0;
    virtual double getRhoMatrix() const;
    virtual void addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
