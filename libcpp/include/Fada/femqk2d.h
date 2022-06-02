#ifndef __Fada_FemQk2d_h
#define __Fada_FemQk2d_h

#include  "fembase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  template<int K>
  class FemQk2d : public FEMBase
  {
public:
    ~FemQk2d();
    FemQk2d();
    FemQk2d(const FemQk2d& orig);
    FemQk2d& operator=(const FemQk2d& orig);
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
