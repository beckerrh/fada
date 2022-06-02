#ifndef __Alat_IterativeSolverWithVisitor_h
#define __Alat_IterativeSolverWithVisitor_h

#include  "vector.h"
#include  "ghostvector.h"
#include  "iterationinfo.h"
#include  "linearsolverinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IterativeSolverVisitorInterface;

  class IterativeSolverWithVisitor : public virtual LinearSolverInterface
  {
private:
    int _id;
    static int _totalids;
    mutable Alat::Vector<Alat::GhostVector> _memory;
    Alat::IterationInfo _info;
    Alat::IterativeSolverVisitorInterface* _visitor;
    bool _deletevisitor;

protected:
    void memory();
    Alat::GhostVector& getMemory(int i) const;
    bool _basicinitcalled;

public:
    ~IterativeSolverWithVisitor();
    IterativeSolverWithVisitor();
    IterativeSolverWithVisitor( const IterativeSolverWithVisitor& iterativesolverwithvisitor);
    IterativeSolverWithVisitor& operator=( const IterativeSolverWithVisitor& iterativesolverwithvisitor);
    std::string getName() const;
    Alat::IterativeSolverWithVisitor* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    std::ostream& printLoopInformation(std::ostream& os) const;
    Alat::IterativeSolverVisitorInterface*& newVisitorPointer();
    void setVisitorPointer(Alat::IterativeSolverVisitorInterface* visitor);
    const Alat::IterativeSolverVisitorInterface* getVisitor() const;
    Alat::IterativeSolverVisitorInterface* getVisitor();
    const IterationInfo* getIterationInfo() const;
    IterationInfo* getIterationInfo();
    bool hasIterationInfo() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
