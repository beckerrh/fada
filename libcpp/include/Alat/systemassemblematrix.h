#ifndef __Alat_SystemAssembleMatrix_h
#define __Alat_SystemAssembleMatrix_h

#include  "assemblematrix.h"
// #include  "intvector.h"
#include  "map.h"
#include  "pair.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemAssembleVector;

  class SystemAssembleMatrix : public Alat::Map<Alat::StringPair, AssembleMatrix>
  {
public:
    ~SystemAssembleMatrix();
    SystemAssembleMatrix();
    SystemAssembleMatrix( const SystemAssembleMatrix& systemassemblematrix);
    SystemAssembleMatrix& operator=( const SystemAssembleMatrix& systemassemblematrix);
    std::string getName() const;

    void set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& mcomp, const Alat::StringIntMap& n, const Alat::StringIntMap& m);
    void set_size(const Alat::StringIntMap& ncomp, const Alat::StringIntMap& nloc);
    void zeros();
    void productVector(SystemAssembleVector& y, const Alat::SystemAssembleVector& x, double d=1.0) const;
    void productVectorTransposed(SystemAssembleVector& y, const Alat::SystemAssembleVector& x, double d=1.0) const;
    bool hasKey(std::string ivar, std::string jvar) const;
    AssembleMatrix& operator()(std::string ivar, std::string jvar);
    const AssembleMatrix& operator()(std::string ivar, std::string jvar) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
