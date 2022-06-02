#ifndef __NeumannWithExactSolution_h
#define __NeumannWithExactSolution_h

#include  "Fada/datafunctionvector.h"
#include  "Fada/datainterface.h"
#include  "Fada/neumanninterface.h"
#include  "Integrators/diffusionmatrixinterface.h"

/*--------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class NeumannWithExactSolution : public Fada::NeumannInterface
  {
protected:
    const Integrators::DiffusionMatrixInterface* _diffusionclass;
    double _diffusionparameter;
    const Fada::DataFunctionVector& _solution;

public:
    NeumannWithExactSolution(const Fada::DataFunctionVector& solution);
    NeumannWithExactSolution(const NeumannWithExactSolution& neumann);
    void basicInit();
    std::string getName() const
    {
      return "NeumannWithExactSolution";
    }

    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };
// }

#endif
