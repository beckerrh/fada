#ifndef __Fada_FemCgQ12d_h
#define __Fada_FemCgQ12d_h

#include  "femqk2d.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemCgQ12d : public FemQk2d<1>
  {
public:
    ~FemCgQ12d();
    FemCgQ12d();
    FemCgQ12d(const FemCgQ12d& orig);
    FemCgQ12d& operator=(const FemCgQ12d& orig);
    FemCgQ12d* clone() const;
    std::string getName() const;

    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
