#include  "Fada/modelinterface.h"
#include  "Fada/constantfunction.h"
#include  "Fada/datafunctionvector.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/neumannzero.h"
#include  "Fada/problemdata.h"
#include  "Alat/stringvector.h"
#include  "Alat/tokenize.h"
#include  "ramp.h"
#include  <cassert>
// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/
std::string RampDirichlet::getName() const
{
  return "RampDirichlet";
}
/*--------------------------------------------------------------------------*/
void RampDirichlet::getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t) const
{
  g.zeros();
  // if(color == 400)
  {
    // if( ( x >= 1.0 ) && ( y <= 0.75 ) )
    if( x >= 0.0 )
    {
      g[0] = 1.0;
    }
    else
    {
      g[0] = -1.0;
    }
  }
}

/*--------------------------------------------------------------------------*/
Ramp::~Ramp(){}
Ramp::Ramp() : Fada::Application(){}
Ramp::Ramp( const Ramp& rotstat) : Fada::Application(rotstat)
{
  assert(0);
}
Ramp& Ramp::operator=( const Ramp& rotstat)
{
  Fada::Application::operator=(rotstat);
  assert(0);
  return *this;
}
std::string Ramp::getName() const
{
  return "Ramp";
}
Ramp* Ramp::clone() const
{
  return new Ramp(*this);
}
/*--------------------------------------------------------------------------*/
bool Ramp::hasExactSolutions() const
{
  return true;
}

/*--------------------------------------------------------------------------*/
std::string RampExactSolution::getName() const
{
  return "RampExactSolution";
}
double RampExactSolution::operator()(double x, double y, double z, double t) const
{
  double r = 0.6*x-0.1*y -0.1;
  if (r<0.0)
  {
    return -1.0;
  }
  else if (r>0.0)
  {
    return 1.0;
  }
  else
  {
    return 0.0;
  }
}
double RampExactSolution::x(double x, double y, double z, double t) const
{
  return 0.0;
}
double RampExactSolution::y(double x, double y, double z, double t) const
{
  return 0.0;
}
 
/*--------------------------------------------------------------------------*/
void Ramp::defineProblemData(Fada::ProblemData* problemdata) const
{
  Fada::DataFunctionVector* uex = dynamic_cast<Fada::DataFunctionVector*>( problemdata->getData("U_exact", "ExactSolution" ) );
  assert(uex);
  assert(uex->size()==1);

  Alat::Node beta(0.1, 0.6, 0.);
  // Alat::StringVector bouts = Alat::Tokenize(getStringParameter("beta"), "|");
  // assert(bouts.size() == 3);
  // Alat::Node beta ( atof( bouts[0].c_str() ), atof( bouts[1].c_str() ), atof( bouts[2].c_str() ));
  
  // const Fada::DataFunctionVector* betaclass = dynamic_cast<const Fada::DataFunctionVector*>( problemdata->getData("U", "beta") );
  // assert(betaclass);
  // const Fada::ConstantFunction* betac0 = dynamic_cast<const Fada::ConstantFunction*>(betaclass->getDataFunction(0));
  // assert(betac0);
  // const Fada::ConstantFunction* betac1 = dynamic_cast<const Fada::ConstantFunction*>(betaclass->getDataFunction(1));
  // assert(betac1);
  // const Fada::ConstantFunction* betac2 = dynamic_cast<const Fada::ConstantFunction*>(betaclass->getDataFunction(2));
  // assert(betac2);
  // Alat::Node beta ( (*betac0)(0,0,0), (*betac1)(0,0,0),(*betac2)(0,0,0));
  
  uex->getDataFunction(0) = new RampExactSolution(beta);
    
  problemdata->setData("U", "Dirichlet") = new RampDirichlet;
  problemdata->setData("U", "Neumann") = new Fada::NeumannZero;
}

/*--------------------------------------------------------------------------*/
Fada::DataInterface* Ramp::defineData(std::string name) const
{
  if(name == "beta")
  {
    int dim = getDimension();
    Fada::DataFunctionVector* datafct = new Fada::DataFunctionVector(dim);

    Alat::Node betanode(0.1, 0.6, 0.0);
    datafct->getDataFunction(0) = new Fada::ConstantFunction( betanode.x() );
    datafct->getDataFunction(1) = new Fada::ConstantFunction( betanode.y() );
    if(dim == 3)
    {
      datafct->getDataFunction(2) = new Fada::ConstantFunction( betanode.z() );
    }
    return datafct;
  }
  else
  {
    return NULL;

    std::cerr << "Ramp::defineData(): unknown " << name << "\n";
    assert(0);
  }
}
