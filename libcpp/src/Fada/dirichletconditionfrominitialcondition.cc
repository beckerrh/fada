#include  "Fada/dirichletconditionfrominitialcondition.h"
#include  "Fada/initialconditioninterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

DirichletConditionFromInitialCondition::~DirichletConditionFromInitialCondition() 
{
}

DirichletConditionFromInitialCondition::DirichletConditionFromInitialCondition(const InitialConditionInterface* initialconditioninterface): DirichletInterface(), _initialconditioninterface(initialconditioninterface)
{
}

DirichletConditionFromInitialCondition::DirichletConditionFromInitialCondition( const DirichletConditionFromInitialCondition& dirichletconditionfrominitialcondition): DirichletInterface(dirichletconditionfrominitialcondition)
{
assert(0);
}

DirichletConditionFromInitialCondition& DirichletConditionFromInitialCondition::operator=( const DirichletConditionFromInitialCondition& dirichletconditionfrominitialcondition) 
{
DirichletInterface::operator=(dirichletconditionfrominitialcondition);
assert(0);
return *this;
}

std::string DirichletConditionFromInitialCondition::getName() const 
{
return "DirichletConditionFromInitialCondition";
}

DirichletConditionFromInitialCondition* DirichletConditionFromInitialCondition::clone() const 
{
assert(0);
//return new DirichletConditionFromInitialCondition(*this);
}

/*--------------------------------------------------------------------------*/

void DirichletConditionFromInitialCondition::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  _initialconditioninterface->getValue(g, x, y, z); 
}

