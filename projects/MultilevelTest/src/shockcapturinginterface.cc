#include  "shockcapturinginterface.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
ShockCapturingInterface::~ShockCapturingInterface() 
{
}

ShockCapturingInterface::ShockCapturingInterface(): Alat::InterfaceBase()
{
}

ShockCapturingInterface::ShockCapturingInterface( const ShockCapturingInterface& slopelimiterinterface): Alat::InterfaceBase(slopelimiterinterface)
{
(*this).operator=(slopelimiterinterface);
}

ShockCapturingInterface& ShockCapturingInterface::operator=( const ShockCapturingInterface& slopelimiterinterface) 
{
Alat::InterfaceBase::operator=(slopelimiterinterface);
assert(0);
return *this;
}

std::string ShockCapturingInterface::getName() const 
{
return "ShockCapturingInterface";
}

std::string ShockCapturingInterface::getInterfaceName() const 
{
return "ShockCapturingInterface";
}

/*--------------------------------------------------------------------------*/
