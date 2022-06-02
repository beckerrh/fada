#include  "convectiondiffusionreaction.h"
#include  "laplace.h"
#include  "transport.h"
#include  "modelmanager.h"
#include  <cassert>

// using namespace FadaMain;

/*--------------------------------------------------------------------------*/
ModelManager::~ModelManager()
{}

ModelManager::ModelManager() : Fada::ModelManager()
{}

ModelManager::ModelManager( const ModelManager& modelmanager) : Fada::ModelManager(modelmanager)
{
  ( *this ).operator=(modelmanager);
}

ModelManager& ModelManager::operator=( const ModelManager& modelmanager)
{
  Fada::ModelManager::operator=(modelmanager);
  assert(0);
  return *this;
}

std::string ModelManager::getName() const
{
  return "ModelManager";
}

ModelManager* ModelManager::clone() const
{
  return new ModelManager(*this);
}

/*--------------------------------------------------------------------------*/
Fada::DomainModelInterface* ModelManager::constructModelByName(const std::string& modelname) const
{
  if(modelname == "ConvectionDiffusionReaction")
  {
    return new ConvectionDiffusionReaction();
  }
  else if(modelname == "Laplace")
  {
    return new Laplace();
  }
  else if(modelname == "Transport")
  {
    return new Transport();
  }
  else
  {
    _error_string("constructModelByName", "unknown model", modelname);
  }
}
