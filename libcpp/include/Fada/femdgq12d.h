#ifndef __Fada_FemDgQ12d_h
#define __Fada_FemDgQ12d_h

#include  "femqk2d.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemDgQ12d : public FemQk2d<1>
  {
public:
    ~FemDgQ12d();
    FemDgQ12d();
    FemDgQ12d(const FemDgQ12d& orig);
    FemDgQ12d& operator=(const FemDgQ12d& orig);
    FemDgQ12d* clone() const;
    std::string getName() const;

    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
