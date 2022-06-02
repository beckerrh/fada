#ifndef __Fada_FemDgQ22d_h
#define __Fada_FemDgQ22d_h

#include  "femqk2d.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemDgQ22d : public FemQk2d<2>
  {
public:
    ~FemDgQ22d();
    FemDgQ22d();
    FemDgQ22d(const FemDgQ22d& orig);
    FemDgQ22d& operator=(const FemDgQ22d& orig);
    FemDgQ22d* clone() const;
    std::string getName() const;

    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
