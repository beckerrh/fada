#ifndef __Alat_PreconditionerWithSorting_h
#define __Alat_PreconditionerWithSorting_h

#include  "preconditionerwithvariablematrix.h"
#include  "intvector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleVector;
}

namespace Alat
{
  class SorterInterface;
  class VariableVector;
  class SystemVector;

  class PreconditionerWithSorting : public Alat::PreconditionerWithVariableMatrix
  {
private:
    Alat::SorterInterface* _sorter;

protected:
    Alat::IntVector _p, _pinv;
    void sortIndices();

    const Alat::SorterInterface* getSorter() const;

public:
    ~PreconditionerWithSorting();
    PreconditionerWithSorting(const Alat::VariableMatrixInterface* matrix);
    PreconditionerWithSorting( const PreconditionerWithSorting& preconditionerwithsorting);
    PreconditionerWithSorting& operator=( const PreconditionerWithSorting& preconditionerwithsorting);
    std::string getName() const;
    PreconditionerWithSorting* clone() const;

    const Alat::IntVector& getP() const;
    const Alat::IntVector& getPinv() const;
    Alat::IntVector& getPinv();
    Alat::SorterInterface*& newSorterPointer();
    void reInit();
    void permute_from(int icomp, Alat::DoubleVector& x, const Alat::VariableVector& y) const;
    void permute_to(int icomp, Alat::VariableVector& y, const Alat::DoubleVector& x) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
