#include  "Fada/integratormanager.h"
#include  "Fada/variable.h"
#include  "Fada/variablemanager.h"
#include  "Integrators/identityintegrator.h"
#include  "Integrators/laplacedgintegrator.h"
#include  "Integrators/reactionintegrator.h"
#include  "Integrators/transportdgintegrator.h"
#include  "Fada/unstructuredintegrationloop.h"
#include  "laplace.h"
#include  <cassert>

// using namespace FadaLaplace;

/*--------------------------------------------------------------------------*/
Laplace::~Laplace() {}
Laplace::Laplace() : Model() {}
Laplace::Laplace( const Laplace& model) : Model(model)
{
}
Laplace& Laplace::operator=( const Laplace& model)
{
  Model::operator=(model);
  assert(0);
  return *this;
}
Fada::DomainModelInterface* Laplace::clone() const
{
  return new Laplace(*this);
}
std::string Laplace::getName() const
{
  return "Laplace";
}

/*--------------------------------------------------------*/
Fada::DomainIntegrationLoopInterface* Laplace::newDiscretization() const
{
  return new Fada::UnStructuredIntegrationLoop();
}

/*--------------------------------------------------------*/
void Laplace::addStringsAndParameters(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh)
{
  Model::addStringsAndParameters(parameterfile, mesh);
  addNumericalParameter("boundarypenalty", 10.0);
  addNumericalParameter("interiorpenalty", 2.0);
  addNumericalParameter("interiorderivativepenalty", 0.0);
  addNumericalParameter("nitschesigma", 1.0);
  addPhysicalParameter("viscosity", 1.0);
  addPhysicalParameter("alpha", 0.0);
}

/*--------------------------------------------------------*/
void Laplace::defineParametersForProblemData()
{
  setParameterForProblemDataIfDefined( "U", "RightHandSide", "alpha", getPhysicalParameter("alpha") );
  setParameterForProblemDataIfDefined( "U", "RightHandSide", "diffusion", getPhysicalParameter("viscosity") );
  setParameterForProblemDataIfDefined( "U", "Neumann", "diffusion", getPhysicalParameter("viscosity") );
}

/*--------------------------------------------------------*/
void Laplace::defineIntegrators(Fada::IntegratorManager* integratormanager) const
{
  Model::defineIntegrators(integratormanager);
    if(_dg)
    {
      integratormanager->add("Laplace", new Integrators::LaplaceDgIntegrator, "{U}", "{U}");
    }
    else
    {
      integratormanager->add("Laplace", new Integrators::LaplaceIntegrator, "{U}", "{U}");
    }
  integratormanager->add("Identity", new Integrators::IdentityIntegrator, "{U}", "{U}");
  if( isDataDefined("reaction") )
  {
    integratormanager->setData( "Reaction", "reaction", getData("reaction") );
  }
  integratormanager->setParameterIfDefined( "Identity", "coefficient", getPhysicalParameter("alpha") );
  integratormanager->setParameterIfDefined( "Laplace", "diffusion", getPhysicalParameter("viscosity") );
  integratormanager->setParameterIfDefined( "Laplace", "boundarypenalty", getNumericalParameter("boundarypenalty") );
  integratormanager->setParameterIfDefined( "Laplace", "interiorpenalty", getNumericalParameter("interiorpenalty") );
  integratormanager->setParameterIfDefined( "Laplace", "interiorderivativepenalty", getNumericalParameter("interiorderivativepenalty") );
  integratormanager->setParameterIfDefined( "Laplace", "nitschesigma", getNumericalParameter("nitschesigma") );
}
