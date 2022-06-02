#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Alat/systemvector.h"
#include  "Alat/umfsolveronevariable.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
UmfSolverOneVariable::~UmfSolverOneVariable()
{}
UmfSolverOneVariable::UmfSolverOneVariable() : Alat::IterativeSolverWithVisitor(), Alat::Preconditioner(), _reinitcalled(false), _computecalled(false), _variablematrix(NULL)
{}
UmfSolverOneVariable::UmfSolverOneVariable(const Alat::VariableMatrixInterface* variablematrix) : Alat::IterativeSolverWithVisitor(), Alat::Preconditioner(), _reinitcalled(false), _computecalled(false), _variablematrix(variablematrix)
{}
UmfSolverOneVariable::UmfSolverOneVariable( const UmfSolverOneVariable& umfsolver) : Alat::IterativeSolverWithVisitor(umfsolver), Alat::Preconditioner(umfsolver), _reinitcalled(false), _computecalled(false), _variablematrix(NULL)
{}

UmfSolverOneVariable& UmfSolverOneVariable::operator=( const UmfSolverOneVariable& umfsolver)
{
  Alat::IterativeSolverWithVisitor::operator=(umfsolver);
  assert(0);
  return *this;
}

std::string UmfSolverOneVariable::getName() const
{
  return "UmfSolverOneVariable";
}

std::string UmfSolverOneVariable::getInterfaceName() const
{
  return "UmfSolverOneVariable";
}

UmfSolverOneVariable* UmfSolverOneVariable::clone() const
{
  assert(0);
  return new UmfSolverOneVariable(*this);
}

/*--------------------------------------------------------------------------*/
int UmfSolverOneVariable::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
void UmfSolverOneVariable::setMatrix(const Alat::VariableMatrixInterface* variablematrix)
{
  _variablematrix = variablematrix;
}

/*--------------------------------------------------------------------------*/
void UmfSolverOneVariable::reInit()
{  
  assert(_reinitcalled == false);
  _reinitcalled = true;

  assert(_variablematrix);
  int ntotal = _variablematrix->getN()*_variablematrix->getNComponents();
  // std::cerr << "UmfSolverOneVariable::reInit( ncomp="<<_variablematrix->getNComponents()<<" N="<< _variablematrix->getN()<<" ntotal="<<ntotal<<"\n";
  _u.set_size(ntotal);
  _f.set_size(ntotal);

  Alat::SparsityPatternSoft sparsitypatternsoft;
  sparsitypatternsoft.set_size(ntotal);
  _variablematrix->addEntriesForDirectSolver(0, 0, sparsitypatternsoft);
  _umf.getSparseMatrix().set_size(sparsitypatternsoft);
  _umf.reInit();
}

/*--------------------------------------------------------------------------*/
void UmfSolverOneVariable::compute()
{
  assert(_reinitcalled);
  _computecalled = true;

  const Alat::SparsityPattern* sparsitypattern = _umf.getSparseMatrix().getSparsityPattern();
  Alat::DoubleVector&  values = _umf.getSparseMatrix().getValue();
  values.zeros();
  _variablematrix->addMatrixForDirectSolver(0, 0, values, sparsitypattern);
  _umf.computeLu();
}
void UmfSolverOneVariable::computePreconditioner()
{
  compute();
}
void UmfSolverOneVariable::_solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const
{
  solve(status, u, f);
}

/*--------------------------------------------------------------------------*/
void UmfSolverOneVariable::solve(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f) const
{ 
  assert(_reinitcalled);
  assert(_computecalled);

  _f.zeros();
  f->addVectorRhsForDirectSolver(0, _f);
  _umf.solve(_u, _f);
  u->setSingleVectorFromDirectSolver(0, _u);
  status = AlatEnums::IterationStatusConverged;
}

/*--------------------------------------------------------------------------*/
void UmfSolverOneVariable::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  assert(0);
}
