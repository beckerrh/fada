#ifndef __Fada_FemFunctions_h
#define __Fada_FemFunctions_h

#include  "Alat/map.h"
#include  "Alat/vector.h"
#include  "femfunction.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{                              
  class AssembleVector;
}
namespace Fada
{                              
  class FemInterface;  
  
  class FemFunctions : public Alat::Map<std::string, Fada::FemFunction>
  {
public:
    ~FemFunctions();
    FemFunctions();
    FemFunctions( const Fada::FemFunctions& femfunctions);
    FemFunctions& operator=( const Fada::FemFunctions& femfunctions);
    std::string getName() const;
    FemFunctions* clone() const;
                       
    void basicInit(const Alat::StringIntMap& ncomps);
    void reInitAndEqual(const Fada::FemFunctions& other);

    const Alat::DoubleVector& operator()(std::string ivar) const;
    
    const double& operator()(std::string ivar, int icomp) const;
    double& operator()(std::string ivar, int icomp);
    const Alat::Node& d(std::string ivar, int icomp) const;
    Alat::Node& d(std::string ivar, int icomp);

    void computeFunctionAndGradient(std::string ivar, const Fada::FemInterface* fem, const Alat::AssembleVector& uloc);
  };
  // std::ostream& operator<<(std::ostream& os, const FemFunctions& g);
}

/*--------------------------------------------------------------------------*/

#endif
