#include  "Fada/constantfunction.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/dirichletwithexactsolution.h"
#include  "Fada/modelinterface.h"
#include  "Fada/integratormanager.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/problemdata.h"
#include  "Fada/simpleexactsolutions.h"
// #include  "Integrators/errorintegrator.h"
#include  "applicationwithexactsolution.h"
#include  "neumannwithexactsolution.h"
#include  "pwlinearsolution.h"
#include  "righthandsidewithexactsolution.h"
#include  "Alat/tokenize.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/

ApplicationWithExactSolution::~ApplicationWithExactSolution()
{}

/*--------------------------------------------------------------------------*/

ApplicationWithExactSolution::ApplicationWithExactSolution() : Fada::ApplicationWithExactSolution()
{
  addStringParameter("beta");
}

/*--------------------------------------------------------------------------*/

ApplicationWithExactSolution::ApplicationWithExactSolution( const ApplicationWithExactSolution& applicationwithexactsolution) : Fada::ApplicationWithExactSolution(applicationwithexactsolution)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

ApplicationWithExactSolution& ApplicationWithExactSolution::operator=( const ApplicationWithExactSolution& applicationwithexactsolution)
{
  Fada::ApplicationWithExactSolution::operator=(applicationwithexactsolution);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

Fada::ApplicationInterface* ApplicationWithExactSolution::clone() const
{
  return new ApplicationWithExactSolution(*this);
}

/*--------------------------------------------------------------------------*/

std::string ApplicationWithExactSolution::getName() const
{
  return "ApplicationWithExactSolution";
}
//
// /*--------------------------------------------------------------------------*/
//
// void ApplicationWithExactSolution::defineIntegrators(Fada::IntegratorManager* integratormanager) const
// {
//   // integratormanager->add("Error", new Integrators::ErrorIntegrator, "{Err_U_L2,Err_U_H1,Err_U_C0}", "{U}");
// }

/*--------------------------------------------------------------------------*/

void ApplicationWithExactSolution::constructExactSolution(Fada::DataFunctionVector& uex) const
{
  if(getExactSolutionName() == "constant")
  {
    uex.getDataFunction(0) = new Fada::ConstantSolution(7.0);
  }
  else if(getExactSolutionName() == "linear")
  {
    // double a = 1.1, b = 2.2, c = 3.3, d = 4.4;
    // double a = 1.0, b = 0.0, c = 3.0, d = 4.4;
    // double a = 1.0, b = 2.2, c = 0.0, d = 0.0;
    double a = 0.0, b = 0.3, c = 0.7, d = 0.0;
    if(getDimension() == 2)
    {
      d = 0.0;
    }
    uex.getDataFunction(0) = new Fada::LinearSolution(a, b, c, d);
  }
  else if(getExactSolutionName() == "pwlinear")
  {
    uex.getDataFunction(0) = new PwLinearSolution();
  }
  else if(getExactSolutionName() == "quadratic")
  {
    double a = 1.0, b = 1.0, c = 1.0, d = 0.3, e = -0.4, f = 1.0, g = 0.9, h = 0.7, i = 0.6;
    if(getDimension() == 2)
    {
      g = h = i = 0.0;
    }
    uex.getDataFunction(0) = new Fada::QuadraticSolution(a, b, c, d, e, f, g, h, i);
  }
  else if(getExactSolutionName() == "cubic")
  {
    double a = 1.0, b = 1.0, c = 1.0, d = 1.0, e = 1.0, f = 1.0, g = 1.0, h = 1.0, i = 1.0, j = 1.0;
    double k = 1.0, l = 1.0, m = 1.0, n = 1.0, o = 1.0, p = 1.0, r = 1.0, s = 1.0, t = 1.0, u = 1.0;

    if(getDimension() == 2)
    {
      k = l = m = n = o = p = r = s = t = u = 0.0;
    }
    uex.getDataFunction(0) = new Fada::CubicSolution(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, r, s, t, u);
  }
  else if(getExactSolutionName() == "trigonometric")
  {
    if(getDimension() == 2)
    {
      // uex.getDataFunction(0) = new Fada::CosinusSinusSolution(1.0, 1.0, 1.0);
      // uex.getDataFunction(0) = new Fada::SinusCosinusSolution(1.0, 1.0, 1.0);
      //
      // même que la pression dans IncompressibleFlow
      uex.getDataFunction(0) = new Fada::CosinusPlusCosinusSolution(-0.25, 0.5, 0.25);
    }
    else
    {
      assert(0);
      // uex.getDataFunction(0) = new Fada::CosinusSolution3D(0.5, 0.5, 0.0);
    }
  }
  else if(getExactSolutionName() == "exponential")
  {
    double raideur = 0.5;
    if(getDimension() == 2)
    {
      uex.getDataFunction(0) = new Fada::ExponentialSolution( 0.5,  0.5,  0.5,  raideur);
    }
    else
    {
      uex.getDataFunction(0) = new Fada::ExponentialSolution3D( 0.5,  0.5,  0.5,  raideur);
    }
  }
  else if(getExactSolutionName() == "ldomain")
  {
    if(getDimension() == 2)
    {
      uex.getDataFunction(0) = new Fada::LDomainSolution;
    }
    else
    {
      std::cerr<<"*** ERROR ApplicationWithExactSolution::setExactSolution() : exact solution "<<getExactSolutionName()<<" not written in 3D"<<"\"\n";
      assert(0);
    }
  }
  else if(getExactSolutionName() == "slitdomain")
  {
    if(getDimension() == 2)
    {
      uex.getDataFunction(0) = new Fada::SlitDomainSolution;
    }
    else
    {
      std::cerr<<"*** ERROR ApplicationWithExactSolution::setExactSolution() : exact solution "<<getExactSolutionName()<<" not written in 3D" <<"\"\n";
      assert(0);
    }
  }
  else
  {
    std::cerr<<"*** ERROR ApplicationWithExactSolution::setExactSolution() : unknown exact solution \""<<getExactSolutionName()<<"\"\n";
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/

void ApplicationWithExactSolution::defineProblemData(Fada::ProblemData* problemdata) const
{
  Fada::DataFunctionVector* uex = dynamic_cast<Fada::DataFunctionVector*>( problemdata->getData("U_exact", "ExactSolution" ) );
  assert(uex);
  constructExactSolution(*uex);

  problemdata->setData("U", "Dirichlet") = new Fada::DirichletWithExactSolution("U",*uex);
  problemdata->setData("U", "RightHandSide") = new RightHandSideWithExactSolution(*uex);
  problemdata->setData("U", "Neumann") = new NeumannWithExactSolution(*uex);
}

/*--------------------------------------------------------------------------*/

Fada::DataInterface* ApplicationWithExactSolution::defineData(std::string name) const
{
  if(name == "beta")
  {
    int dim = getDimension();
    Fada::DataFunctionVector* datafct = new Fada::DataFunctionVector(dim);

    Alat::StringVector bouts = Alat::Tokenize(getStringParameter("beta"), "|");
    assert(bouts.size() == 3);
    datafct->getDataFunction(0) = new Fada::ConstantFunction( atof( bouts[0].c_str() ) );
    datafct->getDataFunction(1) = new Fada::ConstantFunction( atof( bouts[1].c_str() ) );
    if(dim == 3)
    {
      datafct->getDataFunction(2) = new Fada::ConstantFunction( atof( bouts[2].c_str() ) );
    }
    return datafct;
  }
  else
  {
    return NULL;

    assert(0);
  }
}
