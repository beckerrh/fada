#ifndef __Reaction_h
#define __Reaction_h

#include  "Integrators/reactioninterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleMatrix;
  class DoubleVector;
}

  class Reaction : public Integrators::ReactionInterface
  {
protected:

public:
    ~Reaction() {}
    Reaction(): Integrators::ReactionInterface() {}
    Reaction( const ReactionInterface& reaction) : Integrators::ReactionInterface(reaction) {}
    std::string getName() const {return "Reaction";}

    void reaction(Alat::DoubleVector& omega, const Alat::DoubleVector& u, double x, double y, double z, double t) const;
    void reactionPrime(Alat::DoubleMatrix& omegaprime, const Alat::DoubleVector& u, double x, double y, double z, double t) const;
  };

/*--------------------------------------------------------------------------*/

#endif
