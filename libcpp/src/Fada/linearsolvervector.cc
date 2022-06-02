#include  "Fada/linearsolvervector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearSolverVector::~LinearSolverVector()
{}

LinearSolverVector::LinearSolverVector() : LinearSolverVectorInterface(), Alat::Vector<Alat::LinearSolverInterface*>()
{}

LinearSolverVector::LinearSolverVector( const LinearSolverVector& linearsolvervector) : LinearSolverVectorInterface(linearsolvervector), Alat::Vector<Alat::LinearSolverInterface*>(linearsolvervector)
{
  assert(0);
}

LinearSolverVector& LinearSolverVector::operator=( const LinearSolverVector& linearsolvervector)
{
  LinearSolverVectorInterface::operator=(linearsolvervector);
  assert(0);
  return *this;
}

std::string LinearSolverVector::getName() const
{
  return "LinearSolverVector";
}

LinearSolverVector* LinearSolverVector::clone() const
{
  assert(0);
//return new LinearSolverVector(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& LinearSolverVector::printLoopInformation(std::ostream& os) const
{
  for( int level = 0; level < getNLevels(); level++ )
  {
    getLinearSolver( level)->printLoopInformation(os);
  }
  return os;
}

/*--------------------------------------------------------------------------*/
int LinearSolverVector::getNLevels() const
{
  return size();
}

void LinearSolverVector::restart()
{
  for( int level = 0; level < getNLevels(); level++ )
  {
    ( *this )[level]->restart();
  }
}

void LinearSolverVector::reInit(int nlevels)
{
  Alat::Vector<Alat::LinearSolverInterface*>::set_size(nlevels);
}

Alat::LinearSolverInterface*& LinearSolverVector::getLinearSolverPointer(int level)
{
  return ( *this )[level];
}

Alat::LinearSolverInterface* LinearSolverVector::getLinearSolver(int level)
{
  return ( *this )[level];
}

const Alat::LinearSolverInterface* LinearSolverVector::getLinearSolver(int level) const
{
  return ( *this )[level];
}

bool LinearSolverVector::allSolversSet() const
{
  for( int level = 0; level < getNLevels(); level++ )
  {
    if( ( *this )[level] == NULL )
    {
      return false;
    }
  }
  return true;
}

void LinearSolverVector::reInit()
{
  for(int level = 0; level < size(); level++)
  {
    assert( ( *this )[level] );
    ( *this )[level]->reInit();
  }
}

void LinearSolverVector::compute()
{
  for(int level = 0; level < size(); level++)
  {
    // std::cerr << "LinearSolverVector::compute() "<<   ( *this )[level]->getName()<<"\n";
    ( *this )[level]->compute();
  }
}
