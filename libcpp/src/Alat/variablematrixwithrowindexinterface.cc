#include  "Alat/variablematrixwithrowindexinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableMatrixWithRowIndexInterface::~VariableMatrixWithRowIndexInterface() {}
VariableMatrixWithRowIndexInterface::VariableMatrixWithRowIndexInterface(): Alat::VariableMatrixInterface(){}
VariableMatrixWithRowIndexInterface::VariableMatrixWithRowIndexInterface( const VariableMatrixWithRowIndexInterface& sparsematrixwithrowindexinterface): Alat::VariableMatrixInterface(sparsematrixwithrowindexinterface)
{
assert(0);
}
VariableMatrixWithRowIndexInterface& VariableMatrixWithRowIndexInterface::operator=( const VariableMatrixWithRowIndexInterface& sparsematrixwithrowindexinterface) 
{
VariableMatrixInterface::operator=(sparsematrixwithrowindexinterface);
assert(0);
return *this;
}
std::string VariableMatrixWithRowIndexInterface::getName() const 
{
return "VariableMatrixWithRowIndexInterface";
}
VariableMatrixWithRowIndexInterface* VariableMatrixWithRowIndexInterface::clone() const 
{
assert(0);
//return new VariableMatrixWithRowIndexInterface(*this);
}

/*--------------------------------------------------------------------------*/

