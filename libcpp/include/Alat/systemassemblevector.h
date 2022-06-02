#ifndef __Alat_SystemAssembleVector_h
#define __Alat_SystemAssembleVector_h

#include  "assemblevector.h"
#include  "map.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemAssembleVector : public Alat::Map<std::string, Alat::AssembleVector>
  {
public:
    ~SystemAssembleVector();
    SystemAssembleVector();
    SystemAssembleVector( const Alat::SystemAssembleVector& systemassemblevector);
    Alat::SystemAssembleVector& operator=( const Alat::SystemAssembleVector& systemassemblevector);
    std::string getName() const;

    void set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& nloc);
    void zeros();
    double operator*(const Alat::SystemAssembleVector& v) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
