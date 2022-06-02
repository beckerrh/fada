#ifndef __Fada_FemDgQ02d_h
#define __Fada_FemDgQ02d_h

#include  "fembase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class FemDgQ02d : public FEMBase
  {
public:
    std::string getName() const
    {
      return "FemDgQ02d";
    }
    FemDgQ02d* clone() const;
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
