#include  "Fada/integratormanager.h"
#include  "Integrators/reactionintegrator.h"
#include  "Integrators/righthandsideboundaryintegrator.h"
#include  "Integrators/righthandsidedomainintegrator.h"
#include  "Integrators/transportdgintegrator.h"
#include  "transportintegrator.h"
#include  "transportintegratorsupg.h"
#include  "convectiondiffusionreaction.h"
#include  "reaction.h"
#include  <cassert>

// using namespace FadaConvectionDiffusionReaction;

/*--------------------------------------------------------------------------*/
ConvectionDiffusionReaction::~ConvectionDiffusionReaction()
{
}

ConvectionDiffusionReaction::ConvectionDiffusionReaction() : Laplace()
{
}

ConvectionDiffusionReaction::ConvectionDiffusionReaction(const ConvectionDiffusionReaction& model) : Laplace(model)
{
}

ConvectionDiffusionReaction& ConvectionDiffusionReaction::operator=(const ConvectionDiffusionReaction& model)
{
   Laplace::operator=(model);
   assert(0);
   return(*this);
}

Fada::ModelInterface * ConvectionDiffusionReaction::clone() const
{
   return(new ConvectionDiffusionReaction(*this));
}

std::string ConvectionDiffusionReaction::getName() const
{
   return("ConvectionDiffusionReaction");
}

/*--------------------------------------------------------*/
void ConvectionDiffusionReaction::addStringsAndParameters(const Alat::ParameterFile *parameterfile, const FadaMesh::MeshInterface *mesh)
{
   Laplace::addStringsAndParameters(parameterfile, mesh);
}

/*--------------------------------------------------------*/
void ConvectionDiffusionReaction::defineParametersForProblemData()
{
   Laplace::defineParametersForProblemData();

   // setData("reaction") = new Reaction;
   // std::string reaction = getStringParameter("reaction");
   // if(reaction == "simple")
   // {
   // }
   // else if(reaction == "nonlinear")
   // {
   //   setData("reaction") = new NonlinearReaction;
   // }
   // else
   // {
   //   _error_string("unknown reaction", reaction);
   // }


   Fada::DataInterface *beta     = getApplication()->defineData("beta");
   Fada::DataInterface *reaction = getApplication()->defineData("reaction");
   setData("beta")     = beta;
   setData("reaction") = reaction;
   if (getProblemData("U", "RightHandSide"))
   {
      setDataForProblemData("U", "RightHandSide", "beta", beta);
      setDataForProblemData("U", "RightHandSide", "reaction", getData("reaction"));
   }
   setDataForProblemData("U", "Neumann", "beta", beta);
}

/*--------------------------------------------------------*/
void ConvectionDiffusionReaction::defineIntegrators(Fada::IntegratorManager *integratormanager) const
{
   Laplace::defineIntegrators(integratormanager);



   integratormanager->add("Reaction", new Integrators::ReactionIntegrator, "{U}", "{U}");
   integratormanager->setDataIfDefined( "reaction", "reaction", getData("reaction") );
   integratormanager->setData("Reaction", "reaction", getData("reaction"));
}
