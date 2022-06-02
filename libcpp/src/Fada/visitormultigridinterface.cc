#include  "Fada/visitormultigridinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

VisitorMultigridInterface::~VisitorMultigridInterface()
{}

VisitorMultigridInterface::VisitorMultigridInterface() : Alat::IterativeSolverVisitorInterface()
{}

VisitorMultigridInterface::VisitorMultigridInterface( const VisitorMultigridInterface& multigridsolvervisitorinterface) : Alat::IterativeSolverVisitorInterface(multigridsolvervisitorinterface)
{
  assert(0);
}

VisitorMultigridInterface& VisitorMultigridInterface::operator=( const VisitorMultigridInterface& multigridsolvervisitorinterface)
{
  Alat::IterativeSolverVisitorInterface::operator=(multigridsolvervisitorinterface);
  assert(0);
  return *this;
}

std::string VisitorMultigridInterface::getName() const
{
  return "VisitorMultigridInterface";
}

VisitorMultigridInterface* VisitorMultigridInterface::clone() const
{
  assert(0);
  // return new VisitorMultigridInterface(*this);
}

std::string VisitorMultigridInterface::getInterfaceName() const
{
  return "VisitorMultigridInterface ";
}

/*-------------------------------------------------------------*/

std::string VisitorMultigridInterface::getVectorType() const
{
  return "unknowns";
}
int VisitorMultigridInterface::getVectorLevel() const
{
  return -1;
}

void VisitorMultigridInterface::setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& f) const
{
  _notWritten("setMultiGridVector");
}

void VisitorMultigridInterface::setVector(int level, Alat::GhostVector& f, const Alat::GhostVector& mgf) const
{
  _notWritten("setVector");
}

void VisitorMultigridInterface::vectorZero(int level, Alat::GhostVector& v) const
{
  _notWritten("vectorZero");
}

double VisitorMultigridInterface::vectorNorm(int level, const Alat::GhostVector& r) const
{
  _notWritten("vectorNorm");
  return 0.0;
}

double VisitorMultigridInterface::scalarProduct(int level, const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const
{
  _notWritten("vectorNorm");
  return 0.0;
}

void VisitorMultigridInterface::vectorEqual(int level, Alat::GhostVector& r, const Alat::GhostVector& f) const
{
  _notWritten("vectorEqual");
}

void VisitorMultigridInterface::vectorAdd(int level, Alat::GhostVector& p, double d, const Alat::GhostVector& q) const
{
  _notWritten("vectorAdd");
}

void VisitorMultigridInterface::matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const
{
  _notWritten("matrixVectorProduct");
}

void VisitorMultigridInterface::residual(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, const Alat::GhostVector& b) const
{
  vectorEqual(level, r, b);
  matrixVectorProduct(level, A, r, u, -1.0);
}

int VisitorMultigridInterface::getMinLevel() const
{
  _notWritten("getMinLevel");
  return 0;
}

int VisitorMultigridInterface::getMaxLevel() const
{
  _notWritten("getMaxLevel");
  return 0;
}

void VisitorMultigridInterface::restrict(int level, Alat::GhostVector& f, const Alat::GhostVector& v ) const
{
  _notWritten("restrict");
}

void VisitorMultigridInterface::prolongate(int level, Alat::GhostVector& u, const Alat::GhostVector& v, double d) const
{
  _notWritten("prolongate");
}

void VisitorMultigridInterface::setLevel(int level) const
{
  _notWritten("setLevel");
}
