#include  "Alat/tokenize.h"
#include  "Alat/variablevector.h"
#include  "Alat/ilupreconditioner.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
IluPreconditioner::~IluPreconditioner(){}
IluPreconditioner::IluPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax) : PreconditionerWithSorting(matrix), _matrixilu(NULL), _type(type), _relax(relax){}
IluPreconditioner::IluPreconditioner( const IluPreconditioner& spaismoother) : PreconditionerWithSorting(spaismoother), _matrixilu(spaismoother._matrixilu)
{
  assert(0);
}

IluPreconditioner& IluPreconditioner::operator=( const IluPreconditioner& spaismoother)
{
  PreconditionerWithSorting::operator=(spaismoother);
  assert(0);
  return *this;
}

std::string IluPreconditioner::getName() const
{
  return "IluPreconditioner_"+_type;
}

IluPreconditioner* IluPreconditioner::clone() const
{
  assert(0);
//return new IluPreconditioner(*this);
}

int IluPreconditioner::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
void IluPreconditioner::computePreconditioner()
{
  sortIndices();
  _matrixilu->computeIlu(_relax, _matrix, _p, _pinv);
}

/*--------------------------------------------------------------------------*/
void IluPreconditioner::_solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const
{
  // u->zeros();
  _matrixilu->ilu(u, f, _relax, _p, _pinv);
}

/*--------------------------------------------------------------------------*/
void IluPreconditioner::reInit()
{
  PreconditionerWithSorting::reInit();  
  int ncomp = _matrix->getNComponents(); 
  std::string type("none"), storage;
  Alat::StringVector bouts = Alat::Tokenize(_type, ":");
  if(bouts.size() == 2)
  {
    assert(bouts[0] == "diagonal");
    type=bouts[0];
    storage = bouts[1];
  }
  else
  {
    storage = _type;
  }
  _matrixilu = _matrix->newIluMatrixAndReinit(ncomp, this, type, storage);
}
