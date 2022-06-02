#include  "Fada/applicationwithexactsolution.h"
#include  "Alat/node.h"
#include  "righthandsidewithexactsolution.h"
#include  "Alat/doublematrix.h"
#include  "Alat/doublevector.h"

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------*/
RightHandSideWithExactSolution::RightHandSideWithExactSolution(const Fada::DataFunctionVector& solution) : Fada::RightHandSideInterface(), _diffusionclass(NULL), _reactionclass(NULL), _betaclass(NULL), _solution(solution){}
RightHandSideWithExactSolution::RightHandSideWithExactSolution(const RightHandSideWithExactSolution& righthandside) : Fada::RightHandSideInterface(righthandside), _diffusionclass(righthandside._diffusionclass), _reactionclass(righthandside._reactionclass), _betaclass(righthandside._betaclass), _solution(righthandside._solution) {}

/*--------------------------------------------------------*/
void RightHandSideWithExactSolution::basicInit()
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
  if( hasData("reaction") )
  {
    _reactionclass = dynamic_cast<const Integrators::ReactionInterface*>( getData("reaction") );
    assert(_reactionclass);
  }
  else
  {
    _reactionclass = NULL;
    _reactionparameter = getParameter("alpha");
  }
  if( hasData("beta") )
  {
    _betaclass = dynamic_cast<const Fada::DataInterface*>( getData("beta") );
  }
  else
  {
    _betaclass = NULL;
  }
  int ncomp = 1;
  _R.set_size(ncomp);
  _u.set_size(ncomp);
  _K.set_size(3,3);
}

/*--------------------------------------------------------*/

void RightHandSideWithExactSolution::getValue(Alat::DoubleVector& f, double x, double y, double z, double t) const
{
  _u[0] = _solution(0) ( x, y, z, t );

  if(_reactionclass)
  {
    _reactionclass->reaction(_R, _u, x, y, z, t);
    f[0] = _R[0];
  }
  else
  {
    f[0] = _reactionparameter*_u[0];
  }

  Alat::Node betanode;
  Alat::Node v;
  v.x() = x;
  v.y() = y;
  v.z() = z;

  if(_betaclass)
  {
    // assert(0);
    _betaclass->getValue(betanode, v);
    // std::cerr << "betanode " << betanode << "\n";
    f[0] += betanode.x()*_solution(0).x(x, y, z, t)+betanode.y()*_solution(0).y(x, y, z, t)+betanode.z()*_solution(0).z(x, y, z, t);
  }

  if(_diffusionclass)
  {
    std::cerr << "*** ERROR RightHandSideWithExactSolution: Do not know how to compute rhs with variable diffusion\n";
    assert(0);
    _diffusionclass->Diffusion(_K, _u, x, y, z, t);
    f[0] -= _K(0, 0)*_solution(0).xx(x, y, z, t);
    f[0] -= _K(0, 1)*_solution(0).xy(x, y, z, t);
    f[0] -= _K(1, 0)*_solution(0).xy(x, y, z, t);
    f[0] -= _K(1, 1)*_solution(0).yy(x, y, z, t);
  }
  else
  {
    f[0] -= _diffusionparameter*( _solution(0).xx(x, y, z, t)+_solution(0).yy(x, y, z, t) +_solution(0).zz(x, y, z, t) );
  }
}
