#include  "Fada/chronometer.h"
#include  "Fada/domainsolverinterface.h"
#include  "Alat/ghostvector.h"
#include  "Alat/matrixallvariables.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitordomainsolveronelevel.h"
#include  "Alat/doublevector.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
VisitorDomainSolverOneLevel::~VisitorDomainSolverOneLevel(){}

VisitorDomainSolverOneLevel::VisitorDomainSolverOneLevel(DomainSolverInterface* domainsolver, int level, const Alat::StringSet& variables) : IterativeSolverVisitorWithVectorInterface(), _domainsolver(domainsolver), _level(level), _variables(variables){}

VisitorDomainSolverOneLevel::VisitorDomainSolverOneLevel( const VisitorDomainSolverOneLevel& visitoronelevel) : IterativeSolverVisitorWithVectorInterface(visitoronelevel), _domainsolver(visitoronelevel._domainsolver), _level(visitoronelevel._level), _variables(visitoronelevel._variables)
{
  assert(0);
}

VisitorDomainSolverOneLevel& VisitorDomainSolverOneLevel::operator=( const VisitorDomainSolverOneLevel& visitoronelevel)
{
  IterativeSolverVisitorWithVectorInterface::operator=(visitoronelevel);
  assert(0);
  return *this;
}

std::string VisitorDomainSolverOneLevel::getName() const
{
  std::stringstream ss;
  // ss << "VisitorDomainSolverOneLevel_"<<_level;
  ss << "VisDsOlAv_"<<_level<<"_"<<_variables;
  return ss.str();
}

VisitorDomainSolverOneLevel* VisitorDomainSolverOneLevel::clone() const
{
  return new VisitorDomainSolverOneLevel(*this);
}

/*--------------------------------------------------------------------------*/
DomainSolverInterface* VisitorDomainSolverOneLevel::getDomainSolver()
{
  return _domainsolver;
}

const DomainSolverInterface* VisitorDomainSolverOneLevel::getDomainSolver() const
{
  return _domainsolver;
}

/*--------------------------------------------------------------------------*/
Alat::SystemVector* VisitorDomainSolverOneLevel::getVector(Alat::GhostVector& vector) const
{
  Alat::SystemVector* vectori = dynamic_cast<Alat::SystemVector*>( getDomainSolver()->getVector(vector) );
  Fada::MultiLevelVector* mlvector = dynamic_cast<Fada::MultiLevelVector*>( getDomainSolver()->getVector(vector)  );
  if(mlvector)
  {
    return mlvector->getVector(_level);
  }
  else
  {
    return vectori;
  }
}

const Alat::SystemVector* VisitorDomainSolverOneLevel::getVector(const Alat::GhostVector& vector) const
{
  // std::cerr << "VisitorDomainSolverOneLevel::getVector() vector " << vector << "\n";
  const Alat::SystemVector* vectori = dynamic_cast<const Alat::SystemVector*>( getDomainSolver()->getVector(vector) );
  const Fada::MultiLevelVector* mlvector = dynamic_cast<const Fada::MultiLevelVector*>( getDomainSolver()->getVector(vector) );
  if(mlvector)
  {
		assert(mlvector->getVector(_level));
	  // std::cerr << "VisitorDomainSolverOneLevel::getVector() _level " << _level << " name=" << mlvector->getVector(_level)->getName() << "\n";
    return mlvector->getVector(_level);
  }
  else
  {
		assert(vectori);
    return vectori;
  }
}

Alat::SystemVector* VisitorDomainSolverOneLevel::getVectorVariables(Alat::GhostVector& u) const
{
  assert( _variables == u.getVariables() );
  Alat::SystemVector* vector = dynamic_cast<Alat::SystemVector*>( getDomainSolver()->getVector(u) );
  assert(vector);
  return vector;
}

const Alat::SystemVector* VisitorDomainSolverOneLevel::getVectorVariables(const Alat::GhostVector& u) const
{
  assert( _variables == u.getVariables() );
  const Alat::SystemVector* vector = dynamic_cast<const Alat::SystemVector*>( getDomainSolver()->getVector(u) );
  assert(vector);
  return vector;
}

const Alat::MatrixAllVariables* VisitorDomainSolverOneLevel::getMatrixVariables(const Alat::GhostMatrix& A) const
{
  const Alat::MatrixAllVariables* matrix = dynamic_cast<const Alat::MatrixAllVariables*>( getDomainSolver()->getMatrix(_level, A) );
  assert(matrix);
  return matrix;
}

/*--------------------------------------------------------------------------*/
int VisitorDomainSolverOneLevel::level() const
{
  return _level;
}

int VisitorDomainSolverOneLevel::nlevels() const
{
  return getDomainSolver()->getNLevels();
}

/*--------------------------------------------------------------------------*/
int VisitorDomainSolverOneLevel::getVectorLevel() const
{
  return _level;
}

void VisitorDomainSolverOneLevel::newVector(Alat::GhostVector* u)
{
  u->setDescription("somevars");
  u->setVariables(_variables);
  getDomainSolver()->registerVector(*u);
}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::vectorEqual(Alat::GhostVector& p, const Alat::GhostVector& q) const
{
  Alat::SystemVector* pa = getVectorVariables(p);
  const Alat::SystemVector* qa = getVectorVariables(q);
  pa->equal(qa);
}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::vectorZero(Alat::GhostVector& v) const
{
  Alat::SystemVector* va = getVectorVariables(v);
  va->zeros();
}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const
{
  Alat::SystemVector* pa = getVectorVariables(p);
  const Alat::SystemVector* qa = getVectorVariables(q);
  pa->add(d, qa);
}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::vectorScale(Alat::GhostVector& r, double d) const
{
  Alat::SystemVector* ra = getVectorVariables(r);
  ra->scale(d);
}

/*--------------------------------------------------------------------------*/
double VisitorDomainSolverOneLevel::scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const
{
  _chronometer.start("scalarProduct");
  const Alat::SystemVector* ra = getVectorVariables(r);
  const Alat::SystemVector* pa = getVectorVariables(p);
  double d = ra->scalarProduct(pa);
  if(d != d)
  {
    std::cerr << "VisitorDomainSolverOneLevel::scalarProduct() is Nan !\n";
    std::cerr << "r = " << *ra <<"\n";
    std::cerr << "p = " << *pa <<"\n";
    assert(0);
  }
  _chronometer.stop("scalarProduct");
  return d;
}

/*--------------------------------------------------------------------------*/
void VisitorDomainSolverOneLevel::matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const
{
  _chronometer.start("matrixVectorProduct");
  bool integration = false;
  if(integration)
  {
    assert(0);
    AlatEnums::residualstatus status;
    vectorScale(r, -1.0/d);
    getDomainSolver()->constructForm(status, r, u);
    vectorScale(r, -d);
  }
  else
  {
    const Alat::SystemVector* ua = getVectorVariables(u);
    Alat::SystemVector* ra = getVectorVariables(r);
    const Alat::MatrixAllVariables* matrix = getMatrixVariables(A);
    matrix->matrixVectorProduct(ra, ua, d);
  }
  _chronometer.stop("matrixVectorProduct");
}
