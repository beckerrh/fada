#ifndef __RightHandSideWithExactSolution_h
#define __RightHandSideWithExactSolution_h

#include  "Fada/datafunctionvector.h"
#include  "Fada/datainterface.h"
#include  "Fada/righthandsideinterface.h"
#include  "Integrators/diffusionmatrixinterface.h"
#include  "Integrators/reactioninterface.h"

/*--------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class RightHandSideWithExactSolution : public Fada::RightHandSideInterface
  {
protected:
    const Integrators::DiffusionMatrixInterface* _diffusionclass;
    const Integrators::ReactionInterface* _reactionclass;
    const Fada::DataInterface* _betaclass;
    double _diffusionparameter, _reactionparameter;
    const Fada::DataFunctionVector& _solution;
    mutable Alat::DoubleVector _R, _u;
    mutable Alat::DoubleMatrix _K;

public:
    RightHandSideWithExactSolution(const Fada::DataFunctionVector& solution);
    RightHandSideWithExactSolution(const RightHandSideWithExactSolution& righthandside);
    void basicInit();
    std::string getName() const
    {
      return "RightHandSideWithExactSolution";
    }

    void getValue(Alat::DoubleVector& f, double x, double y, double z, double t) const;
  };
// }

#endif
