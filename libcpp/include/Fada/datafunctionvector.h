#ifndef __Fada_DataFunctionVector_h
#define __Fada_DataFunctionVector_h

#include  "Alat/vector.h"
#include  "data.h"
#include  "datafunction.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DataFunctionVector : public Data
  {
private:
    Alat::Vector<DataFunction*> _datafunctionvector;

public:
    ~DataFunctionVector();
    DataFunctionVector();
    DataFunctionVector(int n);
    DataFunctionVector( const DataFunctionVector& datafunctionvector);
    DataFunctionVector& operator=( const DataFunctionVector& datafunctionvector);
    std::string getName() const;

    DataFunction& operator()(int i);
    const DataFunction& operator()(int i) const;
    DataFunction*& getDataFunction(int i);
    const DataFunction* getDataFunction(int i) const;
    // void set_size(int n);
    int size() const;
    // const Alat::Vector<DataFunction*>& getVector() const;
    void getValue(Alat::Node& betanode, const Alat::Node& v) const;
  };
  std::ostream& operator<<( std::ostream& s, const DataFunctionVector& A );
}

/*--------------------------------------------------------------------------*/

#endif
