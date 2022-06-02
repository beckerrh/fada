#include  "Fada/domainsolverinterface.h"
#include  "Alat/ghostmatrix.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/matrixallvariables.h"
#include  "Fada/solvermanagerinterface.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "Alat/sparsitypatternwithrowindex.h"
#include  "Fada/variablemanager.h"
#include  "Alat/systemvector.h"
#include  "Fada/directsolver.h"
#include  <cassert>
#include  <iomanip>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DirectSolver::~DirectSolver()
{}
DirectSolver::DirectSolver(int level, const Alat::GhostMatrix& ghostmatrix, const Fada::SolverManagerInterface* solvermanager, const Alat::StringVector& variables) : Alat::IterativeSolverWithVisitor(), _ghostmatrix(ghostmatrix), _solvermanager(solvermanager), _level(level), _reinitcalled(false), _computecalled(false)
{
  assert( variables.size() );
  _variables.set_size( variables.size() );
  _variables = variables;
  int levelmatrix = ghostmatrix.getLevel();
  if(levelmatrix != -1)
  {
    assert(levelmatrix == _level);
  }
}

DirectSolver::DirectSolver( const DirectSolver& directsolver) : Alat::IterativeSolverWithVisitor(directsolver), _ghostmatrix(directsolver._ghostmatrix)
{
  assert(0);
}

DirectSolver& DirectSolver::operator=( const DirectSolver& directsolver)
{
  assert(0);
  return *this;
}

std::string DirectSolver::getName() const
{
  return "DirectSolver";
}

bool DirectSolver::hasIterationInfo() const
{
  return false;
}

/*--------------------------------------------------------------------------*/
Fada::Chronometer DirectSolver::_chronometer("DirectSolver");
void DirectSolver::printChronometer(std::ostream& os)
{
  _chronometer.print(os);
}

void DirectSolver::initChronometer()
{
  _chronometer.enrol("compute", true);
  _chronometer.enrol("solve", true);
  _chronometer.enrol("reInit", true);
}

/*--------------------------------------------------------------------------*/
void DirectSolver::reInit()
{
  _chronometer.start("reInit");
  assert(_reinitcalled == false);
  _reinitcalled = true;

  int nsubdomains = _solvermanager->getNDomainSolvers();

  //numerotation des variables
  // std::cerr << "_variables="<<_variables<<"\n";
  // std::cerr << "nsubdomains="<<nsubdomains<<"\n";
  _varnametoindex.set_size(nsubdomains);
  for(int idomain = 0; idomain < nsubdomains; idomain++)
  {
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    const Fada::VariableManager* variablemanager = domainsolver->getVariableManager();
    int count = 0;
    for(int ivar = 0; ivar < _variables.size(); ivar++)
    {
      std::string varname = _variables[ivar];
      if( not variablemanager->variableIsUnknown(varname) )
      {
        continue;
      }
      _varnametoindex[idomain][varname] = count++;
    }
  }
  // std::cerr << "_varnametoindex="<<_varnametoindex<<"\n";
  _ncompofvars.set_size(nsubdomains);
  _nofvars.set_size(nsubdomains);
  for(int idomain = 0; idomain < nsubdomains; idomain++)
  {
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    const Fada::VariableManager* variablemanager = domainsolver->getVariableManager();
    int nvars = _varnametoindex[idomain].size();
    // std::cerr << "idomain="<<idomain << " nvars="<<nvars<<"\n";
    _nofvars[idomain].set_size( nvars );
    _ncompofvars[idomain].set_size( nvars );
    for(Alat::StringIntMap::const_iterator p = _varnametoindex[idomain].begin(); p != _varnametoindex[idomain].end(); p++)
    {
      int ivar = p->second;
      _nofvars[idomain][ivar] = domainsolver->getNOfVar(_level, p->first);
      _ncompofvars[idomain][ivar] = domainsolver->getNcompOfVar(p->first);
    }
  }
  _offsets.set_size(nsubdomains);
  int offsetlast=0;
  int ntotal2=0;
  for(int idomain = 0; idomain < nsubdomains; idomain++)
  {
    int nvars = _nofvars[idomain].size();
    _offsets[idomain].set_size(nvars);
    if(nvars==0) continue;

    _offsets[idomain][0] = offsetlast;
    for(int ivar = 0; ivar < nvars-1; ivar++)
    {
      _offsets[idomain][ivar+1] = _offsets[idomain][ivar] + _ncompofvars[idomain][ivar]*_nofvars[idomain][ivar];
    }
    offsetlast = _offsets[idomain][nvars-1]+_ncompofvars[idomain][nvars-1]*_nofvars[idomain][nvars-1];
    ntotal2 += _ncompofvars[idomain][nvars-1]*_nofvars[idomain][nvars-1];
  }

  // int nlast = _nofvars[nsubdomains-1].size();
  // std::cerr << "nlast="<<nlast<<"\n";
  // assert(nlast);
  // int ntotal = _offsets[nsubdomains-1][nlast-1] + _ncompofvars[nsubdomains-1][nlast-1]*_nofvars[nsubdomains-1][nlast-1];
  // assert(ntotal==offsetlast);
  int ntotal = offsetlast;
  _u.set_size(ntotal);
  _f.set_size(ntotal);
  _sparsitypatternsoft.clear();
  _sparsitypatternsoft.set_size(ntotal);




  for(int idomain = 0; idomain < nsubdomains; idomain++)
  {
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    for(Alat::StringIntMap::const_iterator p = _varnametoindex[idomain].begin(); p != _varnametoindex[idomain].end(); p++)
    {
      int ivar = p->second;
      for(Alat::StringIntMap::const_iterator q = _varnametoindex[idomain].begin(); q != _varnametoindex[idomain].end(); q++)
      {
        int jvar = q->second;
        const Alat::VariableMatrixInterface* matrix = domainsolver->getMatrix(_level, _ghostmatrix, p->first, q->first);
        matrix->addEntriesForDirectSolver(_offsets[idomain][ivar], _offsets[idomain][jvar], _sparsitypatternsoft);
      }
    }
  }

  _umf.getSparseMatrix().set_size(_sparsitypatternsoft);
  _umf.reInit();
  _chronometer.stop("reInit");
}

/*--------------------------------------------------------------------------*/
void DirectSolver::compute()
{
  _chronometer.start("compute");
  assert(_reinitcalled);
  _computecalled = true;
  int ndomains = _solvermanager->getNDomainSolvers();
  _umf.getSparseMatrix().zeros();
  const Alat::SparsityPattern* sparsitypattern = _umf.getSparseMatrix().getSparsityPattern();
  Alat::DoubleVector&  values = _umf.getSparseMatrix().getValue();
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    // addMatrix( idomain, getVisitor()->getDomainMatrix(idomain, _ghostmatrix) );
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    for(Alat::StringIntMap::const_iterator p = _varnametoindex[idomain].begin(); p != _varnametoindex[idomain].end(); p++)
    {
      int ivar = p->second;
      for(Alat::StringIntMap::const_iterator q = _varnametoindex[idomain].begin(); q != _varnametoindex[idomain].end(); q++)
      {
        int jvar = q->second;
        const Alat::VariableMatrixInterface* matrix = domainsolver->getMatrix(_level, _ghostmatrix, p->first, q->first);
        matrix->addMatrixForDirectSolver(_offsets[idomain][ivar], _offsets[idomain][jvar], values, sparsitypattern);
      }
    }
  }

  _umf.computeLu();
  _chronometer.stop("compute");
}

/*--------------------------------------------------------------------------*/
void DirectSolver::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _chronometer.start("solve");
  assert(_computecalled);
  _f.zeros();
  int ndomains = _solvermanager->getNDomainSolvers();
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    const Alat::SystemVector* fv = dynamic_cast<const Alat::SystemVector*>( domainsolver->getVector(_level, f) );
    assert(fv);
    for(Alat::StringIntMap::const_iterator p = _varnametoindex[idomain].begin(); p != _varnametoindex[idomain].end(); p++)
    {
      int ivar = p->second;
      fv->getVector(p->first)->addVectorRhsForDirectSolver(_offsets[idomain][ivar], _f);
    }
  }
  _umf.solve(_u, _f);
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    const Fada::DomainSolverInterface* domainsolver = _solvermanager->getDomainSolver(idomain);
    Alat::SystemVector* uv = dynamic_cast<Alat::SystemVector*>( domainsolver->getVector(_level, u) );
    assert(uv);
    for(Alat::StringIntMap::const_iterator p = _varnametoindex[idomain].begin(); p != _varnametoindex[idomain].end(); p++)
    {
      int ivar = p->second;
      uv->getVector(p->first)->setSingleVectorFromDirectSolver(_offsets[idomain][ivar], _u);
    }
  }
  status = AlatEnums::IterationStatusConverged;
  _chronometer.stop("solve");
}
