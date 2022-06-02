#ifndef __Alat_IterativeSolverWithPreconditioner_h
#define __Alat_IterativeSolverWithPreconditioner_h

#include  "iterativesolverwithvisitor.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class PreconditionerInterface;
  
  class IterativeSolverWithPreconditioner : public virtual IterativeSolverWithVisitor
  {
private:
    PreconditionerInterface* _preconditioner;
    bool _deletepreconditioner;

protected:
    mutable AlatEnums::iterationstatus _statuspreconditioner;

public:
    ~IterativeSolverWithPreconditioner();
    IterativeSolverWithPreconditioner();
    IterativeSolverWithPreconditioner( const IterativeSolverWithPreconditioner& iterativesolverwithpreconditioner);
    IterativeSolverWithPreconditioner& operator=( const IterativeSolverWithPreconditioner& iterativesolverwithpreconditioner);
    std::string getName() const;
    Alat::IterativeSolverWithPreconditioner*  clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    std::ostream& printLoopInformation(std::ostream& os) const;
    void reInit();
    void compute();
 
    Alat::PreconditionerInterface*& newPreconditionerPointer();
    void setPreconditionerPointer(Alat::PreconditionerInterface* preconditioner);
    const Alat::PreconditionerInterface* getPreconditioner() const;
    Alat::PreconditionerInterface* getPreconditioner();
    void addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
