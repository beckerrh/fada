#include  "Fada/applicationwithexactsolution.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/initialconditionwithexactsolution.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

InitialConditionWithExactSolution::~InitialConditionWithExactSolution()
{}

InitialConditionWithExactSolution::InitialConditionWithExactSolution(std::string varname, const Fada::DataFunctionVector& solution) : InitialConditionInterface(), _varname(varname), _solution(solution)
{}

InitialConditionWithExactSolution::InitialConditionWithExactSolution( const InitialConditionWithExactSolution& initialconditionwithexactsolution) : InitialConditionInterface(initialconditionwithexactsolution), _varname(initialconditionwithexactsolution._varname), _solution(initialconditionwithexactsolution._solution)
{
  ( *this ).operator=(initialconditionwithexactsolution);
}

InitialConditionWithExactSolution& InitialConditionWithExactSolution::operator=( const InitialConditionWithExactSolution& initialconditionwithexactsolution)
{
  InitialConditionInterface::operator=(initialconditionwithexactsolution);
  assert(0);
  return *this;
}

std::string InitialConditionWithExactSolution::getName() const
{
  return "InitialConditionWithExactSolution";
}

InitialConditionWithExactSolution* InitialConditionWithExactSolution::clone() const
{
  return new InitialConditionWithExactSolution(*this);
}

/*--------------------------------------------------------------------------*/

void InitialConditionWithExactSolution::getValue(Alat::DoubleVector& g, double x, double y, double z) const
{
  // const Fada::DataFunctionVector& solution = Fada::ApplicationWithExactSolution::getExactSolution(_varname);
  assert( g.size() == _solution.size() );
  double t = 0.0;
  for(int icomp = 0; icomp < g.size(); icomp++)
  {
    g[icomp] = _solution( icomp) ( x, y, z, t );
  }
}
