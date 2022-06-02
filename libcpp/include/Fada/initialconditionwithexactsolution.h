#ifndef __Fada_InitialConditionWithExactSolution_h
#define __Fada_InitialConditionWithExactSolution_h

#include  "initialconditioninterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class InitialConditionWithExactSolution : public InitialConditionInterface
  {
protected:
  std::string _varname;
  const Fada::DataFunctionVector& _solution;

public:
    ~InitialConditionWithExactSolution();
    InitialConditionWithExactSolution(std::string varname, const Fada::DataFunctionVector& solution);
    InitialConditionWithExactSolution( const InitialConditionWithExactSolution& initialconditionwithexactsolution);
    InitialConditionWithExactSolution& operator=( const InitialConditionWithExactSolution& initialconditionwithexactsolution);
    std::string getName() const;
    InitialConditionWithExactSolution* clone() const;

    void getValue(Alat::DoubleVector& g, double x, double y, double z) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
