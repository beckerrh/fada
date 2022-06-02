#ifndef __Fada_PreconditionerSolverManagerAllvariables_h
#define __Fada_PreconditionerSolverManagerAllvariables_h

#include  "Alat/stringvector.h"
#include  "Alat/vector.h"
#include  "Alat/ghostlinearsolver.h"
#include  "Alat/ghostmatrix.h"
#include  "Alat/preconditioner.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  template<class S, class T>
  class Map;
}
namespace Alat
{
  class LinearSolverInterface;
}
namespace Fada
{
  class SolverManagerInterface;

  class PreconditionerSolverManagerAllvariables : public Alat::Preconditioner
  {
protected:
    Alat::Vector<Alat::StringVector> _variablenams;
    Alat::StringVector _solvernames;
    const Alat::GhostMatrix _ghostmatrix;
    Fada::SolverManagerInterface* _solvermanager;
    Alat::Vector<Alat::LinearSolverInterface*> _linearsolvers;
    mutable Alat::Vector<Alat::GhostVector> _memory;

public:
    ~PreconditionerSolverManagerAllvariables();
    PreconditionerSolverManagerAllvariables(const Alat::Map<Alat::StringVector, std::string>& solvernames, const Alat::GhostMatrix& ghostmatrix,  Fada::SolverManagerInterface* solvermanager);
    PreconditionerSolverManagerAllvariables( const PreconditionerSolverManagerAllvariables& preconditionersolvermanagerallvariables);
    PreconditionerSolverManagerAllvariables& operator=( const PreconditionerSolverManagerAllvariables& preconditionersolvermanagerallvariables);
    std::string getName() const;
    PreconditionerSolverManagerAllvariables* clone() const;

    int getNVectors() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor);
    void reInit();
    void computePreconditioner();
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    std::ostream& printLoopInformation(std::ostream& os) const;
  };
}

/*--------------------------------------------------------------------------*/
#endif
