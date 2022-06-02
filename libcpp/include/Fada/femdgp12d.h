#ifndef __Fada_FemDgP12d_h
#define __Fada_FemDgP12d_h

#include  "fempk2d.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class FemDgP12d : public FemPk2d<1>
  {
public:
    ~FemDgP12d();
    FemDgP12d();
    FemDgP12d(const FemDgP12d& orig);
    FemDgP12d& operator=(const FemDgP12d& orig);
    FemDgP12d* clone() const;
    std::string getName() const;

    // void reInitCell(const Alat::Vector<Alat::Node>& nodes);
    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*---------------------------------------------------------*/

#endif
