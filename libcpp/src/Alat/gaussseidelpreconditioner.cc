#include  "Alat/tokenize.h"
#include  "Alat/variablevector.h"
#include  "Alat/gaussseidelpreconditioner.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
GaussSeidelPreconditioner::~GaussSeidelPreconditioner(){}
GaussSeidelPreconditioner::GaussSeidelPreconditioner(const Alat::VariableMatrixInterface* matrix, std::string type, double relax) : PreconditionerWithSorting(matrix), _matrixgs(NULL), _type(type), _relax(relax){}
GaussSeidelPreconditioner::GaussSeidelPreconditioner( const GaussSeidelPreconditioner& spaismoother) : PreconditionerWithSorting(spaismoother), _matrixgs(spaismoother._matrixgs)
{
  assert(0);
}
GaussSeidelPreconditioner& GaussSeidelPreconditioner::operator=( const GaussSeidelPreconditioner& spaismoother)
{
  PreconditionerWithSorting::operator=(spaismoother);
  assert(0);
  return *this;
}
std::string GaussSeidelPreconditioner::getName() const
{
  return "GaussSeidelPreconditioner_"+_type;
}

GaussSeidelPreconditioner* GaussSeidelPreconditioner::clone() const
{
  assert(0);
//return new GaussSeidelPreconditioner(*this);
}

int GaussSeidelPreconditioner::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
void GaussSeidelPreconditioner::computePreconditioner()
{
  sortIndices();
  _matrixgs->computeGaussSeidel(_relax, _matrix);
}

/*--------------------------------------------------------------------------*/
void GaussSeidelPreconditioner::_solveApproximate(AlatEnums::iterationstatus& status, Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, int iteration) const
{
  // u->zeros();
  _matrixgs->gaussSeidel(u, f, _relax, _p, _pinv);
}

/*--------------------------------------------------------------------------*/
void GaussSeidelPreconditioner::reInit()
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
  _matrixgs = _matrix->newGaussSeidelMatrixAndReinit(ncomp, type, storage);
}
