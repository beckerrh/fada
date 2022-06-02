#include  "FadaMesh/meshselectorinterface.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

MeshSelectorInterface::~MeshSelectorInterface()
{}

/*--------------------------------------------------------------------------*/

MeshSelectorInterface::MeshSelectorInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

MeshSelectorInterface::MeshSelectorInterface( const MeshSelectorInterface& meshselectorinterface) : Alat::InterfaceBase(meshselectorinterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

MeshSelectorInterface& MeshSelectorInterface::operator=( const MeshSelectorInterface& meshselectorinterface)
{
  InterfaceBase::operator=(meshselectorinterface);
  assert(0);
  return *this;
}
