#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerwithvariablematrix.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
PreconditionerWithVariableMatrix::~PreconditionerWithVariableMatrix(){}
PreconditionerWithVariableMatrix::PreconditionerWithVariableMatrix(const Alat::VariableMatrixInterface* matrix) : Alat::Preconditioner(), _matrix(matrix){}
PreconditionerWithVariableMatrix::PreconditionerWithVariableMatrix( const PreconditionerWithVariableMatrix& preconditionerwithmatrix) : Alat::Preconditioner(preconditionerwithmatrix)
{
  assert(0);
}
PreconditionerWithVariableMatrix& PreconditionerWithVariableMatrix::operator=( const PreconditionerWithVariableMatrix& preconditionerwithmatrix)
{
  Alat::Preconditioner::operator=(preconditionerwithmatrix);
  assert(0);
  return *this;
}
std::string PreconditionerWithVariableMatrix::getName() const
{
  return "PreconditionerWithVariableMatrix";
}
PreconditionerWithVariableMatrix* PreconditionerWithVariableMatrix::clone() const
{
  assert(0);
//return new PreconditionerWithVariableMatrix(*this);
}

/*--------------------------------------------------------------------------*/
void PreconditionerWithVariableMatrix::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{                          
  Preconditioner::basicInit(parameterfile, blockname, visitor);
  _visitordomainsolver = dynamic_cast<const Alat::IterativeSolverVisitorWithVectorInterface*>( _visitor );
  assert(_visitordomainsolver);
}

/*--------------------------------------------------------------------------*/
Alat::SystemVectorInterface* PreconditionerWithVariableMatrix::getMemory(int i) const
{
  assert(0);
  return _visitordomainsolver->getVector(Preconditioner::getMemory(i));
}

/*--------------------------------------------------------------------------*/
void PreconditionerWithVariableMatrix::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  assert(0);
  std::cerr << "PreconditionerWithVariableMatrix::solveApproximate() " <<  getVisitor()->getName() << "\n";
  // _solveApproximate( status, _visitordomainsolver->getVector(u), _visitordomainsolver->getVector(f), iteration );
}
