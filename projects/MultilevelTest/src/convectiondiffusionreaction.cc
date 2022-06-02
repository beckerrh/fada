#include  "Integrators/identityintegrator.h"
#include  "Integrators/laplacedgintegrator.h"
#include  "Integrators/reactionintegrator.h"
#include  "Integrators/righthandsideboundaryintegrator.h"
#include  "Integrators/righthandsidedomainintegrator.h"
#include  "Integrators/transportdgintegrator.h"
#include  "transportintegrator.h"
#include  "transportintegratorsupg.h"
#include  "convectiondiffusionreaction.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/
ConvectionDiffusionReaction::~ConvectionDiffusionReaction() {}
ConvectionDiffusionReaction::ConvectionDiffusionReaction() : Model() {}
ConvectionDiffusionReaction::ConvectionDiffusionReaction( const ConvectionDiffusionReaction& model) : Model(model)
{
}
ConvectionDiffusionReaction& ConvectionDiffusionReaction::operator=( const ConvectionDiffusionReaction& model)
{
  Model::operator=(model);
  assert(0);
  return *this;
}
Fada::DomainModelInterface* ConvectionDiffusionReaction::clone() const
{
  return new ConvectionDiffusionReaction(*this);
}
std::string ConvectionDiffusionReaction::getName() const
{
  return "ConvectionDiffusionReaction";
}

/*--------------------------------------------------------*/
void ConvectionDiffusionReaction::addStringsAndParameters(const Alat::ParameterFile* parameterfile, const FadaMesh::MeshInterface* mesh)
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
void ConvectionDiffusionReaction::defineParametersForProblemData()
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
  setParameterForProblemDataIfDefined( "U", "RightHandSide", "diffusion", getPhysicalParameter("viscosity") );
  setParameterForProblemDataIfDefined( "U", "Neumann", "diffusion", getPhysicalParameter("viscosity") );
}

/*--------------------------------------------------------*/
void ConvectionDiffusionReaction::defineIntegrators(Fada::IntegratorManager* integratormanager) const
{
  Model::defineIntegrators(integratormanager);
  assert(0);  
}
