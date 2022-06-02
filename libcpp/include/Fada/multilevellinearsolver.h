#ifndef __Fada_MultiLevelLinearSolver_h
#define __Fada_MultiLevelLinearSolver_h

#include  "Alat/ghostlinearsolver.h"
#include  "Fada/multilevelsolverinterface.h"
#include  "Alat/stringvector.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DomainSolverInterface;
}
namespace Fada
{
  class MultiLevelLinearSolver : public Alat::Vector<Alat::LinearSolverInterface*>, public Fada::MultiLevelSolverInterface
  {
protected:
    std::string _coarsesolver, _linearsolvername, _blockname;
    int _nmaxforcoarsesolver;
    Alat::GhostLinearSolver _linearsolver;
    Alat::StringVector _variables;
    void readParameters(const Alat::ParameterFile* parameterfile);

public:
    ~MultiLevelLinearSolver();
    MultiLevelLinearSolver(int nlevels, const Alat::GhostLinearSolver& linearsolver);
    // MultiLevelLinearSolver( const MultiLevelLinearSolver& multilevellinearsolver);
    MultiLevelLinearSolver& operator=( const MultiLevelLinearSolver& multilevellinearsolver);
    std::string getName() const;
    MultiLevelLinearSolver* clone() const;

    void reInit();
    void reInit(const Alat::ParameterFile* parameterfile, const Alat::GhostMatrix& ghostmatrix, Fada::DomainSolverInterface* solver);
    void compute();

    int getNLevels() const;
    Alat::LinearSolverInterface*& getLinearSolverPointer(int level);
    Alat::LinearSolverInterface* getLinearSolver(int level);
    const Alat::LinearSolverInterface* getLinearSolver(int level) const;
    void setLinearSolverPointer(int level, Alat::LinearSolverInterface* solver);
    bool allSolversSet() const;
    bool hasIterationInfo() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
