#ifndef __Fada_InitialConditionFromDirichletCondition_h
#define __Fada_InitialConditionFromDirichletCondition_h

#include  "initialconditioninterface.h"
#include  "Alat/node.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DirichletInterface;
  
  class InitialConditionFromDirichletCondition : public InitialConditionInterface
  {
protected:
  // factice
  Alat::Node _normal;
  int _color;
  const DirichletInterface* _dirichlet;
  
public:
    ~InitialConditionFromDirichletCondition();
    InitialConditionFromDirichletCondition(const DirichletInterface* dirichlet, int color);
    InitialConditionFromDirichletCondition( const InitialConditionFromDirichletCondition& initialconditionfromdirichletcondition);
    InitialConditionFromDirichletCondition& operator=( const InitialConditionFromDirichletCondition& initialconditionfromdirichletcondition);
    std::string getName() const;
    InitialConditionFromDirichletCondition* clone() const;
    
    void getValue(Alat::DoubleVector& g, double x, double y, double z) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
