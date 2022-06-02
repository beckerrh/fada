#ifndef __Alat_NonlinearSolver_h
#define __Alat_NonlinearSolver_h

#include  "vector.h"
#include  "ghostvector.h"
#include  "iterationinfo.h"
#include  "nonlinearsolverinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  // class LinearSolverInterface;
  class NonlinearSolverVisitorInterface;

  class NonlinearSolver : public NonlinearSolverInterface
  {
private:
    // Alat::LinearSolverInterface* _linearsolver;
    // bool _deletelinearsolver;
    int _id;
    static int _totalids;
    mutable Alat::Vector<Alat::GhostVector> _memory;
    Alat::IterationInfo _info;
    Alat::NonlinearSolverVisitorInterface* _visitor;
    bool _deletevisitor;

protected:
    mutable AlatEnums::iterationstatus _status_iteration;
    mutable bool _newmatrix;
    double _lineartoleranceincrease, _rhomatrix;
    mutable double _lintol, _dxnorm, _omega;
    mutable int _nliniter;
    int _maxnlinesearch;
    double _omegalinesearch, _monotonyfactor;
    bool _printlinesearch;

    void memory();
    Alat::GhostVector& getMemory(int i) const;
    bool _basicinitcalled;
    
    void _lineSearchMonotonicity(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f) const;
    void _lineSearchArmijo(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f, double kappa) const;
    void _lineSearchWolfe(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f, double kappa) const;
    bool _checkIteration(Alat::NewtonData& newtondata, const Alat::IterationInfo& info) const;

public:
    ~NonlinearSolver();
    NonlinearSolver();
    NonlinearSolver( const NonlinearSolver& nonlinearsolver);
    NonlinearSolver& operator=( const NonlinearSolver& nonlinearsolver);
    std::string getName() const;
    NonlinearSolver* clone() const;

    // Alat::LinearSolverInterface*& newLinearSolverPointer();
    // void setLinearSolverPointer(Alat::LinearSolverInterface* linearsolver);
    // const Alat::LinearSolverInterface* getLinearSolver() const;
    // Alat::LinearSolverInterface* getLinearSolver();
    const Alat::NonlinearSolverVisitorInterface* getVisitor() const;
    Alat::NonlinearSolverVisitorInterface* getVisitor();
    const IterationInfo& getIterationInfo() const;
    IterationInfo& getIterationInfo();

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    std::ostream& printLoopInformation(std::ostream& os) const;
    Alat::NonlinearSolverVisitorInterface*& newVisitorPointer();
    void setVisitorPointer(Alat::NonlinearSolverVisitorInterface* visitor);
    double getRhoMatrix() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
