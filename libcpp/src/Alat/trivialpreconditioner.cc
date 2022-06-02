#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/trivialpreconditioner.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/

TrivialPreconditioner::~TrivialPreconditioner()
{}

TrivialPreconditioner::TrivialPreconditioner() : Alat::Preconditioner()
{}

TrivialPreconditioner::TrivialPreconditioner( const TrivialPreconditioner& trivialpreconditioner) : Alat::Preconditioner(trivialpreconditioner)
{
  assert(0);
}

TrivialPreconditioner& TrivialPreconditioner::operator=( const TrivialPreconditioner& trivialpreconditioner)
{
  Alat::Preconditioner::operator=(trivialpreconditioner);
  assert(0);
  return *this;
}

std::string TrivialPreconditioner::getName() const
{
  return "TrivialPreconditioner";
}

TrivialPreconditioner* TrivialPreconditioner::clone() const
{
  assert(0);
//return new TrivialPreconditioner(*this);
}

/*--------------------------------------------------------------------------*/
void TrivialPreconditioner::setsmoothtype(std::string smoothtype)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

int TrivialPreconditioner::getNVectors() const
{
  return 0;
}

void TrivialPreconditioner::reInit()
{}
void TrivialPreconditioner::computePreconditioner()
{}
void TrivialPreconditioner::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  getVisitor()->vectorEqual(u, f);
}
