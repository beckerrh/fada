#include  "Alat/iterativesolvervisitorinterface.h"
#include  <cassert>
#include  <iostream>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/
IterativeSolverVisitorInterface::~IterativeSolverVisitorInterface()
{}
IterativeSolverVisitorInterface::IterativeSolverVisitorInterface() : Alat::InterfaceBase()
{}
IterativeSolverVisitorInterface::IterativeSolverVisitorInterface( const IterativeSolverVisitorInterface& iterativesolvervisitorinterface) : Alat::InterfaceBase(iterativesolvervisitorinterface)
{
  assert(0);
}

IterativeSolverVisitorInterface& IterativeSolverVisitorInterface::operator=( const IterativeSolverVisitorInterface& iterativesolvervisitorinterface)
{
  InterfaceBase::operator=(iterativesolvervisitorinterface);
  assert(0);
  return *this;
}

std::string IterativeSolverVisitorInterface::getInterfaceName() const
{
  return "IterativeSolverVisitorInterface";
}

std::string IterativeSolverVisitorInterface::getName() const
{
  return "IterativeSolverVisitorInterface";
}

IterativeSolverVisitorInterface* IterativeSolverVisitorInterface::clone() const
{
  assert(0);
  // return new IterativeSolverVisitorInterface(*this);
}

/*-------------------------------------------------------------*/
// const Alat::IntVector& IterativeSolverVisitorInterface::getNcompOfVars() const
// {
//   _notWritten("getNcompOfVars");
// }
//
// const Alat::IntVector& IterativeSolverVisitorInterface::getNOfVars() const
// {
//   _notWritten("getNcompOfVars");
// }
//
// int IterativeSolverVisitorInterface::getNDomainSolvers() const
// {
//   _notWritten("getNDomainSolvers");
// }
//
// int IterativeSolverVisitorInterface::getNCouplingSolvers() const
// {
//   _notWritten("getNCouplingSolvers");
// }

// Alat::SystemVector* IterativeSolverVisitorInterface::getDomainVector(int i, Alat::GhostVector& u) const
// {
//   _notWritten("getDomainVector");
// }
//
// const Alat::SystemVector* IterativeSolverVisitorInterface::getDomainVector(int i, const Alat::GhostVector& u) const
// {
//   _notWritten("getDomainVector");
// }
//
// const SystemMatrixInterface* IterativeSolverVisitorInterface::getDomainMatrix(int i, const Alat::GhostMatrix& A) const
// {
//   _notWritten("getDomainMatrix");
// }
//
// const SystemMatrixInterface* IterativeSolverVisitorInterface::getCouplingMatrixLeftRight(int i, const Alat::GhostMatrix& A) const
// {
//   _notWritten("getCouplingMatrixLeftRight");
// }
//
// const SystemMatrixInterface* IterativeSolverVisitorInterface::getCouplingMatrixRightLeft(int i, const Alat::GhostMatrix& A) const
// {
//   _notWritten("getCouplingMatrixRightLeft");
// }

// int IterativeSolverVisitorInterface::getLeftSolverIndexOfCouplingSolver(int i) const
// {
//   _notWritten("getLeftSolverIndexOfCouplingSolver");
// }
//
// int IterativeSolverVisitorInterface::getRightSolverIndexOfCouplingSolver(int i) const
// {
//   _notWritten("getRightSolverIndexOfCouplingSolver");
// }

// int IterativeSolverVisitorInterface::getNvarsOfDomain(int idomain) const
// {
//   _notWritten("getNvarsOfDomain");
// }
//
// int IterativeSolverVisitorInterface::getNcompOfVar(int idomain, int ivar) const
// {
//   _notWritten("getNcompOfVar");
// }
//
// int IterativeSolverVisitorInterface::getNOfVar(int idomain, int ivar) const
// {
//   _notWritten("getNOfVar");
// }

std::string IterativeSolverVisitorInterface::getVectorType() const
{
  return "unknowns";
}

void IterativeSolverVisitorInterface::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _notWritten("basicInit");
}

void IterativeSolverVisitorInterface::vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const
{
  _notWritten("vectorEqual");
}

void IterativeSolverVisitorInterface::vectorZero(Alat::GhostVector& v) const
{
  _notWritten("vectorZero");
}

void IterativeSolverVisitorInterface::vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const
{
  _notWritten("vectorAdd");
}

void IterativeSolverVisitorInterface::vectorScale(Alat::GhostVector& r, double d) const
{
  _notWritten("vectorScale");
}

void IterativeSolverVisitorInterface::matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const
{
  _notWritten("matrixVectorProduct");
}

void IterativeSolverVisitorInterface::postProcess(Alat::GhostVector& u) const
{}

/*-------------------------------------------------------------*/

double IterativeSolverVisitorInterface::scalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const
{
  _notWritten("scalarProduct");
  return 0.0;
}

/*-------------------------------------------------------------*/

double IterativeSolverVisitorInterface::vectorNorm(const Alat::GhostVector& r) const
{
  double d = scalarProduct(r, r);
  if(d != d)
  {
    assert(0);
  }
  return sqrt( d );
}

/*-------------------------------------------------------------*/

void IterativeSolverVisitorInterface::residual(const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  vectorEqual(r, f);
  matrixVectorProduct(A, r, u, -1.0);
}

/*-------------------------------------------------------------*/

std::ostream& IterativeSolverVisitorInterface::printLoopInformation(std::ostream& os) const
{
  os << "\"" <<getName() <<"\"";
  return os;
}

// /*-------------------------------------------------------------*/
//
// void IterativeSolverVisitorInterface::matrixVectorProduct(Alat::SystemVector* r, const Alat::SystemVector* u, double d) const
// {
//   _notWritten("matrixVectorProduct(Alat::SystemVector*)");
// }

/*-------------------------------------------------------------*/
const Alat::IntVector& IterativeSolverVisitorInterface::getDomainsPermutation(int iteration) const
{
  _notWritten("getDomainsPermutation");
}

void IterativeSolverVisitorInterface::solveOnDomain(int idomain, const Alat::GhostLinearSolver& linearsolverdomain, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _notWritten("solveOnDomain");
}

void IterativeSolverVisitorInterface::vectorEqualOnDomain(int idomain, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _notWritten("vectorEqualOnDomain");
}

void IterativeSolverVisitorInterface::matrixVectorProductCoupling(int i, const Alat::GhostMatrix& ghostmatrix, Alat::GhostVector& u, const Alat::GhostVector& f, double d) const
{
  _notWritten("matrixVectorProductCoupling");
}
