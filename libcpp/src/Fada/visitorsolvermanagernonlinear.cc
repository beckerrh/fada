#include  "Fada/domainsolverinterface.h"
#include  "Alat/iterationinfo.h"
#include  "Fada/solvermanager.h"
#include  "Fada/visitorsolvermanagernonlinear.h"
#include  "Alat/doublevector.h"
#include  "Alat/stringdoublemap.h"
#include  <cassert>
#include  <cmath>

using namespace Fada;

/*--------------------------------------------------------------------------*/

VisitorSolverManagerNonlinear::~VisitorSolverManagerNonlinear()
{}

VisitorSolverManagerNonlinear::VisitorSolverManagerNonlinear(SolverManager* solvermanager, int level) : Alat::NonlinearSolverVisitorInterface(), _solvermanager(solvermanager), _level(level)
{}

VisitorSolverManagerNonlinear::VisitorSolverManagerNonlinear( const VisitorSolverManagerNonlinear& visitorsolvermanagernonlinear) : Alat::NonlinearSolverVisitorInterface(visitorsolvermanagernonlinear)
{
  ( *this ).operator=(visitorsolvermanagernonlinear);
}

VisitorSolverManagerNonlinear& VisitorSolverManagerNonlinear::operator=( const VisitorSolverManagerNonlinear& visitorsolvermanagernonlinear)
{
  Alat::NonlinearSolverVisitorInterface::operator=(visitorsolvermanagernonlinear);
  assert(0);
  return *this;
}

std::string VisitorSolverManagerNonlinear::getName() const
{
  return "VisitorSolverManagerNonlinear";
}

VisitorSolverManagerNonlinear* VisitorSolverManagerNonlinear::clone() const
{
  return new VisitorSolverManagerNonlinear(*this);
}

/*--------------------------------------------------------------------------*/
const SolverManager* VisitorSolverManagerNonlinear::getSolverManager() const
{
  return _solvermanager;
}

SolverManager* VisitorSolverManagerNonlinear::getSolverManager()
{
  return _solvermanager;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerNonlinear::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{}

std::ostream& VisitorSolverManagerNonlinear::printLoopInformation(std::ostream& os) const
{
  assert(0);
}

int VisitorSolverManagerNonlinear::getVectorLevel() const
{
  return _level;
}

void VisitorSolverManagerNonlinear::newVector(Alat::GhostVector* u)
{
  getSolverManager()->registerVector(*u);
}

/*--------------------------------------------------------------------------*/
double VisitorSolverManagerNonlinear::residual(AlatEnums::residualstatus& status, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  // std::cerr << "VisitorSolverManagerNonlinear::residual() u=" << getSolverManager()->vectorNorm(u) << "\n"; 
  double normf = getSolverManager()->vectorNorm(f);
  // std::cerr << "VisitorSolverManagerNonlinear::residual() "<< " normf="<<normf<<"\n";
  normf = fmax(normf,1.0);
  getSolverManager()->vectorEqual(r, f);
  getSolverManager()->constructForm(status, r, u);
  // Alat::GhostMatrix A("A");
  // getSolverManager()->matrixVectorProduct(A, r, u, -1.0);
  // status = AlatEnums::ResidualStatusOk;
  // std::cerr << "VisitorSolverManagerNonlinear::residual() r=" << getSolverManager()->vectorNorm(r) << " normf="<<normf<<"\n";
  return getSolverManager()->vectorNorm(r)/normf;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerNonlinear::setLinearTolerance(double rtol, double gtol, Alat::GhostLinearSolver& B)
{
  getSolverManager()->getLinearSolver(B)->setTolerance(rtol, gtol);
}

/*--------------------------------------------------------------------------*/
int VisitorSolverManagerNonlinear::solve(AlatEnums::iterationstatus& status, const Alat::GhostLinearSolver& B, const Alat::GhostMatrix& A, Alat::GhostVector& du, const Alat::GhostVector& r) const
{
  getSolverManager()->vectorZero(du);
  return getSolverManager()->linearSolve(status, A, B, du, r);
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerNonlinear::vectorZero(Alat::GhostVector& u) const
{
  getSolverManager()->vectorZero(u);
}

void VisitorSolverManagerNonlinear::vectorAdd(Alat::GhostVector& u, double s, const Alat::GhostVector& du) const
{
  getSolverManager()->vectorAdd(u, s, du);
  getSolverManager()->newtonProject(u);
}

double VisitorSolverManagerNonlinear::vectorNorm(const Alat::GhostVector& r) const
{
  return getSolverManager()->vectorNorm(r);
}

double VisitorSolverManagerNonlinear::scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const
{
  return getSolverManager()->scalarProduct(r, p);
}

void VisitorSolverManagerNonlinear::vectorEqual(Alat::GhostVector& u, const Alat::GhostVector& w) const
{
  getSolverManager()->vectorEqual(u, w);
}

std::ostream& VisitorSolverManagerNonlinear::vectorWrite(std::ostream& os, const Alat::GhostVector& r) const
{
  getSolverManager()->vectorWrite(os, r);
  return os;
}

/*--------------------------------------------------------------------------*/
void VisitorSolverManagerNonlinear::constructMatrixAndLinearSolvers(AlatEnums::residualstatus& status, Alat::GhostLinearSolver& B, Alat::GhostMatrix& A,  Alat::GhostVector& u)
{
  getSolverManager()->matrixZero(A);
  getSolverManager()->constructJacobianMatrix(status, A, u);
  getSolverManager()->constructLinearSolver(B, u);
}

void VisitorSolverManagerNonlinear::computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& h, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  getSolverManager()->computeLinearization(status, h, u, du);
}

void VisitorSolverManagerNonlinear::setLavrentievParameter(double parameter) const
{
  getSolverManager()->setLavrentievParameter(parameter);
}

double VisitorSolverManagerNonlinear::computeNormSquared(AlatEnums::residualstatus& status, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  Alat::StringDoubleMap norms( u.getVariables() );
  getSolverManager()->computeNormSquared(status, norms, u, du);
  return norms.norm2();
}
