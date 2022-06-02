#ifndef __Fada_FemDgQ03d_h
#define __Fada_FemDgQ03d_h

#include  "fembase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class FemDgQ03d : public FEMBase
  {
public:
    std::string getName() const
    {
      return "FemDgQ03d";
    }
    FemDgQ03d* clone() const;
    int getOrder() const;
    void reInitCell(const Alat::Vector<Alat::Node>& nodes);
    void reInitPhysicalPoint(const Alat::Node& x) {}
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    int ndoflocal() const;
    Fada::TransformationInterface* newTransformation() const;
    std::string getReferenceElementType() const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
  };
}

/*---------------------------------------------------------*/

#endif
