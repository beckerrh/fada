#ifndef __Fada_FemQ22d_h
#define __Fada_FemQ22d_h

#include  "femqk2d.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class FemCgQ22d : public FemQk2d<2>
  {
public:
    ~FemCgQ22d();
    FemCgQ22d();
    FemCgQ22d(const FemCgQ22d& orig);
    FemCgQ22d& operator=(const FemCgQ22d& orig);
    FemCgQ22d* clone() const;
    std::string getName() const;

    // int ndoflocalpatch() const {return 25;}
    int getOrder() const;
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    void reInitReferencePoint(const Alat::Node& xhat);
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
    // void getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const;
    // void computeHatSecondDerivatives(Alat::Vector<Alat::FixArray<6, double> >& d2phihat, const Alat::Node& xhat);
  };
}

/*--------------------------------------------------------------------------*/

#endif
