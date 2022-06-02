#include  "Alat/doublevector.h"
#include  "Fada/dirichletinterface.h"
#include  "Alat/vector.h"
#include  "Alat/node.h"

using namespace Fada;

/*--------------------------------------------------------*/
DirichletInterface::~DirichletInterface() {}
DirichletInterface::DirichletInterface() : Fada::Data() {}
DirichletInterface::DirichletInterface(const DirichletInterface& DI) : Fada::Data()
{
  assert(0);
}
DirichletInterface& DirichletInterface::operator=(const DirichletInterface& DI)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------*/
void DirichletInterface::getGradient(Alat::Vector<Alat::Node>& grad, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  double eps = 1e-4;
  int ncomp = grad.size();
  Alat::DoubleVector gl(ncomp), gr(ncomp);
  Alat::Node vl(x, y, z), vr(x, y, z);
  vr.x() += eps;
  vl.x() -= eps;
  getValue( gr, color, normal, vr.x(), vr.y(), vr.z(), t );
  getValue( gl, color, normal, vl.x(), vl.y(), vl.z(), t );
  for(int icomp = 0; icomp < ncomp; icomp ++)
  {
    grad[icomp].x() = ( gr[icomp] - gl[icomp] )/( 2.*eps );
  }
  vr.x() -= eps;
  vl.x() += eps;
  vr.y() += eps;
  vl.y() -= eps;
  getValue( gr, color, normal, vr.x(), vr.y(), vr.z(), t );
  getValue( gl, color, normal, vl.x(), vl.y(), vl.z(), t );
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    grad[icomp].y() = ( gr[icomp] - gl[icomp] )/( 2.*eps );
  }
  vr.y() -= eps;
  vl.y() += eps;
  vr.z() += eps;
  vl.z() -= eps;
  getValue( gr, color, normal, vr.x(), vr.y(), vr.z(), t );
  getValue( gl, color, normal, vl.x(), vl.y(), vl.z(), t );
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    grad[icomp].z() = ( gr[icomp] - gl[icomp] )/( 2.*eps );
  }
}
