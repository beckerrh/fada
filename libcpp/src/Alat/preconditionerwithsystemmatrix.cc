#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerwithsystemmatrix.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
PreconditionerWithSystemMatrix::~PreconditionerWithSystemMatrix(){}
PreconditionerWithSystemMatrix::PreconditionerWithSystemMatrix(const Alat::SystemMatrixInterface* matrix) : Alat::Preconditioner(), _matrix(matrix){}
PreconditionerWithSystemMatrix::PreconditionerWithSystemMatrix( const PreconditionerWithSystemMatrix& preconditionerwithmatrix) : Alat::Preconditioner(preconditionerwithmatrix)
{
  assert(0);
}
PreconditionerWithSystemMatrix& PreconditionerWithSystemMatrix::operator=( const PreconditionerWithSystemMatrix& preconditionerwithmatrix)
{
  Alat::Preconditioner::operator=(preconditionerwithmatrix);
  assert(0);
  return *this;
}
std::string PreconditionerWithSystemMatrix::getName() const
{
  return "PreconditionerWithSystemMatrix";
}
PreconditionerWithSystemMatrix* PreconditionerWithSystemMatrix::clone() const
{
  assert(0);
//return new PreconditionerWithSystemMatrix(*this);
}

/*--------------------------------------------------------------------------*/
void PreconditionerWithSystemMatrix::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  Preconditioner::basicInit(parameterfile, blockname, visitor);
  _visitorsystem = dynamic_cast<const Alat::IterativeSolverVisitorWithVectorInterface*>( _visitor );
  assert(_visitorsystem);
}

/*--------------------------------------------------------------------------*/
Alat::SystemVector* PreconditionerWithSystemMatrix::getMemory(int i) const
{
  return _visitorsystem->getVector( Preconditioner::getMemory(i) );
}

/*--------------------------------------------------------------------------*/
void PreconditionerWithSystemMatrix::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
	//   std::cerr << "PreconditionerWithSystemMatrix::solveApproximate() " <<  getVisitor()->getName() << "u="<< u << "f="<< f << "\n";
	// std::cerr << "visitot " << _visitorsystem->getName() << "\n";
	assert(_visitorsystem->getVector(f));
  _solveApproximate( status, _visitorsystem->getVector(u), _visitorsystem->getVector(f), iteration );
}
