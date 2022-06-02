#ifndef __Integrators_ReactionIntegrator_h
#define __Integrators_ReactionIntegrator_h

#include  "Fada/integratorsinglevariable.h"

/*--------------------------------------------------------------------------*/

namespace Integrators
{
  class ReactionInterface;

  class ReactionIntegrator : public Fada::IntegratorSingleVariable
  {
protected:
    const ReactionInterface* _reaction;

    void formCell(AlatEnums::residualstatus& status, int iK, double weight) const;
    void matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const;

public:
    ~ReactionIntegrator();
    ReactionIntegrator();
    ReactionIntegrator( const ReactionIntegrator& reactionintegrator);
    ReactionIntegrator& operator=( const ReactionIntegrator& reactionintegrator);
    std::string getName() const;
    ReactionIntegrator* clone() const;

    void initData();
    void setDomainsOfTerms();

    // void formCell(AlatEnums::residualstatus& status, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors) const;
    // void matrixCell(AlatEnums::residualstatus& status, Alat::SystemAssembleMatrix& Aloc, const Fada::LocalVectors& localvectors) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
