#ifndef __Fada_Q22DTransformation_h
#define __Fada_Q22DTransformation_h

#include  "q12dtransformation.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class Q22DTransformation : public Q12DTransformation
  {
protected:
    int getNNodes() const;
    void dtrans(const Alat::Node& xhat);
    void trans(const Alat::Node& xhat);

public:
    ~Q22DTransformation();
    Q22DTransformation();
    Q22DTransformation(const Q22DTransformation& q22dtransformation);
    Q22DTransformation& operator=(const Q22DTransformation& q22dtransformation);
    Q22DTransformation* clone() const;
    std::string getName() const;

    // void reInitCell(int iK);
    void reInitCell(const Alat::Vector<Alat::Node>& nodes);
  };
}

/*--------------------------------------------------------------------------*/

#endif
