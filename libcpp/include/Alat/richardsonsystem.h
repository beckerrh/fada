#ifndef __Alat_RichardsonSystem_h
#define __Alat_RichardsonSystem_h

#include  "richardsonrb.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class RichardsonSystem : public RichardsonRB
  {
private:
    mutable Alat::DoubleVector _scalarproduct;
    mutable Alat::Vector<Alat::GhostVector*> _vectors;

    void _computeSmallSystem(int index, int nmemory) const;
    void _addvector(Alat::GhostVector& u, int nmemory) const;
    void _matrixVectorProduct(int index) const;
    
public:
    ~RichardsonSystem();
    RichardsonSystem(const std::string& type, int nvectors, const std::string& solutiontype);
    RichardsonSystem( const RichardsonSystem& richardsonoptimal);
    RichardsonSystem& operator=( const RichardsonSystem& richardsonoptimal);
    std::string getName() const;
    RichardsonSystem* clone() const;

    void basicInit(const Alat::ParameterFile* parameterfile, std::string blockname);
    int getNVectors() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
