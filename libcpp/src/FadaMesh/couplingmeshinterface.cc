#include  "FadaMesh/couplingmeshinterface.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

CouplingMeshInterface::~CouplingMeshInterface()
{}

/*--------------------------------------------------------------------------*/

CouplingMeshInterface::CouplingMeshInterface() : MeshInterface()
{}

/*--------------------------------------------------------------------------*/

CouplingMeshInterface::CouplingMeshInterface( const CouplingMeshInterface& couplingmeshinterface) : MeshInterface(couplingmeshinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

CouplingMeshInterface& CouplingMeshInterface::operator=( const CouplingMeshInterface& couplingmeshinterface)
{
  MeshInterface::operator=(couplingmeshinterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string CouplingMeshInterface::getName() const
{
  return "CouplingMeshInterface";
}

/*--------------------------------------------------------------------------*/

CouplingMeshInterface* CouplingMeshInterface::clone() const
{
  assert(0);
  // return new CouplingMeshInterface(*this);
}
