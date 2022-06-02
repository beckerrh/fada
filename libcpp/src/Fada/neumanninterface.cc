#include  "Fada/neumanninterface.h"

using namespace Fada;

/*--------------------------------------------------------*/
NeumannInterface::~NeumannInterface() {}
NeumannInterface::NeumannInterface() : Fada::Data() {}
NeumannInterface::NeumannInterface(const NeumannInterface& neumanninterface) : Fada::Data()
{
  assert(0);
}
NeumannInterface& NeumannInterface::operator=(const NeumannInterface& neumanninterface)
{
  assert(0);
  return *this;
}
std::string NeumannInterface::getInterfaceName() const
{
  return "NeumannInterface";
}

