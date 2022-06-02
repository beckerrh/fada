#ifndef __Alat_RichardsonRB_h
#define __Alat_RichardsonRB_h

#include  "doublematrix.h"
#include  "doublevector.h"
#include  "iterativesolverwithpreconditioner.h"
#include  "armadillo.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class RichardsonRB : public IterativeSolverWithPreconditioner
  {
protected:
    mutable double _rnorm;
    mutable int _niterafterrestar;
    int _nvectors, _nvars, _nshift;
    int _nextupdate, _nextproduct;
    mutable int _nmemory, _nextmemory;
    std::string _type, _solutiontype;
    mutable const Alat::GhostMatrix* _ghostmatrix;
    mutable double _condition, _conditionmax, _conditionmean;
    mutable arma::mat _H;
    mutable arma::vec _b, _x;
    Alat::GhostVector& getV(int i) const;
    Alat::GhostVector& getAV(int i, int ivar = 0) const;
    void _solveSmallSystem(AlatEnums::iterationstatus& status) const;
    virtual void _computeSmallSystem(int index, int nmemory) const = 0;
    virtual void _addvector(Alat::GhostVector& u, int nmemory) const = 0;
    virtual void _matrixVectorProduct(int index) const = 0;

public:
    ~RichardsonRB();
    RichardsonRB(const std::string& type, int nvectors, const std::string& solutiontype);
    RichardsonRB( const RichardsonRB& richardsonoptimal);
    RichardsonRB& operator=( const RichardsonRB& richardsonoptimal);
    std::string getName() const;
    RichardsonRB* clone() const;

    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const;
    void restart();
  };
}

/*--------------------------------------------------------------------------*/

#endif
