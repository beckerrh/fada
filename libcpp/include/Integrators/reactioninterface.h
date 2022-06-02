#ifndef __Integrators_ReactionInterface_h
#define __Integrators_ReactionInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleMatrix;
  class DoubleVector;
}

namespace Integrators
{
  class ReactionInterface : public Alat::InterfaceBase
  {
protected:
  std::string getInterfaceName() const;

public:
    ~ReactionInterface();
    ReactionInterface();
    ReactionInterface( const ReactionInterface& reactioninterface);
    ReactionInterface& operator=( const ReactionInterface& reactioninterface);
    std::string getName() const;

    virtual void reaction(Alat::DoubleVector& omega, const Alat::DoubleVector& u, double x, double y, double z, double t) const = 0;
    virtual void reactionPrime(Alat::DoubleMatrix& omegaprime, const Alat::DoubleVector& u, double x, double y, double z, double t) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
