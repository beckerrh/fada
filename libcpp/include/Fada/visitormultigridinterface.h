#ifndef __Fada_VisitorMultigridInterface_h
#define __Fada_VisitorMultigridInterface_h

#include  "Alat/iterativesolvervisitorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class VisitorMultigridInterface : public Alat::IterativeSolverVisitorInterface
  {
protected:
    mutable int _level;
    std::string getInterfaceName() const;

public:
    ~VisitorMultigridInterface();
    VisitorMultigridInterface();
    VisitorMultigridInterface( const VisitorMultigridInterface& multigridsolvervisitorinterface);
    VisitorMultigridInterface& operator=( const VisitorMultigridInterface& multigridsolvervisitorinterface);
    std::string getName() const;
    VisitorMultigridInterface* clone() const;

    std::string getVectorType() const;
    int getVectorLevel() const;
    virtual void setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const;
    virtual void setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const;
    virtual void vectorZero(int level, Alat::GhostVector& v) const;
    virtual double vectorNorm(int level, const Alat::GhostVector& r) const;
    virtual double scalarProduct(int level, const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const;
    virtual void vectorEqual(int level, Alat::GhostVector& r, const Alat::GhostVector& f) const;
    virtual void vectorAdd(int level, Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    virtual void matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;
    virtual void residual(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& b) const;
    virtual int getMinLevel() const;
    virtual int getMaxLevel() const;
    virtual void restrict(int level, Alat::GhostVector& f, const Alat::GhostVector& v ) const;
    virtual void prolongate(int level, Alat::GhostVector& u, const Alat::GhostVector& v, double d = 1.0) const;
    virtual void setLevel(int level) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
