#include  "Fada/applicationwithexactsolution.h"
#include  "Alat/node.h"
#include  "neumannwithexactsolution.h"
#include  "Alat/doublematrix.h"

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------*/

NeumannWithExactSolution::NeumannWithExactSolution(const Fada::DataFunctionVector& solution) : Fada::NeumannInterface(), _diffusionclass(NULL), _solution(solution) {}
NeumannWithExactSolution::NeumannWithExactSolution(const NeumannWithExactSolution& neumann) :Fada::NeumannInterface(neumann), _diffusionclass(neumann._diffusionclass), _solution(neumann._solution) {} 

/*--------------------------------------------------------*/

void NeumannWithExactSolution::basicInit()
{
  if( hasData("diffusion") )
  {
    _diffusionclass = dynamic_cast<const Integrators::DiffusionMatrixInterface*>( getData("diffusion") );
    assert(_diffusionclass);
  }
  else
  {
    _diffusionclass = NULL;
    _diffusionparameter = getParameter("diffusion");
  }
}

/*--------------------------------------------------------*/

void NeumannWithExactSolution::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t ) const
{
  int ncomp = 1;
  Alat::DoubleMatrix K(3,3);

  // const Fada::DataFunctionVector& solution = Fada::ApplicationWithExactSolution::getExactSolution("U_exact");

  if(_diffusionclass)
  {
    Alat::DoubleVector u(ncomp);
    u[0] = _solution(0) ( x, y, z, t );
    _diffusionclass->Diffusion(K, u, x, y, z, t);
    g[0] = normal.x()*( K(0, 0)*_solution(0).x(x, y, z, t)+K(0, 1)*_solution(0).y(x, y, z, t) );
    g[0] += normal.y()*( K(1, 0)*_solution(0).x(x, y, z, t)+K(1, 1)*_solution(0).y(x, y, z, t) );
// take into account 3D
    g[0] += normal.x()* K(0, 2)*_solution(0).z(x, y, z, t)+normal.y()*K(1, 2)*_solution(0).z(x, y, z, t);
    g[0] += normal.z()* ( K(2, 0)*_solution(0).x(x, y, z, t)+K(2, 1)*_solution(0).y(x, y, z, t)+ K(2, 2)*_solution(0).z(x, y, z, t) );
  }
  else
  {
    g[0] = _diffusionparameter*( normal.x()*_solution(0).x(x, y, z, t)+normal.y()*_solution(0).y(x, y, z, t) );
// take into account 3D
    g[0] += _diffusionparameter*normal.z()*_solution(0).z(x, y, z, t);
  }
}
