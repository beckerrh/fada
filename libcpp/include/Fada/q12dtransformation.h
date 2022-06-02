#ifndef __Fada_Q12DTransformation_h
#define __Fada_Q12DTransformation_h

#include  "Alat/doublematrix.h"
#include  "transformationbase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Q12DTransformation : public TransformationBase
  {
protected:
    void dtrans(const Alat::Node& xhat);
    void trans(const Alat::Node& xhat);
    int getNNodes() const;
    void correctIntegrationPoint(Alat::Node& shat) const;
    void setNormalHat( int iiS, Alat::Node& normalhat) const;
    void ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS);
    double getSurfaceJacobianHat(int iiS) const;

public:
    ~Q12DTransformation();
    Q12DTransformation();
    Q12DTransformation(const Q12DTransformation& q12dtransformation);
    Q12DTransformation& operator=(const Q12DTransformation& q12dtransformation);
    Q12DTransformation* clone() const;
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
