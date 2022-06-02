#ifndef __Fada_FemCgP12d_h
#define __Fada_FemCgP12d_h

#include  "fempk2d.h"
#include  "Alat/doublematrix.h"

/*---------------------------------------------------------*/

namespace Fada
{ 
  class FemCgP12d : public FemPk2d<1>
  {
  
  public:
    ~FemCgP12d();
    FemCgP12d();
    FemCgP12d(const FemCgP12d& orig);
    FemCgP12d& operator=(const FemCgP12d& orig);
    FemCgP12d* clone() const;
    std::string getName() const;
    void reInitReferencePoint(const Alat::Node& xhat);
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
    bool interpolationMatrixWrong() const;
    void interpolate(Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const;
  };
}   
/*---------------------------------------------------------*/

#endif
