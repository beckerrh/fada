#include  "applicationwithexactsolution.h"
#include  "model.h"
#include  "errorestimatorintegrator.h"
#include  "Fada/femtype.h"
#include  "Fada/integratormanager.h"
#include  "Fada/variable.h"
#include  "Fada/variablemanager.h"
#include  "Integrators/righthandsideboundaryintegrator.h"
#include  "Integrators/righthandsidedomainintegrator.h"
#include  "Integrators/trustregion.h"
#include  "Integrators/righthandsideboundaryintegrator.h"
#include  "Integrators/righthandsidedomainintegrator.h"
#include  "Integrators/trustregion.h"
#include  "Integrators/initialintegrator.h"
#include  "Integrators/errorintegrator.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
Model::~Model()
{
}

Model::Model(): Fada::Model(), _dg(false)
{
}

Model::Model( const Model& model): Fada::Model(model)
{
  assert(0);
  _dg = model._dg;
  (*this).operator=(model);
}

Model& Model::operator=( const Model& model)
{
  Fada::Model::operator=(model);
  assert(0);
  _dg = model._dg;
  return *this;
}

std::string Model::getName() const
{
  return "Model";
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
Fada::ApplicationInterface* Model::newApplication(std::string applicationname) const
{
  if(applicationname == "ExactSolution")
  {
    return new ApplicationWithExactSolution;
  }
  else
  {
    _error_string("newApplication", "unknown application \""+applicationname+"\"");
  }
}

/*--------------------------------------------------------*/
int Model::numberOfBoundaryConditionsPerColor() const
{
  return 1;
}

void Model::definePossibleBoundaryConditions(Fada::ModelInterface::BoundaryConditionToVariables& bdryconditions) const
{
  bdryconditions["Dirichlet"].insert("U");
  bdryconditions["Neumann"].insert("U");
}

/*--------------------------------------------------------*/
void Model::addStringsAndParameters(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh)
{
  addString("fem", "none");
  addStringOfLoop("newton", "none");
}

/*--------------------------------------------------------*/
void Model::defineVariables(Fada::VariableManager* variable_manager) const
{
  std::string fem = getStringParameter("fem");
  // std::cerr << "fem="<<fem<<"\n";
  _dg = Fada::femType( fem, fem, getMesh() );
  assert(not _dg);
  std::string visutype = "node";
  if(_dg)
  {
    visutype = "cell";
  }
  variable_manager->addUnknownVariable( new Fada::Variable("U", fem, 1, visutype) );
}

/*--------------------------------------------------------*/
void Model::defineIntegrators(Fada::IntegratorManager* integratormanager) const
{
  if(getLoopType() == FadaEnums::PostProcessLoop)
  {
    std::string applicationname = getApplication()->getName();
    // std::cerr << "@@@@@@@@@@@@@@@@@@@@@@@@ applicationname="<<applicationname<<"\n";
    if(applicationname == "ApplicationWithExactSolution")
    {
      integratormanager->add("Error", new Integrators::ErrorIntegrator, "{Err_U_L2,Err_U_H1,Err_U_C0}", "{U}");
    }
    integratormanager->add("ErrorEstimator", new ErrorEstimatorIntegrator, "{Est_U}", "{U}");
    // integratormanager->add("Estimator2", new EstimatorIntegrator2, "{Est2_U}", "{U}");
  }
  if(getLoopType() == FadaEnums::StaticLoop)
  {
    if( ( getStringParameterOfLoop("newton") == "newtontr" )||( getStringParameterOfLoop("newton") == "newtontr2" )||( getStringParameterOfLoop("newton") == "newton3" ) )
    {
      integratormanager->add("TrustRegion", new Integrators::TrustRegion, "{U}", "{U}");
    }
  }
  if(getLoopType() == FadaEnums::DynamicLoop)
  {
    integratormanager->add( "InitialCondition", new Integrators::InitialIntegrator, "{U}", "{U}");
  }
  integratormanager->add("RightHandSideDomain", new Integrators::RightHandSideDomainIntegrator(), "{U}", "{}");
  Alat::Map<std::string, std::string> bdrycondofvar;
  bdrycondofvar["U"] = "Neumann";
  integratormanager->add("RightHandSideBoundary", new Integrators::RightHandSideBoundaryIntegrator(bdrycondofvar), "{U}", "{}");
}
