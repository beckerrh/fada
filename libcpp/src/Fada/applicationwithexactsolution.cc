#include  "Fada/applicationwithexactsolution.h"
#include  "Alat/filescanner.h"
#include  "Fada/modelinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
ApplicationWithExactSolution::~ApplicationWithExactSolution() {}
ApplicationWithExactSolution::ApplicationWithExactSolution() : Application()
{}
ApplicationWithExactSolution::ApplicationWithExactSolution( const ApplicationWithExactSolution& applicationwithexactsolution) : Application(applicationwithexactsolution)
{
  *this = applicationwithexactsolution;
}

ApplicationWithExactSolution& ApplicationWithExactSolution::operator=( const ApplicationWithExactSolution& applicationwithexactsolution)
{
  Application::operator=(applicationwithexactsolution);
  return *this;
}

std::string ApplicationWithExactSolution::getName() const
{
  return "ApplicationWithExactSolution";
}

/*--------------------------------------------------------------------------*/
void ApplicationWithExactSolution::basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model, const Alat::StringSet& possibleboundaryconditions)
{
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("exactsolution", &_exactsolutionname, "none");
  Alat::FileScanner FS(dataformathandler, parameterfile, "Application_"+model->getName(), 0);
  Application::basicInit(parameterfile, model, possibleboundaryconditions);
}

/*--------------------------------------------------------------------------*/
const std::string& ApplicationWithExactSolution::getExactSolutionName() const
{
  return _exactsolutionname;
}

bool ApplicationWithExactSolution::hasExactSolutions() const
{
  return true;
}
