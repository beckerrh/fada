#include  "Alat/doublevector.h"
#include  "Fada/applicationwithexactsolution.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/dirichletwithexactsolution.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

DirichletWithExactSolution::~DirichletWithExactSolution()
{}

/*--------------------------------------------------------------------------*/

DirichletWithExactSolution::DirichletWithExactSolution(std::string varname, const Fada::DataFunctionVector& solution) : DirichletInterface(), _varname(varname), _solution(solution)
{
  // std::cerr << "DirichletWithExactSolution() varname="<<varname<<"\n";
  // assert(0);
}

/*--------------------------------------------------------------------------*/

DirichletWithExactSolution::DirichletWithExactSolution( const DirichletWithExactSolution& dirichletwithexactsolution) : DirichletInterface(dirichletwithexactsolution), _varname(dirichletwithexactsolution._varname), _solution(dirichletwithexactsolution._solution)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

DirichletWithExactSolution& DirichletWithExactSolution::operator=( const DirichletWithExactSolution& dirichletwithexactsolution)
{
  DirichletInterface::operator=(dirichletwithexactsolution);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string DirichletWithExactSolution::getName() const
{
  return "DirichletWithExactSolution";
}

/*--------------------------------------------------------*/

void DirichletWithExactSolution::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  // const Fada::DataFunctionVector& solution = Fada::ApplicationWithExactSolution::getExactSolution(_varname);
  assert( g.size() == _solution.size() );
  for(int icomp = 0; icomp < g.size(); icomp++)
  {
    g[icomp] = _solution( icomp) ( x, y, z, t );
  }
  // std::cerr << "DirichletWithExactSolution::getValue() " << x << " "<<  y << " " << z << " ==> " << g << "\n";
}
