#include  "Fada/localdofinformation.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

LocalDofInformation::~LocalDofInformation()
{}

/*--------------------------------------------------------------------------*/

LocalDofInformation::LocalDofInformation() : Alat::FixArray<4, int>()
{
  Alat::FixArray<4, int>::operator=(0);
}

/*--------------------------------------------------------------------------*/

LocalDofInformation::LocalDofInformation( const LocalDofInformation& localdofinformation) : Alat::FixArray<4, int>(localdofinformation)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

LocalDofInformation& LocalDofInformation::operator=( const LocalDofInformation& localdofinformation)
{
  Alat::FixArray<4, int>::operator=(localdofinformation);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string LocalDofInformation::getName() const
{
  return "LocalDofInformation";
}

/*--------------------------------------------------------------------------*/

LocalDofInformation* LocalDofInformation::clone() const
{
  return new LocalDofInformation(*this);
}

/*--------------------------------------------------------------------------*/

std::ostream& Fada::operator<<(std::ostream& os, const LocalDofInformation& A)
{
  os << "nodes/edges/sides/cells: " << A.nodes() << "/" << A.edges() << "/" << A.sides() << "/" << A.cells();
  return os;
}

/*--------------------------------------------------------------------------*/

int& LocalDofInformation::nodes()
{
  return ( *this )[0];
}

int& LocalDofInformation::edges()
{
  return ( *this )[1];
}

int& LocalDofInformation::sides()
{
  return ( *this )[2];
}

int& LocalDofInformation::cells()
{
  return ( *this )[3];
}

int LocalDofInformation::nodes() const
{
  return ( *this )[0];
}

int LocalDofInformation::edges() const
{
  return ( *this )[1];
}

int LocalDofInformation::sides() const
{
  return ( *this )[2];
}

int LocalDofInformation::cells() const
{
  return ( *this )[3];
}
