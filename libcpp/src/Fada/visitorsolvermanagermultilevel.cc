#include  "Fada/domainsolverinterface.h"
#include  "Alat/ghostmatrix.h"
#include  "Alat/iomanager.h"
#include  "Alat/iterativesolverwithvisitor.h"
#include  "Fada/linearsolvervector.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/solvermanager.h"
#include  "Fada/visitorsolvermanagermultilevel.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  <cassert>
#include  <cmath>
#include  <fstream>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
VisitorSolverManagerMultiLevel::~VisitorSolverManagerMultiLevel()
{}

VisitorSolverManagerMultiLevel::VisitorSolverManagerMultiLevel(SolverManager* solvermanager, int nlevelsignore, int maxlevel, const Fada::LinearSolverVector&  linearsolvers, const Alat::StringSet& variables) : VisitorMultigridInterface(), _solvermanager(solvermanager), _nlevelsignore(nlevelsignore), _maxlevel(maxlevel), _chronometer( ), _linearsolvers(linearsolvers), _variables(variables)
{
  _chronometer.enrol("scalarProduct", true);
  _chronometer.enrol("vectorOperations", true);
  _chronometer.enrol("matrixVectorProduct", true);
  _chronometer.enrol("smooth", true);
  _chronometer.enrol("transfer", true);
  _chronometer.enrol("matrixVectorProductCoupling", true);
}

VisitorSolverManagerMultiLevel::VisitorSolverManagerMultiLevel( const VisitorSolverManagerMultiLevel& visitorsolvermanagermultilevel) : VisitorMultigridInterface(visitorsolvermanagermultilevel), _solvermanager(visitorsolvermanagermultilevel._solvermanager), _nlevelsignore(visitorsolvermanagermultilevel._nlevelsignore), _maxlevel(visitorsolvermanagermultilevel._maxlevel), _chronometer(visitorsolvermanagermultilevel._chronometer), _linearsolvers(visitorsolvermanagermultilevel._linearsolvers), _variables(visitorsolvermanagermultilevel._variables)
{
  assert(0);
}

VisitorSolverManagerMultiLevel& VisitorSolverManagerMultiLevel::operator=( const VisitorSolverManagerMultiLevel& visitormultigrid)
{
  VisitorMultigridInterface::operator=(visitormultigrid);
  assert(0);
  return *this;
}

std::string VisitorSolverManagerMultiLevel::getName() const
{
  std::stringstream ss;
  ss << "VisitorSolverManagerMultiLevel_" << _variables;
  return ss.str();
}

VisitorSolverManagerMultiLevel* VisitorSolverManagerMultiLevel::clone() const
{
  return new VisitorSolverManagerMultiLevel(*this);
}


/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::setLevel(int level) const
{
	_level = level;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _chronometer.setClassName(getName());
  int nlevels = _linearsolvers.getNLevels();
  _visitors.set_size(nlevels);
  for(int level = 0; level < nlevels; level++)
  {
    const Alat::IterativeSolverWithVisitor* solverwithvisitor = dynamic_cast<const Alat::IterativeSolverWithVisitor*>( _linearsolvers.getLinearSolver(level) );
    assert(solverwithvisitor);
    const VisitorSolverManagerOneLevel* visitordomainsolver = dynamic_cast<const VisitorSolverManagerOneLevel*>( solverwithvisitor->getVisitor() );
    assert(visitordomainsolver);
    // assert(0);
    //!! getVariables() n'est plus de vector mais string
    assert(visitordomainsolver->getVariables() == _variables);
    _visitors[level] = visitordomainsolver;
  }
}

/*--------------------------------------------------------------------------*/

void VisitorSolverManagerMultiLevel::setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const
{
  for(int i = 0; i < getSolverManager()->getNDomainSolvers(); i++)
  {
    getSolverManager()->getDomainSolver(i)->setMultiGridVector(level, mgf, gf);
  }
}

/*--------------------------------------------------------------------------*/

void VisitorSolverManagerMultiLevel::setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const
{
  for(int i = 0; i < getSolverManager()->getNDomainSolvers(); i++)
  {
    getSolverManager()->getDomainSolver(i)->setVector(level, gf, mgf );
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::restrict(int level, Alat::GhostVector& f, const Alat::GhostVector& v ) const
{
  _chronometer.start("transfer");
  for(int i = 0; i < getSolverManager()->getNDomainSolvers(); i++)
  {
    getSolverManager()->getDomainSolver(i)->restrict(level, f, v );
  }
  _chronometer.stop("transfer");
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::prolongate(int level, Alat::GhostVector& u, const Alat::GhostVector& v, double d) const
{
  _chronometer.start("transfer");
  for(int i = 0; i < getSolverManager()->getNDomainSolvers(); i++)
  {
    getSolverManager()->getDomainSolver(i)->prolongate(level, u, v, d);
  }
  _chronometer.stop("transfer");
}

/*--------------------------------------------------------------------------*/

SolverManager* VisitorSolverManagerMultiLevel::getSolverManager()
{
  return _solvermanager;
}

const SolverManager* VisitorSolverManagerMultiLevel::getSolverManager() const
{
  return _solvermanager;
}

/*--------------------------------------------------------------------------*/
int VisitorSolverManagerMultiLevel::getMaxLevel() const
{
  return _maxlevel;
}

int VisitorSolverManagerMultiLevel::getMinLevel() const
{
  return _nlevelsignore;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::newVector(Alat::GhostVector* u)
{
  u->setDescription("somevars");
  u->setVariables(_variables);
  // std::cerr << "VisitorSolverManagerMultiLevel::newVector() u="<<*u<<"\n";
  for(int i = 0; i < getSolverManager()->getNDomainSolvers(); i++)
  {
    getSolverManager()->getDomainSolver(i)->registerVector(*u);
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::vectorZero(int level, Alat::GhostVector& v) const
{
  _chronometer.start("vectorOperations");
  _visitors[level]->vectorZero(v);
  _chronometer.stop("vectorOperations");
}

/*--------------------------------------------------------------------------*/
double VisitorSolverManagerMultiLevel::vectorNorm(int level, const Alat::GhostVector& r) const
{
  _chronometer.start("vectorOperations");
  double d = _visitors[level]->vectorNorm(r);
  _chronometer.stop("vectorOperations");
  return d;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::vectorEqual(int level, Alat::GhostVector& r, const Alat::GhostVector& f) const
{
  _chronometer.start("vectorOperations");
  _visitors[level]->vectorEqual(r, f);
  _chronometer.stop("vectorOperations");
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::vectorAdd(int level, Alat::GhostVector& p, double d, const Alat::GhostVector& q) const
{
  _chronometer.start("vectorOperations");
  _visitors[level]->vectorAdd(p, d, q);
  _chronometer.stop("vectorOperations");
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerMultiLevel::matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const
{
  _chronometer.start("matrixVectorProduct");
  _visitors[level]->matrixVectorProduct(A, r, u, d);
  _chronometer.stop("matrixVectorProduct");
}
