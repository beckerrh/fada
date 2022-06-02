#ifndef __Fada_P12DTransformation_h
#define __Fada_P12DTransformation_h

#include  "Alat/doublematrix.h"
#include  "transformationbase.h"


/*--------------------------------------------------------------------------*/

namespace Fada
{
  class P12DTransformation : public TransformationBase
  {
protected:
    void dtrans(const Alat::Node& xhat);
    void trans(const Alat::Node& xhat);
    int getNNodes() const
    {
      return 3;
    }

    void setNormalHat( int iiS, Alat::Node& normalhat) const;
    void ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS);


public:
    P12DTransformation();
    P12DTransformation(const P12DTransformation& p12dtransformation);
    P12DTransformation& operator=(const P12DTransformation& p12dtransformation);
    P12DTransformation* clone() const;
    std::string getName() const
    {
      return "P12DTranformation";
    }
    double getSurfaceJacobianHat(int iiS) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
