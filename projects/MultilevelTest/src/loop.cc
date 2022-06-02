#include  "loop.h"
#include  "solvermanager.h"
#include  "Fada/solvermanagerinterface.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  <cassert>
#include  <fstream>

/*--------------------------------------------------------------------------*/
Loop::~Loop()
{
}

Loop::Loop(std::string todo): Fada::Loop(), _todo(todo), _help("help", "unknowns")
{
}

Loop::Loop( const Loop& loop): Fada::Loop(loop)
{
(*this).operator=(loop);
}

Loop& Loop::operator=( const Loop& loop)
{
Fada::Loop::operator=(loop);
assert(0);
return *this;
}

std::string Loop::getName() const
{
return "Loop";
}

Loop* Loop::clone() const
{
return new Loop(*this);
}
FadaEnums::looptype Loop::getType() const
{
  return FadaEnums::OwnLoop;
}

/*---------------------------------------------------------*/
void Loop::basicInit(Fada::ModelManagerInterface* modelmanager, Fada::SolverManagerInterface* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  Fada::Loop::basicInit(modelmanager, solvermanager, rundirectory, parameterfile);
  _help.setLevel(-1);
  getSolverManager()->registerVector(_help);
  getSolverManager()->reInit();

}

/*--------------------------------------------------------------------------*/

void Loop::run()
{
  const SolverManager* solvermanager = dynamic_cast<const SolverManager*>(getSolverManager());
  assert(solvermanager);

  // Test 1
  if(_todo=="project")
  {
  initializeSolution(_u);
  solvermanager->projectToAllLevels(_u);
  }
  // Test 2
  else if(_todo=="prolongate")
  {
    solvermanager->testProlongate(_u);
  }
  else if(_todo=="donothing")
  {
    initializeSolution(_u);
  }
  else
  {
    _error_string("run", "unknown toto", _todo);
  }

  getSolverManager()->writeVariablesInfo( );
  getMeshComposition()->writeH5( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "Mesh") );
  getMeshComposition()->writeMeshInfo( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "MeshInfo") );
  getSolverManager()->writeUnknownVariables(_help, _u);
  AlatEnums::residualstatus status;
  int level = 0; // -1: all levels
  getSolverManager()->postProcess(status, _postprocess, _u, _u, level);
  getSolverManager()->writePostProcessVariables(_postprocess);
}
