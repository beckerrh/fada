#ifndef __Integrators_ReactionInterface_h
#define __Integrators_ReactionInterface_h

#include  "Fada/datainterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DoubleMatrix;
  class DoubleVector;
}

namespace Integrators
{
  // class ReactionInterface : public Alat::InterfaceBase
  class ReactionInterface : public Fada::DataInterface
  {
protected:
  std::string getInterfaceName() const{return "ReactionInterface";}

public:
    ~ReactionInterface() {}
    ReactionInterface(): Fada::DataInterface(){}
    ReactionInterface( const ReactionInterface& reactioninterface): Fada::DataInterface(reactioninterface){}
    ReactionInterface& operator=( const ReactionInterface& reactioninterface){
      Fada::DataInterface::operator=(reactioninterface);assert(0);return *this;
    }

    std::string getName() const {return "ReactionInterface";}

    virtual void reaction(Alat::DoubleVector& omega, const Alat::DoubleVector& u, double x, double y, double z, double t) const = 0;
    virtual void reactionPrime(Alat::DoubleMatrix& omegaprime, const Alat::DoubleVector& u, double x, double y, double z, double t) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
