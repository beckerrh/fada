#include  "Fada/integratormanager.h"
#include  "Fada/variable.h"
#include  "Fada/variablemanager.h"
#include  "Integrators/identityintegrator.h"
#include  "Integrators/reactionintegrator.h"
#include  "Integrators/transportdgintegrator.h"
#include  "Integrators/errorintegrator.h"
#include  "transportintegrator.h"
#include  "transportintegratorsupg.h"
#include  "applicationwithexactsolution.h"
#include  "ramp.h"
#include  "rotatingcone.h"
#include  "rotstat.h"
#include  "transport.h"
#include  "discretizationsupg.h"
#include  <cassert>

// using namespace FadaTransport;

/*--------------------------------------------------------------------------*/
Transport::~Transport() {}
Transport::Transport() : Model() {}
Transport::Transport( const Transport& model) : Model(model)
{
}
Transport& Transport::operator=( const Transport& model)
{
  Model::operator=(model);
  assert(0);
  return *this;
}
Fada::DomainModelInterface* Transport::clone() const
{
  return new Transport(*this);
}
std::string Transport::getName() const
{
  return "Transport";
}
/*--------------------------------------------------------------------------*/
Fada::ApplicationInterface* Transport::newApplication(std::string applicationname) const
{
  if(applicationname == "ExactSolution")
  {
    return new ApplicationWithExactSolution;
  }
  else if(applicationname == "RotStat")
  {
    return new RotStat;
  }
  else if(applicationname == "Ramp")
  {
    return new Ramp;
  }
  else if(applicationname == "RotatingCone")
  {
    return new RotatingCone;
  }
  else
  {
    return Model::newApplication(applicationname);
  }
}

/*--------------------------------------------------------*/
void Transport::addStringsAndParameters(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh)
{
  Model::addStringsAndParameters(parameterfile, mesh);
  addPhysicalParameter("alpha", 0.0);
  addString("discretization");
}

/*--------------------------------------------------------*/
void Transport::defineParametersForProblemData()
{
  Fada::DataInterface* beta = getApplication()->defineData("beta");
  Fada::DataInterface* reaction = getApplication()->defineData("reaction");
  if(beta)
  {
    setData("beta") =  beta;
  }
  if( isDataDefined("beta") )
  {
    if( getProblemData("U", "RightHandSide") )
    {
      setDataForProblemData( "U", "RightHandSide", "beta", getData("beta") );
    }
    setDataForProblemData( "U", "Neumann", "beta", getData("beta") );
  }

  setParameterForProblemDataIfDefined( "U", "RightHandSide", "alpha", getPhysicalParameter("alpha") );
  setParameterForProblemDataIfDefined( "U", "RightHandSide", "diffusion", 0.0);
  setParameterForProblemDataIfDefined( "U", "Neumann", "diffusion", 0.0 );
}

/*--------------------------------------------------------*/
Fada::DomainIntegrationLoopInterface* Transport::newDiscretization() const
{
  std::string discretization = getStringParameter("discretization");
  if(discretization=="supg" or discretization=="supgsl" or discretization=="supgsc" or discretization=="supgslsc")
  {
    return new DiscretizationSupg(discretization);
  }
  else
  {
    _error_string("basicInit", "unknown discretization", discretization);
  }
}

/*--------------------------------------------------------*/
void Transport::defineIntegrators(Fada::IntegratorManager* integratormanager) const
{
  Model::defineIntegrators(integratormanager);
  std::string applicationname = getApplication()->getName();
  if(applicationname == "Ramp")
  {
    integratormanager->add("Error", new Integrators::ErrorIntegrator, "{Err_U_L1,Err_U_L2,Err_U_C0,Err_U_W1,Err_U_OS,Err_U_US}", "{U}");
  }
  integratormanager->add("Identity", new Integrators::IdentityIntegrator, "{U}", "{U}");
  integratormanager->setDataIfDefined( "DomainSorterIntegrator", "beta", getData("beta") );
  integratormanager->setParameterIfDefined( "Identity", "coefficient", getPhysicalParameter("alpha") );
}
