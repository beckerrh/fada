#include  "Fada/chronometer.h"
#include  "Fada/domainsolverinterface.h"
#include  "Alat/ghostlinearsolver.h"
#include  "Alat/ghostmatrix.h"
#include  "Alat/ghostvector.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "Alat/matrixallvariables.h"
#include  "Fada/multilevelvector.h"
#include  "Alat/preconditionerinterface.h"
#include  "Fada/solvermanager.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/doublevector.h"
#include  "Fada/multilevellinearsolver.h"
#include  <cassert>
#include  <sstream>
#ifdef  CLANG
#include  <algorithm>
#include  <numeric>
#else
#include  <ext/algorithm>
#include  <ext/numeric>
#endif

using namespace Fada;

/*--------------------------------------------------------------------------*/
VisitorSolverManagerOneLevel::~VisitorSolverManagerOneLevel()
{}

VisitorSolverManagerOneLevel::VisitorSolverManagerOneLevel( SolverManager* solvermanager, int level, const Alat::StringSet& variables, bool integration) : Alat::IterativeSolverVisitorInterface(), _solvermanager(solvermanager), _variables(variables), _level(level),  _integration(integration), _linearizationpoint(NULL)
{
}

VisitorSolverManagerOneLevel::VisitorSolverManagerOneLevel( const VisitorSolverManagerOneLevel& visitoronelevel) : Alat::IterativeSolverVisitorInterface(visitoronelevel), _variables(visitoronelevel._variables)
{
  assert(0);
}

VisitorSolverManagerOneLevel& VisitorSolverManagerOneLevel::operator=( const VisitorSolverManagerOneLevel& visitoronelevel)
{
  Alat::IterativeSolverVisitorInterface::operator=(visitoronelevel);
  assert(0);
  return *this;
}

std::string VisitorSolverManagerOneLevel::getName() const
{
  std::stringstream ss;
  ss << "VisSmOlAv_"<<_level << "_" << _variables;
  return ss.str();
}

VisitorSolverManagerOneLevel* VisitorSolverManagerOneLevel::clone() const
{
  return new VisitorSolverManagerOneLevel(*this);
}
const Alat::StringSet& VisitorSolverManagerOneLevel::getVariables() const
{
  return _variables;
}

SolverManager* VisitorSolverManagerOneLevel::getSolverManager()
{
  return _solvermanager;
}

const SolverManager* VisitorSolverManagerOneLevel::getSolverManager() const
{
  return _solvermanager;
}

const DomainSolverInterface* VisitorSolverManagerOneLevel::getDomainSolver(int idomain) const
{
  return _solvermanager->getDomainSolver(idomain);
}

DomainSolverInterface* VisitorSolverManagerOneLevel::getDomainSolver(int idomain)
{
  return _solvermanager->getDomainSolver(idomain);
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::newVector(Alat::GhostVector* u)
{
  u->setVariables(_variables);
  u->setDescription("somevars");
  // std::cerr << "VisitorSolverManagerOneLevel::newVector() u="<<*u<<"\n";
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    getDomainSolver(idomain)->registerVector(*u);
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::setsmoothtype(const Alat::GhostLinearSolver& linearsolverdomain, std::string type)
{
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    Alat::LinearSolverInterface* linearsolver = getDomainSolver(idomain)->getLinearSolver(linearsolverdomain);
    // std::cerr << "linearsolver="<<linearsolver->getName()<<"\n";
    Fada::MultiLevelLinearSolver* multilevellinearsolver = dynamic_cast<Fada::MultiLevelLinearSolver*>( linearsolver );
    assert(multilevellinearsolver);
    Alat::IterativeSolverWithPreconditioner*  iterativesolverwithpreconditioner =  dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( multilevellinearsolver->getLinearSolver(_level) );
    // std::cerr << "iterativesolverwithprecondition="<<iterativesolverwithpreconditioner->getName()<<"\n";
    if(iterativesolverwithpreconditioner)
    {
      // std::cerr << "linearsolver->getPreconditioner()="<<iterativesolverwithpreconditioner->getPreconditioner()->getName()<<"\n";
      iterativesolverwithpreconditioner->getPreconditioner()->setsmoothtype(type);
    }
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{}

/*--------------------------------------------------------------------------*/
//
// for DirectSolver
//
Alat::SystemVector* VisitorSolverManagerOneLevel::getDomainVector(int idomain, Alat::GhostVector& u) const
{
  return getDomainSolver(idomain)->getVector(_level, u);
}

const Alat::SystemVector* VisitorSolverManagerOneLevel::getDomainVector(int idomain, const Alat::GhostVector& u) const
{
  return getDomainSolver(idomain)->getVector(_level, u);
}

// const Alat::IntVector& VisitorSolverManagerOneLevel::getDomainsPermutation(int iteration) const
// {
//   return getSolverManager()->getDomainsPermutation(iteration);
// }

/*--------------------------------------------------------------------------*/

int VisitorSolverManagerOneLevel::getVectorLevel() const
{
  return _level;
}

/*--------------------------------------------------------------------------*/
Alat::SystemVector* VisitorSolverManagerOneLevel::getVector(int idomain, Alat::GhostVector& gu) const
{
  Fada::MultiLevelVector* umg = dynamic_cast<Fada::MultiLevelVector*>( getDomainSolver(idomain)->getVector(gu) );
  if(umg)
  {
    return umg->getVector(_level);
  }
  else
  {
    Alat::SystemVector* us = dynamic_cast<Alat::SystemVector*>( getDomainSolver(idomain)->getVector(gu) );
    assert(us);
    return us;
  }
}

const Alat::SystemVector* VisitorSolverManagerOneLevel::getVector(int idomain, const Alat::GhostVector& gu) const
{
  const Fada::MultiLevelVector* umg = dynamic_cast<const Fada::MultiLevelVector*>( getDomainSolver(idomain)->getVector(gu) );
  if(umg)
  {
    return umg->getVector(_level);
  }
  else
  {
    const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( getDomainSolver(idomain)->getVector(gu) );
    assert(us);
    return us;
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::vectorEqual(Alat::GhostVector& gp, const Alat::GhostVector& gq) const
{
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    getVector(idomain, gp)->equal( getVector(idomain, gq) );
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::vectorZero(Alat::GhostVector& gp) const
{
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    getVector(idomain, gp)->zeros();
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::vectorAdd(Alat::GhostVector& gp, double d, const Alat::GhostVector& gq) const
{
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    getVector(idomain, gp)->add( d, getVector(idomain, gq) );
  }
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::vectorScale(Alat::GhostVector& gp, double d) const
{
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    getVector(idomain, gp)->scale(d);
  }
}

/*--------------------------------------------------------------------------*/
double VisitorSolverManagerOneLevel::scalarProduct(const Alat::GhostVector& gp, const Alat::GhostVector& gq) const
{
  _chronometer.start("scalarProduct");
  double d = 0.0;
  for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
  {
    d += getVector(idomain, gp)->scalarProduct( getVector(idomain, gq) );
  }
  // std::cerr << "VisitorSolverManagerOneLevel::scalarProduct() " << d << "\n";
  _chronometer.stop("scalarProduct");
  return d;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::setLinearizationPoint(const Alat::GhostVector& linearizationpoint)
{
  // std::cerr << "VisitorSolverManagerOneLevel::setLinearizationPoint() _level="<<_level<<"\n";
  _linearizationpoint = &linearizationpoint;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const
{
  // std::cerr << "VisitorSolverManagerOneLevel::matrixVectorProduct() A="<<A << " r="<<r <<" u="<<u<<"\n";
  _chronometer.start("matrixVectorProduct");

  if(_integration)
  {
    assert(0);
    assert(_linearizationpoint);
    AlatEnums::residualstatus status;
    vectorScale(r, -1.0/d);
    assert(_level==r.getLevel());
    assert(_level==u.getLevel());
    getSolverManager()->computeLinearization(status, r, *_linearizationpoint, u);
    vectorScale(r, -d);
  }
  else
  {
// #ifndef  NOOPENMP
//   #pragma omp parallel for
// #endif
    for(int idomain = 0; idomain < getSolverManager()->getNDomainSolvers(); idomain++)
    {
      getDomainSolver(idomain)->matrixVectorProduct(_level, A, r, u, d);
    }
  }
  // for(int i = 0; i < getSolverManager()->getNCouplingSolvers(); i++)
  // {
  //   getSolverManager()->getCouplingSolver(i)->matrixVectorProduct(_level, A, r, u, d);
  // }
  _chronometer.stop("matrixVectorProduct");
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerOneLevel::solveOnDomain(int idomain, const Alat::GhostLinearSolver& linearsolverdomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  Fada::MultiLevelLinearSolver* multilevellinearsolver = dynamic_cast< Fada::MultiLevelLinearSolver*>( getDomainSolver(idomain)->getLinearSolver(linearsolverdomain) );
  assert(multilevellinearsolver);
  multilevellinearsolver->restart();
  AlatEnums::iterationstatus status;
  multilevellinearsolver->getLinearSolver(_level)->solve(status, ghostmatrix, u, f);
}

void VisitorSolverManagerOneLevel::vectorEqualOnDomain(int idomain, Alat::GhostVector& gp, const Alat::GhostVector& gq) const
{
  getVector(idomain, gp)->equal( getVector(idomain, gq) );
}

// void VisitorSolverManagerOneLevel::matrixVectorProductCoupling(int idomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f, double d) const
// {
//   getSolverManager()->matrixVectorProductCoupling(idomain, _level, ghostmatrix, u, f, d);
// }
