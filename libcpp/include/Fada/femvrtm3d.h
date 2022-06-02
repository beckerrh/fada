#ifndef __Fada_FemVRTM3d_h
#define __Fada_FemVRTM3d_h

#include  "femvrtm.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemVRTM3d : public FemVRTM
  {
public:
    ~FemVRTM3d();
    FemVRTM3d(std::string type = "mean");
    FemVRTM3d(const FemVRTM3d& fem);
    FemVRTM3d& operator=(const FemVRTM3d& fem);
    FemVRTM3d* clone() const;
    std::string getName() const;

    std::string getReferenceElementType() const;
    Fada::TransformationInterface* newTransformation() const;
    Fada::TransformationInterface* newTransformationCurved();

    void reInitReferencePoint(const Alat::Node& x);
    int ndoflocal() const
    {
      return 6;
    }

    bool cannotInterpolate() const
    {
      return 1;
    }

    int ndoflocalpatch() const
    {
      return 36;
    }

    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
    void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const;
    void getReferenceCenterPoint(Alat::Node& xhat) const;

  };
}

/*--------------------------------------------------------------------------*/

#endif
