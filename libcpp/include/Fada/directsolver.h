#ifndef __Fada_DirectSolver_h
#define __Fada_DirectSolver_h

#include  "Alat/doublevector.h"
#include  "Alat/map.h"
#include  "Fada/chronometer.h"
#include  "Alat/iterativesolverwithvisitor.h"
#include  "Fada/solvermanager.h"
#include  "Alat/sparsematrix.h"
#include  "Alat/umfmatrixbase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class MatrixAllVariables;
  class SystemVector;
}

namespace Fada
{
  class DirectSolver : public Alat::IterativeSolverWithVisitor
  {
private:
    static Fada::Chronometer _chronometer;
    bool _reinitcalled, _computecalled;
    const Fada::SolverManager* _solvermanager;
    Alat::Vector<Alat::IntVector> _ncompofvars, _nofvars;
    Alat::SparsityPatternSoft _sparsitypatternsoft;
    Alat::UmfMatrixBase _umf;
    mutable Alat::DoubleVector _u, _f;
    Alat::Vector<Alat::IntVector> _offsets;
    const Alat::GhostMatrix _ghostmatrix;
    Alat::Vector<Alat::StringIntMap > _varnametoindex;
    Alat::StringVector _variables;
    int _level;
    // void set_size(const Alat::Vector<Alat::IntVector>& nofvars, const Alat::Vector<Alat::IntVector>& ncompofvars);
    // void addMatrix(int i, const Alat::SystemMatrixInterface* A);
    // void addCouplingMatrix(int idomain, int jdomain, const Alat::SystemMatrixInterface* matrixLR, const Alat::SystemMatrixInterface* matrixRL);
    // void addVectorRhs( int i, const Alat::SystemVector* f) const;
    // void setVectorSolution(int i, Alat::SystemVector* u) const;
    // void addMatrixEntries(int i, const Alat::SystemMatrixInterface* A);
    // void addCouplingMatrixEntries(int idomain, int jdomain, const Alat::SystemMatrixInterface* matrixLR, const Alat::SystemMatrixInterface* matrixRL);

public:
    ~DirectSolver();
    DirectSolver(int level, const Alat::GhostMatrix& ghostmatrix, const Fada::SolverManager* solvermanager, const Alat::StringVector& variables);
    DirectSolver( const DirectSolver& directsolver);
    DirectSolver& operator=( const DirectSolver& directsolver);
    std::string getName() const;
    static void initChronometer();
    static void printChronometer(std::ostream& os);

    bool hasIterationInfo() const;
    void reInit();
    void compute();
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
