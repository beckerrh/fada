#include  "Fada/domainmodelinterface.h"
#include  "Fada/constantfunction.h"
#include  "Fada/datafunctionvector.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/neumannzero.h"
#include  "Fada/problemdata.h"
#include  "rotstat.h"
#include  "rotstatbeta.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/

RotStat::~RotStat()
{}

/*--------------------------------------------------------------------------*/

RotStat::RotStat() : Fada::Application()
{}

/*--------------------------------------------------------------------------*/

RotStat::RotStat( const RotStat& rotstat) : Fada::Application(rotstat)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

RotStat& RotStat::operator=( const RotStat& rotstat)
{
  Fada::Application::operator=(rotstat);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string RotStat::getName() const
{
  return "RotStat";
}

/*--------------------------------------------------------------------------*/

RotStat* RotStat::clone() const
{
  return new RotStat(*this);
}

/*--------------------------------------------------------------------------*/

void RotStat::defineProblemData(Fada::ProblemData* problemdata) const
{
  if(getDimension() == 2)
  {
    problemdata->setData("U", "Dirichlet") = new RotStatDirichlet2d;
  }
  else
  {
    problemdata->setData("U", "Dirichlet") = new RotStatDirichlet3d;
  }
  problemdata->setData("U", "Neumann") = new Fada::NeumannZero;
}

/*--------------------------------------------------------------------------*/

Fada::DataInterface* RotStat::defineData(std::string name) const
{
  if(name == "beta")
  {
    int dim = getDimension();
    Fada::DataFunctionVector* datafct = new Fada::DataFunctionVector(dim);

    datafct->getDataFunction(0) = new RotStatBetax;
    datafct->getDataFunction(1) = new RotStatBetay;
    if(dim == 3)
    {
      datafct->getDataFunction(2) = new RotStatBetaz;
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

std::string RotStatDirichlet2d::getName() const
{
  return "RotStatDirichlet2d";
}

/*--------------------------------------------------------------------------*/

void RotStatDirichlet2d::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  g.zeros();
  if(color == 400)
  {
    if( ( y >= -0.5 ) && ( y <= 0.5 ) )
    {
      g[0] = 1.0;
    }
  }
}

/*--------------------------------------------------------------------------*/

std::string RotStatDirichlet3d::getName() const
{
  return "RotStatDirichlet3d";
}

/*--------------------------------------------------------------------------*/

void RotStatDirichlet3d::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
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
