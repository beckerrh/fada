#ifndef __Fada_MultiLevelSolverInterface_h
#define __Fada_MultiLevelSolverInterface_h

#include  "Alat/linearsolverinterface.h"
#include  "linearsolvervectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DomainSolverInterface;
  class GhostMatrix;
  class MeshInterface;
  class MultiLevelMatrix;
  class ParameterFile;

  class MultiLevelSolverInterface : public virtual Alat::LinearSolverInterface, public virtual Fada::LinearSolverVectorInterface
  {
public:
    virtual ~MultiLevelSolverInterface();
    MultiLevelSolverInterface();
    MultiLevelSolverInterface( const MultiLevelSolverInterface& multilevelsolverinterface);
    MultiLevelSolverInterface& operator=( const MultiLevelSolverInterface& multilevelsolverinterface);
    std::string getInterfaceName() const;
    std::string getName() const;
    MultiLevelSolverInterface* clone() const;
    virtual int getNLevels() const = 0;
    virtual Alat::LinearSolverInterface*& getLinearSolverPointer(int level) = 0;
    virtual Alat::LinearSolverInterface* getLinearSolver(int level) = 0;
    virtual const Alat::LinearSolverInterface* getLinearSolver(int level) const = 0;
    virtual bool allSolversSet() const = 0;
    virtual void reInit() = 0;
    virtual void compute() = 0;
    virtual void reInit(const Alat::ParameterFile* parameterfile, const Alat::GhostMatrix& ghostmatrix, Fada::DomainSolverInterface* solver);
  };
}

/*--------------------------------------------------------------------------*/

#endif
