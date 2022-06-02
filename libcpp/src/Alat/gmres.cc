#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/gmres.h"
#include  <cassert>
#include  <math.h>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/

Gmres::~Gmres()
{}

Gmres::Gmres(const std::string& type, int nvectors, const std::string& solutiontype) : RichardsonOptimal(type, nvectors, solutiontype)
{}

Gmres::Gmres( const Gmres& richardsonoptimal) : RichardsonOptimal(richardsonoptimal)
{
  assert(0);
}

Gmres& Gmres::operator=( const Gmres& richardsonoptimal)
{
  RichardsonOptimal::operator=(richardsonoptimal);
  assert(0);
  return *this;
}

std::string Gmres::getName() const
{
  std::stringstream ss;
  ss<<"Gmres_"<< _type << "_" << _nvectors<<"_"<<_solutiontype;
  return ss.str();
}

Gmres* Gmres::clone() const
{
  assert(0);
//return new Gmres(*this);
}

/*--------------------------------------------------------------------------*/
int Gmres::getNVectors() const
{
  return 2*_nvectors + 2;
}

/*--------------------------------------------------------------------------*/
void Gmres::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{ 
  _nmemory = 0;
  _nextmemory = 0;
  RichardsonOptimal::solve(status, A, u, f);
}
