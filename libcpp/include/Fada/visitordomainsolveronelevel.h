#ifndef __Fada_VisitorDomainSolverOneLevel_h
#define __Fada_VisitorDomainSolverOneLevel_h

#include  "chronometer.h"
#include  "Alat/iterativesolvervisitorwithvectorinterface.h"
#include  "Alat/intvector.h"
#include  "Alat/stringset.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class MatrixAllVariables;
  class SystemVector;
}
namespace Fada
{
  class DomainSolverInterface; 

  class VisitorDomainSolverOneLevel : public Alat::IterativeSolverVisitorWithVectorInterface
  {
protected:
    int _level;
    DomainSolverInterface* _domainsolver;
    mutable Chronometer _chronometer;
    Alat::StringSet _variables;
    DomainSolverInterface* getDomainSolver();
    Alat::SystemVector* getVectorVariables(Alat::GhostVector& u) const;
    const Alat::SystemVector* getVectorVariables(const Alat::GhostVector& u) const;
    const Alat::MatrixAllVariables* getMatrixVariables(const Alat::GhostMatrix& A) const;

public:
    ~VisitorDomainSolverOneLevel();
    VisitorDomainSolverOneLevel(DomainSolverInterface* domainsolver, int level, const Alat::StringSet& variables);
    VisitorDomainSolverOneLevel( const VisitorDomainSolverOneLevel& visitoronelevel);
    VisitorDomainSolverOneLevel& operator=( const VisitorDomainSolverOneLevel& visitoronelevel);
    std::string getName() const;
    VisitorDomainSolverOneLevel* clone() const;

    const DomainSolverInterface* getDomainSolver() const;
    int getVectorLevel() const;
    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    void newVector(Alat::GhostVector* u);
    void vectorEqual(Alat::GhostVector& p, const Alat::GhostVector& q) const;
    void vectorZero(Alat::GhostVector& p) const;
    void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    void vectorScale(Alat::GhostVector& r, double d) const;
    double scalarProduct(const Alat::GhostVector& p, const Alat::GhostVector& q) const;
    void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& p, const Alat::GhostVector& q, double d) const;
    Alat::SystemVector* getVector(Alat::GhostVector& u) const;
    const Alat::SystemVector* getVector(const Alat::GhostVector& u) const;
    const Alat::IntVector& getNOfVars() const;
    const Alat::IntVector& getNcompOfVars() const;
    int level() const;
    int nlevels() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
