#include  "Fada/chronometer.h"
#include  "Fada/domainsolverinterface.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Fada/domaindecomposition.h"
#include  "Fada/solvermanager.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/intvector.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainDecomposition::~DomainDecomposition()
{}

DomainDecomposition::DomainDecomposition(int level, Fada::SolverManager* solvermanager, const Alat::GhostMatrix& ghostmatrix, const Alat::GhostLinearSolver& linearsolverdomain) : Alat::Preconditioner(), _level(level), _solvermanager(solvermanager), _ghostmatrix(ghostmatrix), _linearsolverdomain(linearsolverdomain)
{}

DomainDecomposition::DomainDecomposition( const DomainDecomposition& preconditionerdomaindecomposition) : _linearsolverdomain(preconditionerdomaindecomposition._linearsolverdomain)
{
  assert(0);
}

DomainDecomposition& DomainDecomposition::operator=( const DomainDecomposition& preconditionerdomaindecomposition)
{
  Alat::Preconditioner::operator=(preconditionerdomaindecomposition);
  assert(0);
  return *this;
}

std::string DomainDecomposition::getName() const
{
  return "DomainDecomposition";
}

DomainDecomposition* DomainDecomposition::clone() const
{
  return new DomainDecomposition(*this);
}

/*--------------------------------------------------------------------------*/
int DomainDecomposition::getNVectors() const
{
  return 3;
}

/*--------------------------------------------------------------------------*/
void DomainDecomposition::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  Preconditioner::basicInit(parameterfile, blockname, visitor);
}

void DomainDecomposition::reInit()
{}
void DomainDecomposition::computePreconditioner()
{}

/*--------------------------------------------------------------------------*/
void DomainDecomposition::setsmoothtype(std::string smoothtype)
{}

/*--------------------------------------------------------------------------*/
void DomainDecomposition::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  // const Alat::IntVector& permutation = getVisitor()->getDomainsPermutation(iteration);

  Alat::GhostVector& v = getMemory(0);
    Alat::GhostVector& w = getMemory(1);
    // for(int i = 0; i < permutation.size(); i++)
    // {
    //   getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, f);
    // }
    getVisitor()->solveOnDomain(0, _linearsolverdomain, _ghostmatrix, u, f);
}
