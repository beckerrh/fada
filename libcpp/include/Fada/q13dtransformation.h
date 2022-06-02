#ifndef __Fada_Q13DTransformation_h
#define __Fada_Q13DTransformation_h

#include  "Alat/doublematrix.h"
#include  "transformationbase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Q13DTransformation : public TransformationBase
  {
private:
    Alat::Node _norhat;

protected:
    void dtrans(const Alat::Node& xhat);
    void trans(const Alat::Node& xhat);
    int getNNodes() const;
    void setNormalHat( int iiS, Alat::Node& normalhat) const;
    void ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS);
    double getSurfaceJacobianHat(int iiS) const;

public:
    ~Q13DTransformation();
    Q13DTransformation();
    Q13DTransformation(const Q13DTransformation& q13dtransformation);
    Q13DTransformation& operator=(const Q13DTransformation& q13dtransformation);
    Q13DTransformation* clone() const;
    std::string getName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
