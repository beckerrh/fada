#ifndef __Fada_LinearSolverVector_h
#define __Fada_LinearSolverVector_h

#include  "Alat/vector.h"
#include  "Alat/linearsolverinterface.h"
#include  "linearsolvervectorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LinearSolverVector : public LinearSolverVectorInterface, public Alat::Vector<Alat::LinearSolverInterface*>
  {
public:
    ~LinearSolverVector();
    LinearSolverVector();
    LinearSolverVector( const LinearSolverVector& linearsolvervector);
    LinearSolverVector& operator=( const LinearSolverVector& linearsolvervector);
    std::string getName() const;
    LinearSolverVector* clone() const;

    int getNLevels() const;
    Alat::LinearSolverInterface*& getLinearSolverPointer(int level);
    Alat::LinearSolverInterface* getLinearSolver(int level);
    const Alat::LinearSolverInterface* getLinearSolver(int level) const;
    bool allSolversSet() const;
    void reInit(int nlevels);
    void reInit();
    void compute();
    std::ostream& printLoopInformation(std::ostream& os) const;
    void restart();
  };
}

/*--------------------------------------------------------------------------*/

#endif
