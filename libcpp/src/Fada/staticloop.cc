#include  "Alat/filescanner.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  "Fada/modelinterface.h"
#include  "Alat/newtondata.h"
#include  "Alat/newtonlavrentiev.h"
#include  "Alat/newtonlavrentievstab.h"
#include  "Alat/newtonlinesearch.h"
#include  "Alat/newtonrb.h"
#include  "Alat/newtonsimple.h"
#include  "Fada/solvermanager.h"
#include  "Fada/staticloop.h"
#include  "Alat/tokenize.h"
#include  "Fada/visitorsolvermanagernonlinear.h"
#include  <fstream>
#include  <sstream>

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
StaticLoop::StaticLoop() : LinearLoop(), _nlsolver(NULL){}
StaticLoop::~StaticLoop()
{
  if(_nlsolver)
  {
    delete _nlsolver;
    _nlsolver = NULL;
  }
}

StaticLoop::StaticLoop( const StaticLoop& loop)
{
  assert(0);
}

StaticLoop& StaticLoop::operator=( const StaticLoop& loop)
{
  assert(0);
  return *this;
}

std::string StaticLoop::getName() const
{
  return "StaticLoop";
}

FadaEnums::looptype StaticLoop::getType() const
{
  return FadaEnums::StaticLoop;
}

/*---------------------------------------------------------*/

void StaticLoop::basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  getChronometer().start("basicInit");
  Alat::DataFormatHandler dataformathandler;
  std::string newton;
  dataformathandler.insert("newton", &newton, "newtonlinesearch_monotonicty");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Loop", 0);

  assert(_nlsolver == NULL);
  Alat::StringVector newtonsplit = Alat::Tokenize(newton, "_");
  if(newtonsplit[0] == "newtonsimple")
  {
    _nlsolver = new Alat::NewtonSimple;
  }
  else if(newtonsplit[0] == "newtonlinesearch")
  {
    assert(newtonsplit.size() == 2);
    _nlsolver = new Alat::NewtonLineSearch(newtonsplit[1]);
  }
  else if(newtonsplit[0] == "newtonlavrentiev")
  {
    assert(newtonsplit.size() == 2);
    _nlsolver = new Alat::NewtonLavrentiev(newtonsplit[1]);
  }
  else if(newtonsplit[0] == "newtonlavrentievstab")
  {
    _nlsolver = new Alat::NewtonLavrentievStab();
  }
  else if(newtonsplit[0] == "newtonlinesearchrb")
  {
    assert(newtonsplit.size() == 2);
    _nlsolver = new Alat::NewtonRb(newtonsplit[1]);
  }
  if(_nlsolver == NULL)
  {
    _error_string("basicInit", "unknwon newton", newton);
  }
  _nlsolver->newVisitorPointer() = new VisitorSolverManagerNonlinear( solvermanager );
  _nlsolver->basicInit(parameterfile, "NonlinearSolver");

  getChronometer().stop("basicInit");
  LinearLoop::basicInit( modelmanager, solvermanager, rundirectory, parameterfile);
}

/*---------------------------------------------------------*/
void StaticLoop::residual(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x) const
{
  getChronometer().start("Residual");
  getSolverManager()->constructForm(status, y, x);
  if(status != AlatEnums::ResidualStatusOk)
  {
    std::cerr << "***WARNING in StaticLoop::Residual() status " <<AlatEnums::residualStatusToString(status) << "\n";
  }
  getChronometer().stop("Residual");
}

/*---------------------------------------------------------*/
void StaticLoop::nonlinearSolve(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& f)
{
  getChronometer().start("nonlinearSolve");
  getSolverManager()->strongDirichletVectorSolution(u);
  assert(_nlsolver);
  newtondata.reset();
  _nlsolver->solve(newtondata, _linearsolverallvars, _A, u, f);
  newtondata.getNNewtonIteration() = _nlsolver->getIterationInfo().getNumberOfIterations();
  newtondata.getFirstResidual() = _nlsolver->getIterationInfo().getFirstResidual();
  // std::cerr<<"StaticLoop::nonlinearSolve(): status "<<AlatEnums::newtonStatusToString( newtondata.getNewtonStatus() )<<'\n';
  getChronometer().stop("nonlinearSolve");
}

/*---------------------------------------------------------*/
void StaticLoop::run()
{
  initializeSolution(_u);
  getChronometer().start("Residual");
  AlatEnums::residualstatus status;
  getSolverManager()->constructRightHandSide(status, _f);
  getChronometer().stop("Residual");
  nonlinearSolve(_newtondata, _u, _f);
  getChronometer().start("InPutOutPut");
  std::string filename = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "IterationInfo");
  std::ofstream file( filename.c_str() );
  if( !file.is_open() )
  {
    _error_string("run", "cannot open file", filename);
  }
  file<<_newtondata.getNNewtonIteration()<<" "<<_newtondata.getNLinearIteration()<<"\n";
  file.close();

  getMeshComposition()->writeH5( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "Mesh") );
  getMeshComposition()->writeMeshInfo( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "MeshInfo") );
  getSolverManager()->writeVariablesInfo( );
  getChronometer().stop("InPutOutPut");
  if(_newtondata.getNewtonStatus() != AlatEnums::NewtonStatusConverged)
  {
		std::cerr << "StaticLoop::run() _nlsolver=" << _nlsolver->getName() << "\n";
    _error_string( "run", "no convergence ! status_newton=", AlatEnums::newtonStatusToString( _newtondata.getNewtonStatus() ) );
  }
  else
  {
    getChronometer().start("InPutOutPut");
    getSolverManager()->writeUnknownVariables(_f, _u);
    // getSolverManager()->postProcess(status, _postprocess, _u, _u);
    // getSolverManager()->writePostProcessVariables(_postprocess);
    getChronometer().stop("InPutOutPut");
  }
}

/*---------------------------------------------------------*/
Alat::NonlinearSolverInterface* StaticLoop::getNonLinearSolver()
{
  return _nlsolver;
}

const Alat::NonlinearSolverInterface* StaticLoop::getNonLinearSolver() const
{
  return _nlsolver;
}
