#ifndef __Fada_Multigrid_h
#define __Fada_Multigrid_h

#include  "chronometer.h"
#include  "Alat/iterativesolverwithvisitor.h"
#include  "Alat/preconditionerinterface.h"
#include  "visitormultigridinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LinearSolverVectorInterface;

  class Multigrid : public virtual Alat::IterativeSolverWithVisitor, public virtual Alat::PreconditionerInterface
  {
private:
    std::string _dirname;
    bool _deletemultilevellinearsolver, _initialized;
    mutable AlatEnums::iterationstatus _statussmoother;
    int _niterpre, _niterpost, _nitercoarse;
    mutable int _cycle, _minlevel, _maxlevel;
    static Fada::Chronometer _chronometer;
    LinearSolverVectorInterface* _linearsolvervector;

    double step(AlatEnums::iterationstatus& status, int level, int nlevelsignore, int maxlevel, const Alat::GhostMatrix& A, Alat::GhostVector& u, Alat::GhostVector& f, Alat::GhostVector& v, const Alat::IterationInfo& info) const;
    void smoothpre(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const;
    void smoothpost(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const;
    void smoothcoarse(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const;

public:
    ~Multigrid();
    Multigrid(std::string dirname);
    Multigrid( const Multigrid& multigrid);
    Multigrid& operator=( const Multigrid& multigrid);
    std::string getName() const;
    Multigrid* clone() const;
    std::string getInterfaceName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    static void initChronometer();
    static void printChronometer(std::ostream& os);

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor = NULL);
    int getNVectors() const;
    void reInit();
    // void restart();
    void compute();
    void computePreconditioner();
    const Fada::VisitorMultigridInterface* getVisitor() const;
    Fada::VisitorMultigridInterface* getVisitor();
    void setLinearSolverVector(LinearSolverVectorInterface* linearsolvervectorinterface);
    void newLinearSolverVector(LinearSolverVectorInterface* linearsolvervectorinterface);
    LinearSolverVectorInterface* getLinearSolverVector();
    void solve(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solveApproximate(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    // void solve(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const;
    void setsmoothtype(std::string smoothtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
