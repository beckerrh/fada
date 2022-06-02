#include  "slopelimiterinterface.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
SlopeLimiterInterface::~SlopeLimiterInterface() 
{
}

SlopeLimiterInterface::SlopeLimiterInterface(): Alat::InterfaceBase()
{
}

SlopeLimiterInterface::SlopeLimiterInterface( const SlopeLimiterInterface& slopelimiterinterface): Alat::InterfaceBase(slopelimiterinterface)
{
(*this).operator=(slopelimiterinterface);
}

SlopeLimiterInterface& SlopeLimiterInterface::operator=( const SlopeLimiterInterface& slopelimiterinterface) 
{
Alat::InterfaceBase::operator=(slopelimiterinterface);
assert(0);
return *this;
}

std::string SlopeLimiterInterface::getName() const 
{
return "SlopeLimiterInterface";
}

std::string SlopeLimiterInterface::getInterfaceName() const 
{
return "SlopeLimiterInterface";
}

/*--------------------------------------------------------------------------*/
