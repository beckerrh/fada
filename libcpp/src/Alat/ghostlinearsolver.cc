#include  "Alat/ghostlinearsolver.h"
#include  <cassert>
#include  <iostream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
GhostLinearSolver::~GhostLinearSolver()
{}

// Alat::GhostLinearSolver::GhostLinearSolver() : Ghost()
// {}

GhostLinearSolver::GhostLinearSolver(const std::string name, const Alat::GhostMatrix& matrix, int level) : Ghost(name, level), _matrix(matrix)
{}

GhostLinearSolver::GhostLinearSolver( const Alat::GhostLinearSolver& ghostlinearsolver) : Ghost(ghostlinearsolver), _matrix(ghostlinearsolver._matrix)
{}

GhostLinearSolver& Alat::GhostLinearSolver::operator=( const Alat::GhostLinearSolver& ghostlinearsolver)
{
  Ghost::operator=(ghostlinearsolver);
  assert(0);
  return *this;
}

std::ostream& Alat::operator<<(std::ostream& os, const Alat::GhostLinearSolver& g)
{
  os << "(Name/description/level/matrix/variables:) " << g.getName() <<"/"<< g.getDescription()<<"/"<< g.getLevel() << "/"<< g.getMatrix()<< "/"<< g.getVariables();
  return os;
}

/*--------------------------------------------------------------------------*/
// void Alat::GhostLinearSolver::setMatrix(const Alat::GhostMatrix& matrix)
// {
//   _matrix=matrix;
// }

const Alat::GhostMatrix& Alat::GhostLinearSolver::getMatrix() const
{
  return _matrix;
}

/*--------------------------------------------------------------------------*/
std::ostream& Alat::GhostLinearSolver::write(std::ostream& os) const
{
  os << *this;
  return os;
}
