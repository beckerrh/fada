#ifndef __Fada_FemQk3d_h
#define __Fada_FemQk3d_h

#include  "fembase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  template<int K>
  class FemQk3d : public FEMBase
  {
public:
    ~FemQk3d();
    FemQk3d();
    FemQk3d(const FemQk3d& orig);
    FemQk3d& operator=(const FemQk3d& orig);
    std::string getName() const;

    Fada::TransformationInterface* newTransformation() const;
    int ndoflocal() const;
    void getReferenceCenterPoint(Alat::Node& xhat) const;
    std::string getReferenceElementType() const;
  };
}

/*---------------------------------------------------------*/

#endif
