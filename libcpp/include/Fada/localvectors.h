#ifndef __Fada_LocalVectors_h
#define __Fada_LocalVectors_h

#include  "Alat/map.h"
#include  "Alat/systemassemblevector.h"
#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{  
  class SystemVector;
}
namespace Fada
{    
  class LocalVectors
  {
public:
    typedef Alat::Map<std::string, Alat::SystemAssembleVector> LocalVectorMap;
    typedef Alat::Map<std::string, const Alat::SystemVector*> GlobalVectorMap;

private:
    Alat::SystemAssembleVector _dataloc;
    LocalVectorMap _vectormap;

public:
    ~LocalVectors();
    LocalVectors();
    LocalVectors( const LocalVectors& localvectors);
    LocalVectors& operator=( const LocalVectors& localvectors);
    std::string getName() const;

    const LocalVectorMap& getVectorMap() const;
    Alat::SystemAssembleVector& getVector(std::string name);
    const Alat::SystemAssembleVector& getVector(std::string name) const;
    Alat::SystemAssembleVector& getData();
    const Alat::SystemAssembleVector& getData() const;

    void clear();
    void set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& n, const GlobalVectorMap& vectormap, const Alat::StringIntMap& ncompdata, const Alat::StringIntMap& ndata);
  };
  std::ostream& operator<<(std::ostream& os, const LocalVectors& g);
}

/*--------------------------------------------------------------------------*/

#endif
