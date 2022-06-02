#ifndef __Fada_FemDgP02d_h
#define __Fada_FemDgP02d_h

#include  "fembase.h"

/*---------------------------------------------------------*/

namespace Fada
{ 
  class FemDgP02d : public FEMBase
  {

  public:

    std::string getName() const
    {
      return "FemDgP02d";
    }
    FemDgP02d* clone() const;
    int getOrder() const;
    void reInitCell(const Alat::Vector<Alat::Node>& nodes);
    void reInitPhysicalPoint(const Alat::Node& x) {}
    void setDofDescription(Fada::LocalDofInformation& dof_description) const;
    int ndoflocal() const;
    Fada::TransformationInterface* newTransformation() const;
    std::string getReferenceElementType() const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaTransfer() const;
    Fada::IntegrationFormulaInterface* newIntegrationFormulaMassMatrix() const;

  };
}  

/*---------------------------------------------------------*/

#endif
