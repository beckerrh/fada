#include  "Alat/iterativesolvervisitorwithvectorinterface.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Alat/systemvector.h"
#include  "Alat/variablevectorinterface.h"
#include  "Alat/umfsolversomevariables.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
UmfSolverSomeVariables::~UmfSolverSomeVariables()
{}
UmfSolverSomeVariables::UmfSolverSomeVariables() : Alat::IterativeSolverWithVisitor(), _reinitcalled(false), _computecalled(false), _systemmatrix(NULL)
{}
UmfSolverSomeVariables::UmfSolverSomeVariables( const UmfSolverSomeVariables& structuredumfsolver) : Alat::IterativeSolverWithVisitor(structuredumfsolver), _reinitcalled(structuredumfsolver._reinitcalled), _computecalled(structuredumfsolver._computecalled), _systemmatrix(NULL)
{}

UmfSolverSomeVariables& UmfSolverSomeVariables::operator=( const UmfSolverSomeVariables& structuredumfsolver)
{
  Alat::IterativeSolverWithVisitor::operator=(structuredumfsolver);
  assert(0);
  return *this;
}

std::string UmfSolverSomeVariables::getName() const
{
  return "UmfSolverSomeVariables";
}

UmfSolverSomeVariables* UmfSolverSomeVariables::clone() const
{
  assert(0);
  return new UmfSolverSomeVariables(*this);
}

bool UmfSolverSomeVariables::hasIterationInfo() const
{
  return false;
}

/*--------------------------------------------------------------------------*/
void UmfSolverSomeVariables::setMatrix(int level, const Alat::SystemMatrixInterface* systemmatrix, const Alat::StringVector& variables, const Alat::IntVector& ncompofvars, const Alat::IntVector& nofvars)
{
  // std::cerr << "UmfSolverSomeVariables::setMatrix() variables="<<variables<<"\n";
  _systemmatrix = systemmatrix;
  _variables.set_size( variables.size() );
  _variables = variables;
  if(_variables.size()==0)
  {
    _error_string("setMatrix", "no variables _variables=void !!");
  }

  _offsets.set_size(_variables.size()+1);
  _offsets[0] = 0;
  for(int ii = 0; ii < _variables.size(); ii++)
  {
    std::string varname = _variables[ii];
    // int nofvar = domainsolver->getNOfVar(level, varname);
    // int ncompofvar = domainsolver->getNcompOfVar(varname);
    // std::cerr << "UmfSolverSomeVariables::setMatrix() varname="<<varname<<" ncompofvar="<<ncompofvar<<"\n";
    _offsets[ii+1] = _offsets[ii] + ncompofvars[ii]*nofvars[ii];
  }
  // std::cerr << "_offsets="<<_offsets<<"\n";
}

/*--------------------------------------------------------------------------*/
void UmfSolverSomeVariables::reInit()
{
  assert(_reinitcalled == false);
  _reinitcalled = true;

  int ntotal = _offsets[_variables.size()];

  _u.set_size(ntotal);
  _f.set_size(ntotal);

  Alat::SparsityPatternSoft sparsitypatternsoft;
  sparsitypatternsoft.set_size(ntotal);
  for(int ii = 0; ii < _variables.size(); ii++)
  {
    std::string varnameI = _variables[ii];
    for(int jj = 0; jj < _variables.size(); jj++)
    {
      std::string varnameJ = _variables[jj];
      _systemmatrix->getMatrix(varnameI, varnameJ)->addEntriesForDirectSolver(_offsets[ii], _offsets[jj], sparsitypatternsoft);
    }
  }
  _umf.getSparseMatrix().set_size(sparsitypatternsoft);
  _umf.reInit();
}

/*--------------------------------------------------------------------------*/
void UmfSolverSomeVariables::compute()
{
  assert(_reinitcalled);
  _computecalled = true;

  const Alat::SparsityPattern* sparsitypattern = _umf.getSparseMatrix().getSparsityPattern();
  Alat::DoubleVector&  values = _umf.getSparseMatrix().getValue();
  values.zeros();
  for(int ii = 0; ii < _variables.size(); ii++)
  {
    std::string varnameI = _variables[ii];
    for(int jj = 0; jj < _variables.size(); jj++)
    {
      std::string varnameJ = _variables[jj];
      _systemmatrix->getMatrix(varnameI, varnameJ)->addMatrixForDirectSolver(_offsets[ii], _offsets[jj], values, sparsitypattern);
    }
  }
  _umf.computeLu();
}

/*--------------------------------------------------------------------------*/

void UmfSolverSomeVariables::solve(AlatEnums::iterationstatus& status, Alat::SystemVectorInterface* u, const Alat::SystemVectorInterface* f) const
{
  assert(_reinitcalled);
  assert(_computecalled);

  const Alat::SystemVector* fv = dynamic_cast<const Alat::SystemVector*>( f );
  assert(fv);
  Alat::SystemVector* uv = dynamic_cast<Alat::SystemVector*>( u );
  assert(uv);
  _f.zeros();

  for(int ii = 0; ii < _variables.size(); ii++)
  {
    std::string varname = _variables[ii];
    fv->getVector(varname)->addVectorRhsForDirectSolver(_offsets[ii], _f);
  }
  _umf.solve(_u, _f);
  for(int ii = 0; ii < _variables.size(); ii++)
  {
    std::string varname = _variables[ii];
    uv->getVector(varname)->setSingleVectorFromDirectSolver(_offsets[ii], _u);
  }
  status = AlatEnums::IterationStatusConverged;
}

/*--------------------------------------------------------------------------*/
void UmfSolverSomeVariables::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  assert(_variables.contains(u.getVariables())); 
  assert(_variables.contains(f.getVariables())); 
  // std::cerr << "UmfSolverSomeVariables::solve() _variables="<<_variables<<"\n";
  // std::cerr << "UmfSolverSomeVariables::solve() u="<<u<<"\n";
  // std::cerr << "UmfSolverSomeVariables::solve() f="<<f<<"\n";
  // std::cerr << "UmfSolverSomeVariables::getVisitor()="<<getVisitor()->getName()<<"\n";
  const Alat::IterativeSolverVisitorWithVectorInterface* visitor = dynamic_cast<const Alat::IterativeSolverVisitorWithVectorInterface*>(getVisitor());
  assert(visitor);
  solve( status, visitor->getVector(u), visitor->getVector(f) );
}
