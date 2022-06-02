#include  "Integrators/reactioninterface.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
ReactionInterface::~ReactionInterface(){}
ReactionInterface::ReactionInterface() : Alat::InterfaceBase(){}
ReactionInterface::ReactionInterface( const ReactionInterface& reactioninterface) : Alat::InterfaceBase(reactioninterface)
{
  assert(0);
}
ReactionInterface& ReactionInterface::operator=( const ReactionInterface& reactioninterface)
{
  Alat::InterfaceBase::operator=(reactioninterface);
  assert(0);
  return *this;
}
std::string ReactionInterface::getName() const
{
  return "ReactionInterface";
}
std::string ReactionInterface::getInterfaceName() const
{
  return "ReactionInterface";
}
