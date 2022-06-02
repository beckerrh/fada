#include  "Alat/ghostmatrix.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
GhostMatrix::~GhostMatrix()
{}
GhostMatrix::GhostMatrix() : Ghost()
{}
GhostMatrix::GhostMatrix( const Alat::GhostMatrix& ghostmatrix) : Ghost(ghostmatrix), _variablesIn(ghostmatrix._variablesIn)
{}
GhostMatrix::GhostMatrix(const std::string name, int level) : Ghost(name, level) {}
// GhostMatrix::GhostMatrix(const std::string name, const std::string& type) : Ghost(name, type) {}
GhostMatrix& GhostMatrix::operator=( const Alat::GhostMatrix& ghostmatrix)
{
  Ghost::operator=(ghostmatrix);
  _variablesIn=ghostmatrix._variablesIn;
  return *this;
}

/*--------------------------------------------------------------------------*/
// bool GhostMatrix::operator<(const Alat::GhostMatrix& v) const
// {
//   return Ghost::operator<(v);
//   // if( Ghost::operator<(v) )
//   // {
//   //   return true;
//   // }
//   // // if( getVariablesIn() < v.getVariablesIn() )
//   // // {
//   // //   return true;
//   // // }
//   // // if( getVariablesIn() > v.getVariablesIn() )
//   // // {
//   // //   return false;
//   // // }
//   // return false;
// }
// 
// bool GhostMatrix::operator==(const Alat::GhostMatrix& v) const
// {
//   return Ghost::operator==(v);
//   return Ghost::operator==(v)  && ( getVariablesIn() == v.getVariablesIn() );
// }

std::ostream& Alat::operator<<(std::ostream& os, const Alat::GhostMatrix& g)
{
  os << "(Name/description/level/variablesIn/variablesOut:) " << g.getName() <<"/"<< g.getDescription()<<"/"<< g.getLevel()<<"/"<< g.getVariablesIn()<<"/"<<g.getVariablesOut();
  return os;
}

/*--------------------------------------------------------------------------*/
void GhostMatrix::setVariablesIn(const Alat::StringSet& variables)
{
  _variablesIn = variables;
}

const Alat::StringSet& GhostMatrix::getVariablesIn() const
{
  return _variablesIn;
}

void GhostMatrix::setVariablesOut(const Alat::StringSet& variables)
{
  Ghost::setVariables(variables);
}

const Alat::StringSet& GhostMatrix::getVariablesOut() const
{
  return Ghost::getVariables();
}
