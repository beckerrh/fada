#include  "Fada/datainterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

DataInterface::~DataInterface()
{}

/*--------------------------------------------------------------------------*/

DataInterface::DataInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

DataInterface::DataInterface( const DataInterface& datainterface) : Alat::InterfaceBase(datainterface)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

DataInterface& DataInterface::operator=( const DataInterface& datainterface)
{
  InterfaceBase::operator=(datainterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

void DataInterface::basicInit()
{
  // assert(0);
}

/*--------------------------------------------------------------------------*/

const Fada::DataInterface* DataInterface::getData(const std::string& name) const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

const Fada::DataMap* DataInterface::getData() const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

double DataInterface::getParameter(const std::string& name) const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

void DataInterface::setParameter(const std::string& name, double parameter)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

void DataInterface::setData(const std::string& name, const Fada::DataInterface* data)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

void DataInterface::getValue(Alat::Node& betanode, const Alat::Node& v) const
{
  assert(0);
}
