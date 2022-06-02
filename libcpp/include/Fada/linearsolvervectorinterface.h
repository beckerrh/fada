#ifndef __Fada_LinearSolverVectorInterface_h
#define __Fada_LinearSolverVectorInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostMatrix;
  class GhostVector;
  class ParameterFile;
}
namespace Alat
{
  class LinearSolverInterface;
}
namespace Fada
{
  class MeshInterface;
  class MultiLevelMatrix;
  class SolverInterface;

  class LinearSolverVectorInterface : public virtual Alat::InterfaceBase
  {
public:
    ~LinearSolverVectorInterface();
    LinearSolverVectorInterface();
    LinearSolverVectorInterface( const LinearSolverVectorInterface& multilevelsolverinterface);
    LinearSolverVectorInterface& operator=( const LinearSolverVectorInterface& multilevelsolverinterface);
    std::string getInterfaceName() const;
    std::string getName() const;
    LinearSolverVectorInterface* clone() const;
    virtual int getNLevels() const = 0;
    virtual Alat::LinearSolverInterface*& getLinearSolverPointer(int level) = 0;
    virtual Alat::LinearSolverInterface* getLinearSolver(int level) = 0;
    virtual const Alat::LinearSolverInterface* getLinearSolver(int level) const = 0;
    virtual bool allSolversSet() const = 0;
    virtual void reInit() = 0;
    virtual void compute() = 0;
    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    // virtual void restart();
  };
}

/*--------------------------------------------------------------------------*/

#endif
