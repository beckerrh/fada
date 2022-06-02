#ifndef __Fada_FemVRTM_h
#define __Fada_FemVRTM_h

#include  "fembase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemVRTM : public FEMBase
  {
protected:
    std::string _type;

public:
    ~FemVRTM();
    FemVRTM(std::string type="mean");
    FemVRTM(const FemVRTM& fem);
    FemVRTM& operator=(const FemVRTM& fem);
    std::string getName() const;

    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
