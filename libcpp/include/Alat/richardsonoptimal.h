#ifndef __Alat_RichardsonOptimal_h
#define __Alat_RichardsonOptimal_h

#include  "richardsonrb.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class RichardsonOptimal : public RichardsonRB
  {
private:
    void _computeSmallSystem(int index, int nmemory) const;
    void _addvector(Alat::GhostVector& u, int nmemory) const;
    void _matrixVectorProduct(int index) const;

public:
    ~RichardsonOptimal();
    RichardsonOptimal(const std::string& type, int nvectors, const std::string& solutiontype);
    RichardsonOptimal( const RichardsonOptimal& richardsonoptimal);
    RichardsonOptimal& operator=( const RichardsonOptimal& richardsonoptimal);
    std::string getName() const;
    RichardsonOptimal* clone() const;

    int getNVectors() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
