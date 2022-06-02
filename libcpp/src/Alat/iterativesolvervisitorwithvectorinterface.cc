
#include  "Alat/iterativesolvervisitorwithvectorinterface.h"
#include  <cassert>
#include  <iostream>
#include  <math.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/
IterativeSolverVisitorWithVectorInterface::~IterativeSolverVisitorWithVectorInterface(){}
IterativeSolverVisitorWithVectorInterface::IterativeSolverVisitorWithVectorInterface() : Alat::IterativeSolverVisitorInterface(){}
IterativeSolverVisitorWithVectorInterface::IterativeSolverVisitorWithVectorInterface( const IterativeSolverVisitorWithVectorInterface& iterativesolvervisitorinterface) : Alat::IterativeSolverVisitorInterface(iterativesolvervisitorinterface)
{
  assert(0);
}

IterativeSolverVisitorWithVectorInterface& IterativeSolverVisitorWithVectorInterface::operator=( const IterativeSolverVisitorWithVectorInterface& iterativesolvervisitorinterface)
{
  Alat::IterativeSolverVisitorInterface::operator=(iterativesolvervisitorinterface);
  assert(0);
  return *this;
}

std::string IterativeSolverVisitorWithVectorInterface::getName() const
{
  return "IterativeSolverVisitorWithVectorInterface";
}

IterativeSolverVisitorWithVectorInterface* IterativeSolverVisitorWithVectorInterface::clone() const
{
  assert(0);
  // return new IterativeSolverVisitorWithVectorInterface(*this);
}
