#ifndef __Fada_VisitorSolverManagerMultiLevel_h
#define __Fada_VisitorSolverManagerMultiLevel_h

#include  "chronometer.h"
#include  "visitormultigridinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LinearSolverVector;
  class SolverManagerInterface;
  class VisitorSolverManagerOneLevel;

  class VisitorSolverManagerMultiLevel : public VisitorMultigridInterface
  {
protected:
    mutable Fada::Chronometer _chronometer;
    int _nlevelsignore, _maxlevel;
    std::string _varname;
    SolverManagerInterface* _solvermanager;
    SolverManagerInterface* getSolverManager();
    const Fada::LinearSolverVector&  _linearsolvers;
    Alat::Vector<const Fada::VisitorSolverManagerOneLevel*> _visitors;
    const Alat::StringSet _variables;
    
public:
    ~VisitorSolverManagerMultiLevel();
    VisitorSolverManagerMultiLevel(SolverManagerInterface* solvermanager, int nlevelsignore, int maxlevel, const Fada::LinearSolverVector&  linearsolvers, const Alat::StringSet& variables);
    VisitorSolverManagerMultiLevel( const VisitorSolverManagerMultiLevel& visitormultigrid);
    VisitorSolverManagerMultiLevel& operator=( const VisitorSolverManagerMultiLevel& visitormultigrid);
    std::string getName() const;
    VisitorSolverManagerMultiLevel* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    void setLevel(int level) const;
    int getMaxLevel() const;
    int getMinLevel() const;
    const SolverManagerInterface* getSolverManager() const;
    void newVector(Alat::GhostVector* u);
    void vectorZero(int level, Alat::GhostVector& v) const;
    double vectorNorm(int level, const Alat::GhostVector& r) const;
    void vectorEqual(int level, Alat::GhostVector& r, const Alat::GhostVector& f) const;
    void vectorAdd(int level, Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
    void matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;

    void setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const;
    void setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const;
    void restrict(int level, Alat::GhostVector& f, const Alat::GhostVector& v ) const;
    void prolongate(int level, Alat::GhostVector& u, const Alat::GhostVector& v, double d = 1.0) const;
    void smoothInterfaceOnLevel(int level, Alat::GhostVector& u) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
