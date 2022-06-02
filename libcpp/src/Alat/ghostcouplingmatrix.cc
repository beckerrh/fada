#include  "Alat/ghostcouplingmatrix.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
GhostCouplingMatrix::~GhostCouplingMatrix(){}
GhostCouplingMatrix::GhostCouplingMatrix() : GhostMatrix(){}
GhostCouplingMatrix::GhostCouplingMatrix( const Alat::GhostCouplingMatrix& ghostmatrix) : GhostMatrix(ghostmatrix), _variablesRightIn(ghostmatrix._variablesRightIn), _variablesRightOut(ghostmatrix._variablesRightOut){}
GhostCouplingMatrix::GhostCouplingMatrix( const Alat::GhostMatrix& ghostmatrix) : GhostMatrix(ghostmatrix) {}
GhostCouplingMatrix::GhostCouplingMatrix(const std::string name, int level) : GhostMatrix(name, level) {}
// GhostCouplingMatrix::GhostCouplingMatrix(const std::string name, const std::string& type) : GhostMatrix(name, type) {}
GhostCouplingMatrix& GhostCouplingMatrix::operator=( const Alat::GhostCouplingMatrix& ghostmatrix)
{
  _variablesRightIn = ghostmatrix._variablesRightIn;
  _variablesRightOut = ghostmatrix._variablesRightOut;
  GhostMatrix::operator=(ghostmatrix);
  return *this;
}

/*--------------------------------------------------------------------------*/
// bool GhostCouplingMatrix::operator<(const Alat::GhostCouplingMatrix& v) const
// {
//   return GhostMatrix::operator<(v);
//   // if( GhostMatrix::operator<(v) )
//   // {
//   //   return true;
//   // }
//   // // if( getVariablesRightIn() < v.getVariablesRightIn() )
//   // // {
//   // //   return true;
//   // }
//   // // if( getVariablesRightOut() > v.getVariablesRightOut() )
//   // // {
//   // //   return false;
//   // // }
//   // return false;
// }
// 
// bool GhostCouplingMatrix::operator==(const Alat::GhostCouplingMatrix& v) const
// {
//   return (GhostMatrix::operator==(v));
//   return ( (GhostMatrix::operator==(v))  && ( getVariablesRightIn() == v.getVariablesRightIn()) && ( getVariablesRightOut() == v.getVariablesRightOut()) );
// }

std::ostream& Alat::operator<<(std::ostream& os, const Alat::GhostCouplingMatrix& g)
{
  os << "(Name/description/level/variablesLeftIn/variablesLeftOut/variablesRightIn/variablesRightOut:) " << g.getName() <<"/"<< g.getDescription()<<"/"<<  g.getLevel()<<"/"<< g.getVariablesLeftIn()<<"/"<<g.getVariablesLeftOut()<<"/"<< g.getVariablesRightIn()<<"/"<<g.getVariablesRightOut();
  return os;
}

/*--------------------------------------------------------------------------*/
void GhostCouplingMatrix::setVariablesLeftIn(const Alat::StringSet& variables)
{
  GhostMatrix::setVariablesIn(variables);
}

const Alat::StringSet& GhostCouplingMatrix::getVariablesLeftIn() const
{
  return GhostMatrix::getVariablesIn();
}

void GhostCouplingMatrix::setVariablesLeftOut(const Alat::StringSet& variables)
{
  GhostMatrix::setVariablesOut(variables);
}

const Alat::StringSet& GhostCouplingMatrix::getVariablesLeftOut() const
{
  return GhostMatrix::getVariablesOut();
}

void GhostCouplingMatrix::setVariablesRightIn(const Alat::StringSet& variables)
{
  _variablesRightIn = variables;
}

const Alat::StringSet& GhostCouplingMatrix::getVariablesRightIn() const
{
  return _variablesRightIn;
}

void GhostCouplingMatrix::setVariablesRightOut(const Alat::StringSet& variables)
{
  _variablesRightOut = variables;
}

const Alat::StringSet& GhostCouplingMatrix::getVariablesRightOut() const
{
  return _variablesRightOut;
}
