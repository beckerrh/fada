#include  "Alat/tokenize.h"
#include  "Alat/variablevector.h"
#include  "Alat/spaipreconditioner.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/

SpaiPreconditioner::~SpaiPreconditioner()
{}

SpaiPreconditioner::SpaiPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax) : Alat::PreconditionerWithVariableMatrix(matrix), _matrixspai(NULL), _type(type), _relax(relax)
{
}

SpaiPreconditioner::SpaiPreconditioner( const SpaiPreconditioner& spaismoother) : Alat::PreconditionerWithVariableMatrix(spaismoother), _matrixspai(spaismoother._matrixspai)
{
  assert(0);
}

SpaiPreconditioner& SpaiPreconditioner::operator=( const SpaiPreconditioner& spaismoother)
{
  Alat::PreconditionerWithVariableMatrix::operator=(spaismoother);
  assert(0);
  return *this;
}

std::string SpaiPreconditioner::getName() const
{
  return "SpaiPreconditioner_"+_type;
}

SpaiPreconditioner* SpaiPreconditioner::clone() const
{
  assert(0);
//return new SpaiPreconditioner(*this);
}

int SpaiPreconditioner::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
void SpaiPreconditioner::computePreconditioner()
{
  _matrixspai->computeSpai(_relax, _matrix);
}

/*--------------------------------------------------------------------------*/
void SpaiPreconditioner::_solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const
{
  u->zeros();
  _matrixspai->matrixVectorProduct(u, f);
}

/*--------------------------------------------------------------------------*/
void SpaiPreconditioner::reInit()
{  
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
  // std::cerr << "SpaiPreconditioner::reInit( _type="<<_type<< " type "<<type << " storage " <<storage <<   " matrix " << _matrix->getName() << "\n";
  _matrixspai = _matrix->newSpaiMatrixAndReinit(ncomp, type, storage, _matrix);
}
