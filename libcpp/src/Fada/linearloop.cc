#include  "Fada/linearloop.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  "Fada/model.h"
#include  "Fada/solver.h"
#include  "Fada/solvermanagerinterface.h"
#include  <fstream>

using namespace Fada;

/*---------------------------------------------------------*/
LinearLoop::LinearLoop() : Loop(), _f("f", "unknowns"), _A("A"), _linearsolverallvars("linearsolverallvars", _A)
{}

LinearLoop::~LinearLoop()
{
  std::string filename = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "Timer") + FadaEnums::loopTypeToString(getType());
  getSolverManager()->printChronometer(filename);
  // std::ofstream file( filename.c_str(), std::ios::app );
  // std::ofstream file( filename.c_str() );
  // getSolverManager()->getChronometer().print(file);
  // file.close();
}

LinearLoop::LinearLoop( const LinearLoop& loop) : Loop(loop), _f(loop._f), _A(loop._A), _linearsolverallvars(loop._linearsolverallvars)
{
  assert(0);
}

LinearLoop& LinearLoop::operator=( const LinearLoop& loop)
{
  assert(0);
  return *this;
}

std::string LinearLoop::getName() const
{
  return "LinearLoop";
}

FadaEnums::looptype LinearLoop::getType() const
{
  return FadaEnums::StaticLoop;
}

/*---------------------------------------------------------*/
void LinearLoop::basicInit(ModelManagerInterface* modelmanager, SolverManagerInterface* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  // std::cerr << "@@@ LinearLoop::basicInit() BEGIN\n";
  Loop::basicInit(modelmanager, solvermanager, rundirectory, parameterfile);
  getChronometer().start("basicInit");
  if(_vectortype=="ml")
  {
    _f.setLevel(-1);
  }
  getSolverManager()->registerVector(_f);
  getSolverManager()->registerMatrix(_A);
  getSolverManager()->registerLinearSolver(_linearsolverallvars);
  getSolverManager()->reInitMatrixAndLinearSolver();
  getSolverManager()->reInit();

  // std::string filename = getIoManager().getDirectoryName(Alat::IoManager::RunInfo)+"/LoopInformation";
  // std::ofstream file;
  // file.open( filename.c_str() );
  // assert( file.is_open() );
  // printLoopInformation(file);
  // file.close();

  getChronometer().stop("basicInit");
  // std::cerr << "@@@ LinearLoop::basicInit() END\n";
}

/*---------------------------------------------------------*/
void LinearLoop::linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostVector& x, const Alat::GhostVector& b)
{
  assert(0);
  // std::cerr << "LinearLoop::linearSolve()\n";
  getChronometer().start("LinearSolve");
  getSolverManager()->linearSolve(status, A, _linearsolverallvars, x, b);
  getChronometer().stop("LinearSolve");
}

/*---------------------------------------------------------*/
void LinearLoop::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A,  Alat::GhostVector& u)
{
  assert(0);
  getChronometer().start("Jacobian");
  // getSolverManager()->computeLinearizationPoint(u);
  getSolverManager()->matrixZero(A);
  getSolverManager()->constructJacobianMatrix(status, A, u);
  getChronometer().stop("Jacobian");
}

/*---------------------------------------------------------*/
void LinearLoop::constructLinearSolver(Alat::GhostMatrix& A, const Alat::GhostVector& u)
{
  assert(0);
  getChronometer().start("constructLinearSolver");
  assert(_linearsolverallvars.getMatrix() == A);
  getSolverManager()->constructLinearSolver(_linearsolverallvars, u);
  getChronometer().stop("constructLinearSolver");
}

/*---------------------------------------------------------*/
void LinearLoop::run()
{
  initializeSolution(_u);
  getChronometer().start("Residual");
  getSolverManager()->vectorZero(_f);
  AlatEnums::residualstatus residualstatus;
  getSolverManager()->constructRightHandSide(residualstatus, _f);
  getChronometer().stop("Residual");

  constructMatrixAndLinearSolvers(_A, _u);
  AlatEnums::iterationstatus status;
  linearSolve( status, _A, _u, _f);

  getSolverManager()->writeVariablesInfo( );

  // getNonLinearInfo().printInfo( getIoManager().getFileNameOut(Alat::IoManager::RunInfo,"IterationInfo") );
  getMeshComposition()->writeH5( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "Mesh") );
  getMeshComposition()->writeMeshInfo( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "MeshInfo") );

  getChronometer().start("InPutOutPut");
  getSolverManager()->writeUnknownVariables(_f, _u);
  getChronometer().stop("InPutOutPut");
}
