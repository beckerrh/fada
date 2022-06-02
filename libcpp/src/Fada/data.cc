#include  "Fada/data.h"

using namespace Fada;

/*---------------------------------------------------------*/

Data::~Data(){}

/*---------------------------------------------------------*/

// Data::Data() : Fada::DataInterface(), _mesh(NULL), _time(0.0) {}
// Data::Data() : Fada::DataInterface(), _mesh(NULL) {}
Data::Data() : Fada::DataInterface() {}

/*---------------------------------------------------------*/

// Data::Data(const Data& B) : Fada::DataInterface(B), _mesh(NULL)
  Data::Data(const Data& B) : Fada::DataInterface(B), _data(B._data), _parameters(B._parameters)
{
  // assert(0);
}

/*---------------------------------------------------------*/

Data& Data::operator=(const Data& B)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

std::string Data::getInterfaceName() const
{
  return "Data";
}
// 
// /*---------------------------------------------------------*/
// 
// double Data::getTime() const
// {
//   return _time;
// }

// /*---------------------------------------------------------*/
// 
// const FadaMesh::MeshInterface* Data::getMesh() const
// {
//   assert(_mesh);
//   return _mesh;
// }

/*---------------------------------------------------------*/

std::string Data::getName() const
{
  return "Data";
}
// 
// /*---------------------------------------------------------*/
// 
// void Data::setTime(double time) const
// {
//   _time = time;
//   for(DataMap::const_iterator p = _data.begin(); p != _data.end(); p++)
//   {
//     p->second->setTime(time);
//   }
// }

/*---------------------------------------------------------*/

const Fada::DataMap* Data::getData() const
{
  return &_data;
}

/*---------------------------------------------------------*/

Fada::DataMap& Data::getData()
{
  return _data;
}

/*---------------------------------------------------------*/

bool Data::hasData(const std::string& name) const
{
  return _data.hasKey(name);
}

/*---------------------------------------------------------*/

const Fada::DataInterface* Data::getData(const std::string& name) const
{
  return _data.getData(name);
}

/*---------------------------------------------------------*/

void Data::setData(const std::string& name, const Fada::DataInterface* data)
{
  _data.setData(name, data);
}

/*---------------------------------------------------------*/

bool Data::hasParameter(const std::string& name) const
{
  return _parameters.hasKey(name);
}

/*---------------------------------------------------------*/

double Data::getParameter(const std::string& name) const
{
  if( not _parameters.hasKey(name) )
  {
    std::cerr << "*** ERROR Fada::Data::getParameter() parameter not found \"" << name << "\"\n";
    assert(0);
    exit(1);
  }
  return _parameters[name];
}

/*---------------------------------------------------------*/

void Data::setParameter(const std::string& name, double parameter)
{
  if( _data.hasKey(name) )
  {
    std::cerr << "*** ERROR Fada::Data::setParameter() parameter not found \"" << name << "\"\n";
    assert(0);
    exit(1);
  }
  _parameters[name] = parameter;
}
