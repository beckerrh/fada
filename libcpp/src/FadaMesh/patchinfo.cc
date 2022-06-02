#include  "FadaMesh/patchinfo.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/
PatchInfo::~PatchInfo() 
{
}

PatchInfo::PatchInfo() 
{
}

PatchInfo::PatchInfo( const PatchInfo& patchinfo) 
{
(*this).operator=(patchinfo);
}

PatchInfo& PatchInfo::operator=( const PatchInfo& patchinfo) 
{assert(0);
return *this;
}

std::string PatchInfo::getName() const 
{
return "PatchInfo";
}

PatchInfo* PatchInfo::clone() const 
{
return new PatchInfo(*this);
}

/*--------------------------------------------------------------------------*/
