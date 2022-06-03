#ifndef __Alat_IterativeSolverVisitorInterface_h
#define __Alat_IterativeSolverVisitorInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
  class GhostLinearSolver;
  class GhostMatrix;
  class GhostVector;
  class IntVector;
  class ParameterFile;
  class StringVector;
}
namespace Alat
{
  class IterativeSolverVisitorInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~IterativeSolverVisitorInterface();
    IterativeSolverVisitorInterface();
    IterativeSolverVisitorInterface( const IterativeSolverVisitorInterface& iterativesolvervisitorinterface);
    IterativeSolverVisitorInterface& operator=( const IterativeSolverVisitorInterface& iterativesolvervisitorinterface);
    std::string getName() const;
    Alat::IterativeSolverVisitorInterface* clone() const;

    virtual void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    virtual std::ostream& printLoopInformation(std::ostream& os) const;
    virtual std::string getVectorType() const;
    virtual int getVectorLevel() const=0;
    virtual void newVector(Alat::GhostVector* u) = 0;

    virtual void vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const;
    virtual void vectorZero(Alat::GhostVector& v) const;
    virtual void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    virtual void vectorScale(Alat::GhostVector& r, double d) const;
    virtual double scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const;
    virtual double vectorNorm(const Alat::GhostVector& r) const;

    virtual void residual(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& f) const;
    virtual void matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;

    virtual void postProcess(Alat::GhostVector& u) const;

    virtual const Alat::IntVector& getDomainsPermutation(int iteration) const;
    virtual void solveOnDomain(int idomain, const Alat::GhostLinearSolver& linearsolverdomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    virtual void vectorEqualOnDomain(int idomain, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    virtual void matrixVectorProductCoupling(int i, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f, double d) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
