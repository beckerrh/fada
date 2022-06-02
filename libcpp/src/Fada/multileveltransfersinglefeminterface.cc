#include  "Fada/multileveltransfersinglefeminterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferSingleFemInterface::~MultiLevelTransferSingleFemInterface() 
{
}

MultiLevelTransferSingleFemInterface::MultiLevelTransferSingleFemInterface(): InterfaceBase()
{
}

MultiLevelTransferSingleFemInterface::MultiLevelTransferSingleFemInterface( const MultiLevelTransferSingleFemInterface& multileveltransfersinglefeminterface): InterfaceBase(multileveltransfersinglefeminterface)
{
(*this).operator=(multileveltransfersinglefeminterface);
}

MultiLevelTransferSingleFemInterface& MultiLevelTransferSingleFemInterface::operator=( const MultiLevelTransferSingleFemInterface& multileveltransfersinglefeminterface) 
{
InterfaceBase::operator=(multileveltransfersinglefeminterface);
assert(0);
return *this;
}

std::string MultiLevelTransferSingleFemInterface::getName() const 
{
return "MultiLevelTransferSingleFemInterface";
}
std::string MultiLevelTransferSingleFemInterface::getInterfaceName() const
{
return "MultiLevelTransferSingleFemInterface";
}

MultiLevelTransferSingleFemInterface* MultiLevelTransferSingleFemInterface::clone() const 
{
  assert(0);
// return new MultiLevelTransferSingleFemInterface(*this);
}
void MultiLevelTransferSingleFemInterface::interpolate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse) const
{
  _notWritten("interpolate");
}

/*--------------------------------------------------------------------------*/
