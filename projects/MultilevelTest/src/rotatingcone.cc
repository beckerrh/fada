#include  "Fada/domainmodelinterface.h"
#include  "Fada/constantfunction.h"
#include  "Fada/datafunctionvector.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/neumannzero.h"
#include  "Fada/problemdata.h"
#include  "rotatingcone.h"
#include  <cassert>
#include  <cmath>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/

RotatingCone::~RotatingCone()
{}

/*--------------------------------------------------------------------------*/

RotatingCone::RotatingCone() : Fada::Application()
{}

/*--------------------------------------------------------------------------*/

RotatingCone::RotatingCone( const RotatingCone& rotstat) : Fada::Application(rotstat)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

RotatingCone& RotatingCone::operator=( const RotatingCone& rotstat)
{
  Fada::Application::operator=(rotstat);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string RotatingCone::getName() const
{
  return "RotatingCone";
}

/*--------------------------------------------------------------------------*/

RotatingCone* RotatingCone::clone() const
{
  return new RotatingCone(*this);
}

/*--------------------------------------------------------------------------*/

std::string RotatingConeVelocity1::getName() const
{
  return "RotatingConeVelocity1";
}

double RotatingConeVelocity1::operator()(double x, double y, double z, double t) const
{
  return -y;
}

/*--------------------------------------------------------------------------*/

std::string RotatingConeVelocity2::getName() const
{
  return "RotatingConeVelocity2";
}

double RotatingConeVelocity2::operator()(double x, double y, double z, double t) const
{
  return x;
}

/*--------------------------------------------------------------------------*/

void RotatingConeInitial::getValue(Alat::DoubleVector& g, double x, double y, double z) const
{
  double r0 = 0.3;
  double x0 = x - 0.0;
  double y0 = y - 0.5;
  double r = sqrt(x0*x0+y0*y0)/r0;
  if(r >= 1.0)
  {
    g[0] = 0.0;
  }
  else
  {
    g[0] = 1.-r;
  }
  r0 = 0.2;
  x0 = x -0.0;
  y0 = y + 0.5;
  r = fmax( fabs(x0), fabs(y0) )/r0;
  if(r < 1.0)
  {
    g[0] += 1;
  }
  // std::cerr << "RotatingConeInitial::getValue() x="<<x <<" y="<<y<<" r="<<r << " g="<<g << "\n";
}

/*--------------------------------------------------------------------------*/

void RotatingCone::defineProblemData(Fada::ProblemData* problemdata) const
{
  if(getDimension() == 2)
  {
    problemdata->setData("U", "Dirichlet") = new RotatingConeDirichlet2d;
  }
  else
  {
    problemdata->setData("U", "Dirichlet") = new RotatingConeDirichlet3d;
  }
  problemdata->setData("U", "Neumann") = new Fada::NeumannZero;
  problemdata->setData("U", "InitialCondition") = new RotatingConeInitial;
}

/*--------------------------------------------------------------------------*/

Fada::DataInterface* RotatingCone::defineData(std::string name) const
{
  if(name == "beta")
  {
    int dim = getDimension();
    Fada::DataFunctionVector* datafct = new Fada::DataFunctionVector(dim);

    datafct->getDataFunction(0) = new RotatingConeVelocity1;
    datafct->getDataFunction(1) = new RotatingConeVelocity2;
    if(dim == 3)
    {
      assert(0);
    }
    return datafct;
  }
  else
  {
    return NULL;

    assert(0);
  }
}

/*--------------------------------------------------------------------------*/

std::string RotatingConeDirichlet2d::getName() const
{
  return "RotatingConeDirichlet2d";
}

/*--------------------------------------------------------------------------*/

void RotatingConeDirichlet2d::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  g.zeros();
}

/*--------------------------------------------------------------------------*/

std::string RotatingConeDirichlet3d::getName() const
{
  return "RotatingConeDirichlet3d";
}

/*--------------------------------------------------------------------------*/

void RotatingConeDirichlet3d::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  g.zeros();
  if(color == 300)
  {
    if( ( y >= -0.5 ) && ( y <= 0.5 ) && ( z >= -0.5 ) && ( z <= 0.5 ) )
    {
      g[0] = 1.0;
    }
  }
}
