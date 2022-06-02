#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Alat/stringset.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelMatrix::~MultiLevelMatrix()
{
  for(int i = 0; i < size(); i++)
  {
    if( ( *this )[i] )
    {
      delete ( *this )[i];
      ( *this )[i] = NULL;
    }
  }
}
MultiLevelMatrix::MultiLevelMatrix() : Alat::Vector<Alat::SystemMatrixInterface*>(), Alat::MatrixInterface(){}
MultiLevelMatrix::MultiLevelMatrix( const MultiLevelMatrix& multilevelmatrix) : Alat::Vector<Alat::SystemMatrixInterface*>(multilevelmatrix), Alat::MatrixInterface(multilevelmatrix)
{
  assert(0);
}
MultiLevelMatrix& MultiLevelMatrix::operator=( const MultiLevelMatrix& multilevelmatrix)
{
  MatrixInterface::operator=(multilevelmatrix);
  assert(0);
  return *this;
}
std::string MultiLevelMatrix::getName() const
{
  return "MultiLevelMatrix";
}
MultiLevelMatrix* MultiLevelMatrix::clone() const
{
  assert(0);
  return new MultiLevelMatrix(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelMatrix::set_size(int nlevels)
{
  Alat::Vector<Alat::SystemMatrixInterface*>::set_size(nlevels);
}

/*--------------------------------------------------------------------------*/
Alat::SystemMatrixInterface*& MultiLevelMatrix::getMatrixPointer(int level)
{
  return ( *this )[level];
}
Alat::SystemMatrixInterface* MultiLevelMatrix::getMatrix(int level)
{
  return ( *this )[level];
}
const Alat::SystemMatrixInterface* MultiLevelMatrix::getMatrix(int level) const
{
  return ( *this )[level];
}

/*--------------------------------------------------------------------------*/
int MultiLevelMatrix::nlevels() const
{
  return Alat::Vector<Alat::SystemMatrixInterface*>::size();
}

/*--------------------------------------------------------------------------*/
void MultiLevelMatrix::zeros()
{
  for(int level = 0; level < nlevels(); level++)
  {
    getMatrix(level)->zeros();
  }
}

/*--------------------------------------------------------------------------*/
std::ostream& MultiLevelMatrix::write(std::ostream& os) const
{
  for(int level = 0; level < nlevels(); level++)
  {
    os << "=== level " << level << " ===\n";
    getMatrix(level)->write(os);
  }
  return os;
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const MultiLevelMatrix& A)
{
  return A.write(os);
}

/*--------------------------------------------------------------------------*/
void MultiLevelMatrix::matrixVectorProduct(int level, Alat::SystemVectorInterface* f, const Alat::SystemVectorInterface* u, double d) const
{ 
  assert(0);
  // getMatrix(level)->matrixVectorProduct(f, u, d);
}

/*--------------------------------------------------------------------------*/
void MultiLevelMatrix::matrixVectorProduct(Fada::MultiLevelVector* f, const Fada::MultiLevelVector* u, double d) const
{
  assert(0);
  // for(int level = 0; level < nlevels(); level++)
  // {
  //   getMatrix(level)->matrixVectorProduct(f->getVector(level), u->getVector(level), d);
  // }
}
