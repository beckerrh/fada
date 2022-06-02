#ifndef __Alat_Preconditioner_h
#define __Alat_Preconditioner_h

#include  "vector.h"
#include  "ghostvector.h"
#include  "preconditionerinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class Preconditioner : public virtual PreconditionerInterface
  {
private:
    int _id;
    static int _totalids;

protected:
    mutable Alat::Vector<Alat::GhostVector> _memory;
    Alat::IterativeSolverVisitorInterface* _visitor;
    std::string _smoothtype;
    Alat::GhostVector& getMemory(int i) const;

public:
    ~Preconditioner();
    Preconditioner();
    Preconditioner( const Preconditioner& preconditioner);
    Preconditioner& operator=( const Preconditioner& preconditioner);
    std::string getName() const;
    Preconditioner* clone() const;

    Alat::IterativeSolverVisitorInterface* getVisitor();
    const Alat::IterativeSolverVisitorInterface* getVisitor() const;
    int getNVectors() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void memory();
    void setsmoothtype(std::string smoothtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
