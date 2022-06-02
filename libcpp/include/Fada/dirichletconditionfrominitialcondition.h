#ifndef __Fada_DirichletConditionFromInitialCondition_h
#define __Fada_DirichletConditionFromInitialCondition_h

#include  "dirichletinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class InitialConditionInterface;
  
class DirichletConditionFromInitialCondition : public DirichletInterface
{
private:
  const InitialConditionInterface* _initialconditioninterface;

public:
  ~DirichletConditionFromInitialCondition();
  DirichletConditionFromInitialCondition(const InitialConditionInterface* initialconditioninterface);
  DirichletConditionFromInitialCondition( const DirichletConditionFromInitialCondition& dirichletconditionfrominitialcondition);
  DirichletConditionFromInitialCondition& operator=( const DirichletConditionFromInitialCondition& dirichletconditionfrominitialcondition);
  std::string getName() const;
  DirichletConditionFromInitialCondition* clone() const;
   void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
