#include  "Fada/initialconditionfromdirichletcondition.h"
#include  "Fada/dirichletinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

InitialConditionFromDirichletCondition::~InitialConditionFromDirichletCondition()
{}

InitialConditionFromDirichletCondition::InitialConditionFromDirichletCondition(const DirichletInterface* dirichlet, int color) : InitialConditionInterface(), _dirichlet(dirichlet), _color(color)
{
  _normal.x() = 1.0;
  _normal.y() = 0.0;
  _normal.z() = 0.0;
}

InitialConditionFromDirichletCondition::InitialConditionFromDirichletCondition( const InitialConditionFromDirichletCondition& initialconditionfromdirichletcondition) : InitialConditionInterface(initialconditionfromdirichletcondition)
{
  assert(0);
}

InitialConditionFromDirichletCondition& InitialConditionFromDirichletCondition::operator=( const InitialConditionFromDirichletCondition& initialconditionfromdirichletcondition)
{
  InitialConditionInterface::operator=(initialconditionfromdirichletcondition);
  assert(0);
  return *this;
}

std::string InitialConditionFromDirichletCondition::getName() const
{
  return "InitialConditionFromDirichletCondition";
}

InitialConditionFromDirichletCondition* InitialConditionFromDirichletCondition::clone() const
{
  assert(0);
//return new InitialConditionFromDirichletCondition(*this);
}

/*--------------------------------------------------------------------------*/
void InitialConditionFromDirichletCondition::getValue(Alat::DoubleVector& g, double x, double y, double z) const
{
  _dirichlet->getValue(g, _color, _normal, x, y, z, 0.0);
}
