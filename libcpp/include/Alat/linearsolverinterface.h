#ifndef __Alat_LinearSolverInterface_h
#define __Alat_LinearSolverInterface_h

#include  "enums.h"
#include  "interfacebase.h"

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

  class LinearSolverInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~LinearSolverInterface();
    LinearSolverInterface();
    LinearSolverInterface( const LinearSolverInterface& linearsolverinterface);
    LinearSolverInterface& operator=( const LinearSolverInterface& linearsolverinterface);

    virtual int getNVectors() const;
    virtual void reInit();
    virtual void compute();
    virtual void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    virtual std::ostream& printLoopInformation(std::ostream& os) const;
    virtual void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    virtual std::ostream& write(std::ostream& os) const;
    virtual const IterationInfo* getIterationInfo() const;
    virtual IterationInfo* getIterationInfo();
    virtual void addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const;
    virtual bool hasIterationInfo() const = 0;
    virtual void restart();
    virtual void setTolerance(double rtol, double gtol);
    virtual int getNumberOfIterations() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
