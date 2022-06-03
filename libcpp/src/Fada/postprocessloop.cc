#include  "Fada/postprocessloop.h"
#include  "Fada/solvermanager.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
PostProcessLoop::~PostProcessLoop()
{}

PostProcessLoop::PostProcessLoop() : Loop()//, _postprocess("postprocess", "postprocess")
{}

PostProcessLoop::PostProcessLoop( const PostProcessLoop& postprocessloop) : Loop(postprocessloop)
{
  assert(0);
}

PostProcessLoop& PostProcessLoop::operator=( const PostProcessLoop& postprocessloop)
{
  Loop::operator=(postprocessloop);
  assert(0);
  return *this;
}

std::string PostProcessLoop::getName() const
{
  return "PostProcessLoop";
}

FadaEnums::looptype PostProcessLoop::getType() const
{
  return FadaEnums::PostProcessLoop;
}

PostProcessLoop* PostProcessLoop::clone() const
{
  return new PostProcessLoop(*this);
}

/*--------------------------------------------------------------------------*/
void PostProcessLoop::basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  // solvermanager->registerVector(_postprocess);
  Loop::basicInit(model, solvermanager, rundirectory, parameterfile);
  solvermanager->reInit();
  getIoManager().setReadUnknownVariables(true);
  // printLoopInformation(std::cout);
}

/*--------------------------------------------------------------------------*/
void PostProcessLoop::run()
{
  AlatEnums::residualstatus status;
  getSolverManager()->readUnknownVariables(_u);
  getSolverManager()->postProcess(status, _postprocess, _u, _u);
  getSolverManager()->writePostProcessVariables(_postprocess);
}
