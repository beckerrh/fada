#include  "Fada/datafunctionvector.h"
#include  "Alat/node.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

DataFunctionVector::~DataFunctionVector()
{
  // delete ? 
  for(int i = 0; i < _datafunctionvector.size(); i++ )
  {
    delete _datafunctionvector[i];
    _datafunctionvector[i] = NULL;
  }
}

/*--------------------------------------------------------------------------*/

DataFunctionVector::DataFunctionVector() : Data()
{}

/*--------------------------------------------------------------------------*/

DataFunctionVector::DataFunctionVector( int n) : Data()
{
  // set_size(n);
  _datafunctionvector.set_size(n);
  for(int i=0;i<n;i++)
  {
    _datafunctionvector[i] = NULL;    
    // std::cerr << "DataFunctionVector::DataFunctionVector()" << _datafunctionvector[i]->getName() << "\n";
  }
  // std::cerr << "DataFunctionVector::DataFunctionVector()" << getDataFunction(0)->getName() << "\n";
  // std::cerr << "DataFunctionVector::DataFunctionVector()" << _datafunctionvector[0]->getName() << "\n";
}

/*--------------------------------------------------------------------------*/

DataFunctionVector::DataFunctionVector( const DataFunctionVector& datafunctionvector) : Data(datafunctionvector)
{
  _datafunctionvector = datafunctionvector._datafunctionvector;
}

/*--------------------------------------------------------------------------*/

DataFunctionVector& DataFunctionVector::operator=( const DataFunctionVector& datafunctionvector)
{
  Data::operator=(datafunctionvector);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string DataFunctionVector::getName() const
{
  return "DataFunctionVector";
}

/*--------------------------------------------------------------------------*/

DataFunction& DataFunctionVector::operator()(int i)
{
  assert( i < _datafunctionvector.size() );
  return *_datafunctionvector[i];
}

/*--------------------------------------------------------------------------*/

const DataFunction& DataFunctionVector::operator()(int i) const
{
  assert( i < _datafunctionvector.size() );
  return *_datafunctionvector[i];
}

/*--------------------------------------------------------------------------*/

DataFunction*& DataFunctionVector::getDataFunction(int i)
{
  assert( i < _datafunctionvector.size() );
  return _datafunctionvector[i];
}

/*--------------------------------------------------------------------------*/

const DataFunction* DataFunctionVector::getDataFunction(int i) const
{
  assert( i < _datafunctionvector.size() );
  return _datafunctionvector[i];
}

// /*--------------------------------------------------------------------------*/
//
// void DataFunctionVector::set_size(int n)
// {
//   _datafunctionvector.set_size(n);
// }

/*--------------------------------------------------------------------------*/

int DataFunctionVector::size() const
{
  return _datafunctionvector.size();
}

// /*--------------------------------------------------------------------------*/
//
// const Alat::Vector<DataFunction*>& DataFunctionVector::getVector() const
// {
//   return _datafunctionvector;
// }

/*--------------------------------------------------------------------------*/

std::ostream& Fada::operator<<( std::ostream& s, const DataFunctionVector& A )
{
  assert(0);
  return s;
}

/*--------------------------------------------------------------------------*/

void DataFunctionVector::getValue(Alat::Node& betanode, const Alat::Node& v) const
{
  betanode[0] = ( *this )( 0 )( v.x(), v.y(), v.z() );
  betanode[1] = ( *this )( 1 )( v.x(), v.y(), v.z() );
  if(size() == 3)
  {
    betanode[2] = ( *this )( 2 )( v.x(), v.y(), v.z() );
  }
  else
  {
    betanode[2] = 0.0;
  }
}
