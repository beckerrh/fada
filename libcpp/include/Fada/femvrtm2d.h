#ifndef __Fada_FemVRTM2d_h
#define __Fada_FemVRTM2d_h

#include  "femvrtm.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemVRTM2d : public FemVRTM
  {
public:
    ~FemVRTM2d();
    FemVRTM2d(std::string type = "mean");
    FemVRTM2d(const FemVRTM2d& fem);
    FemVRTM2d& operator=(const FemVRTM2d& fem);
    FemVRTM2d* clone() const;
    std::string getName() const;

    std::string getReferenceElementType() const;
    Fada::TransformationInterface* newTransformation() const;
    Fada::TransformationInterface* newTransformationCurved();



    void reInitReferencePoint(const Alat::Node& x);
    int ndoflocal() const
    {
      return 4;
    }

    int ndoflocalpatch() const
    {
      return 12;
    }

    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
    void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const;
    void getReferenceCenterPoint(Alat::Node& xhat) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
