#ifndef __Fada_FemPk2d_h
#define __Fada_FemPk2d_h

#include  "fembase.h"
#include  "p12dtransformation.h"   

/*---------------------------------------------------------*/

namespace Fada
{
  // K est le degre polynomial
  template<int K>
  class FemPk2d : public FEMBase
  {

public:
    ~FemPk2d();
    FemPk2d();
    FemPk2d(const FemPk2d& orig);
    FemPk2d& operator=(const FemPk2d& orig);
    std::string getName() const;
    Fada::TransformationInterface* newTransformation() const;
    Fada::TransformationInterface* newTransformationCurved();
    int ndoflocal() const;
    void getReferenceCenterPoint(Alat::Node& xhat) const;
    std::string getReferenceElementType() const;


  };
}

/*---------------------------------------------------------*/

#endif
