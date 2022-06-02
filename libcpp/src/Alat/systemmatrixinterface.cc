#include  "Alat/sparsitypattern.h"
#include  "Alat/systemmatrixinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SystemMatrixInterface::~SystemMatrixInterface(){}
SystemMatrixInterface::SystemMatrixInterface() : MatrixInterface(){}
SystemMatrixInterface::SystemMatrixInterface( const SystemMatrixInterface& systemmatrixinterface) : MatrixInterface(systemmatrixinterface){}

SystemMatrixInterface& SystemMatrixInterface::operator=( const SystemMatrixInterface& systemmatrixinterface)
{
  MatrixInterface::operator=(systemmatrixinterface);
  assert(0);
  return *this;
}

std::string SystemMatrixInterface::getName() const
{
  return "SystemMatrixInterface";
}

std::string SystemMatrixInterface::getInterfaceName() const
{
  return "SystemMatrixInterface";
}

/*--------------------------------------------------------------------------*/
void SystemMatrixInterface::set_size(const Alat::StringSet& variablesOut, const Alat::StringSet& variablesIn)
{
  _notWritten("reInit");
}

/*--------------------------------------------------------------------------*/
const Alat::VariableMatrixInterface* SystemMatrixInterface::getMatrix(std::string varOut, std::string varIn) const
{
  _notWritten("getMatrix");
}

VariableMatrixInterface* SystemMatrixInterface::getMatrix(std::string varOut, std::string varIn)
{
  _notWritten("getMatrix");
}
