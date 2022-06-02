#ifndef __Fada_FemCgQ13d_h
#define __Fada_FemCgQ13d_h

#include  "femqk3d.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemCgQ13d : public FemQk3d<1>
  {
public:
    ~FemCgQ13d();
    FemCgQ13d();
    FemCgQ13d(const FemCgQ13d& orig);
    FemCgQ13d& operator=(const FemCgQ13d& orig);
    FemCgQ13d* clone() const;
    std::string getName() const;

    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;

    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
