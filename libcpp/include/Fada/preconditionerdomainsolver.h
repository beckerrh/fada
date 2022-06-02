#ifndef __Fada_PreconditionerDomainSolver_h
#define __Fada_PreconditionerDomainSolver_h

#include  "Alat/preconditionerwithsystemmatrix.h"
#include  "Alat/intvector.h"
#include  "Alat/set.h"
#include  "Alat/umfsolversomevariables.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class MatrixAllVariables;
  class VisitorDomainSolverOneLevel;
}

namespace Fada
{
  class PreconditionerDomainSolver : public Alat::PreconditionerWithSystemMatrix
  {
protected:
    const Alat::SystemMatrixInterface* _systemmatrix;
    const Fada::VisitorDomainSolverOneLevel* _visitordsol;
    const Alat::StringVector& _variables;
                                     
    std::string _type;
    Alat::Vector<Alat::PreconditionerInterface*> _preconditioners;
    Alat::Vector<Alat::VariableMatrixInterface*> _schurmatrices;
    Alat::IntMap _ischur;
    Alat::Map<int,std::string> _ivarschur, _kvarschur;
    
public:
    ~PreconditionerDomainSolver();
    PreconditionerDomainSolver(const Alat::SystemMatrixInterface* systemmatrix, const Alat::StringVector& variables);
    PreconditionerDomainSolver( const PreconditionerDomainSolver& preconditionerdirectpervariables);
    PreconditionerDomainSolver& operator=( const PreconditionerDomainSolver& preconditionerdirectpervariables);
    std::string getName() const;
    PreconditionerDomainSolver* clone() const;

    int getNVectors() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void reInit();
    void computePreconditioner();
    void _solveApproximate(AlatEnums::iterationstatus& status, Alat::SystemVectorInterface* u, const Alat::SystemVectorInterface* f, int iteration) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
