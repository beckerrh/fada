#include  "Fada/chronometer.h"
#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "Fada/solvermanager.h"
#include  "Fada/visitorsolvermanagermultilevel.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/tokenize.h"
#include  "Fada/directsolver.h"
#include  "Fada/domaindecomposition.h"
#include  "Fada/iterativesolverselector.h"
#include  "Fada/linearsolversolvermanager.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearSolverSolverManager::~LinearSolverSolverManager()
{
  // _chronometer.print(std::cerr);
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    if( _linearsolvers[i] )
    {
      delete _linearsolvers[i];
      _linearsolvers[i] = NULL;
    }
  }
}

LinearSolverSolverManager::LinearSolverSolverManager(const Alat::StringVector& variables, int nlevels, const Alat::GhostLinearSolver& domainlinearsolver,  Fada::SolverManager* solvermanager) :  Alat::LinearSolverInterface(), Alat::PreconditionerInterface(), _variables(variables), _domainlinearsolver(domainlinearsolver), _ghostmatrix( domainlinearsolver.getMatrix() ), _solvermanager(solvermanager), _basicinitcalled(false), _multigrid( _solvermanager->getIoManager()->getDirectoryName(Alat::IoManager::RunInfo) )
{
  _dirname = _solvermanager->getIoManager()->getDirectoryName(Alat::IoManager::RunInfo);
  _linearsolvers.set_size(nlevels);
}

LinearSolverSolverManager::LinearSolverSolverManager( const LinearSolverSolverManager& mlsolversolvermanager) : Alat::LinearSolverInterface(mlsolversolvermanager), Alat::PreconditionerInterface(mlsolversolvermanager), _variables(mlsolversolvermanager._variables), _domainlinearsolver(mlsolversolvermanager._domainlinearsolver), _ghostmatrix(mlsolversolvermanager._ghostmatrix), _solvermanager(mlsolversolvermanager._solvermanager), _dirname(mlsolversolvermanager._dirname), _multigrid(mlsolversolvermanager._multigrid), _linearsolvers(mlsolversolvermanager._linearsolvers)
{
  assert(0);
}

LinearSolverSolverManager& LinearSolverSolverManager::operator=( const LinearSolverSolverManager& mlsolversolvermanager)
{
  Alat::LinearSolverInterface::operator=(mlsolversolvermanager);
  assert(0);
  return *this;
}

std::string LinearSolverSolverManager::getName() const
{
  std::string name= "LinearSolverSolverManager_";
  Alat::StringSet variableset(_variables);
  for(Alat::StringSet::const_iterator p = variableset.begin();p!=variableset.end();p++)
  {
    name += *p;
  }
  return name;
}

LinearSolverSolverManager* LinearSolverSolverManager::clone() const
{
  assert(0);
//return new LinearSolverSolverManager(*this);
}

std::string LinearSolverSolverManager::getInterfaceName() const
{
  return "LinearSolverSolverManager";
}

/*--------------------------------------------------------------------------*/
std::ostream& LinearSolverSolverManager::printLoopInformation(std::ostream& os) const
{
  os << "\t" << getName() << "\t";
  _multigrid.printLoopInformation(os);
  os <<"\n";
  for(int level = 0; level < getNLevels(); level++)
  {
    os << "\tlevel="<<level<<" ";
    _linearsolvers.getLinearSolver(level)->getName();
    os << "\n";
  }
  return os;
}

void LinearSolverSolverManager::printChronometer(std::ostream& os) const
{
  _multigrid.printChronometer(os);
}

/*--------------------------------------------------------------------------*/
const Fada::LinearSolverVector& LinearSolverSolverManager::getLinearSolvers() const
{
  return _linearsolvers;
}

Fada::LinearSolverVector& LinearSolverSolverManager::getLinearSolvers()
{
  return _linearsolvers;
}

int LinearSolverSolverManager::getNVectors() const
{
  return 0;
}

int LinearSolverSolverManager::getNLevels() const
{
  return _linearsolvers.getNLevels();
}

bool LinearSolverSolverManager::allSolversSet() const
{
  return _linearsolvers.allSolversSet();
}

/*--------------------------------------------------------------------------*/
Alat::LinearSolverInterface*& LinearSolverSolverManager::getLinearSolverPointer(int level)
{
  return _linearsolvers.getLinearSolverPointer(level);
}

Alat::LinearSolverInterface* LinearSolverSolverManager::getLinearSolver(int level)
{
  return _linearsolvers.getLinearSolver(level);
}

const Alat::LinearSolverInterface* LinearSolverSolverManager::getLinearSolver(int level) const
{
  return _linearsolvers.getLinearSolver(level);
}

bool LinearSolverSolverManager::hasIterationInfo() const
{
  return true;
}

const Alat::IterationInfo*  LinearSolverSolverManager::getIterationInfo() const
{
  return _multigrid.getIterationInfo();
}

Alat::IterationInfo* LinearSolverSolverManager::getIterationInfo()
{
  return _multigrid.getIterationInfo();
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::setsmoothtype(std::string smoothtype)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::reInit()
{
  assert(_basicinitcalled);
  _multigrid.reInit();
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::compute()
{
  _multigrid.compute();
  // int nlevels = getNLevels();
  // for(int level = 0; level < nlevels; level++)
  // {
  //   if( ( level == nlevels-1 )&&( _coarsesolver == "direct" ) )
  //   {
  //     continue;
  //   }
  //   Alat::IterativeSolverWithPreconditioner*  iterativesolverwp = dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( _linearsolvers.getLinearSolverPointer(level) );
  //   assert(iterativesolverwp);
  //   Fada::VisitorSolverManagerOneLevel* visitorsolvermanageronelevelallvars = dynamic_cast<Fada::VisitorSolverManagerOneLevel*>( iterativesolverwp->getVisitor() );
  //   assert(visitorsolvermanageronelevelallvars);
  //   visitorsolvermanageronelevelallvars->setLinearizationPoint(u);
  // }
}

void LinearSolverSolverManager::computePreconditioner()
{
  this->compute();
}

void LinearSolverSolverManager::restart()
{
  _multigrid.restart();
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  // std::cerr << "LinearSolverSolverManager::solveApproximate() solve _variables="<<_variables<<"\n";
  // std::cerr << "LinearSolverSolverManager::solveApproximate() solve u="<<u<<"\n";
  // std::cerr << "LinearSolverSolverManager::solveApproximate() solve f="<<f<<"\n";

  _linearsolvers.restart();
  _chronometer.start("LinearSolve");
  _multigrid.solve(status, A, u, f);
  _chronometer.stop("LinearSolve");
}

void LinearSolverSolverManager::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  _multigrid.solveApproximate(status, A, u, f, iteration);
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::readParameters(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("smoother", &_smoother, "domaindecomposition_jacobi");
  dataformathandler.insert("smootheriteration", &_smootheriteration, "richardson");
  dataformathandler.insert("coarsesolver", &_coarsesolver, "direct");
  dataformathandler.insert("integration", &_integration, false);
  // dataformathandler.insert("ddvariables", &_ddvariables, "none");

  Alat::FileScanner filescanner(dataformathandler, parameterfile, blockname, 0);
  // std::cerr << "LinearSolverSolverManager::readParameters() blockname="<<blockname<<" integration="<<_integration<<" _coarsesolver="<<_coarsesolver<<"\n";
  // assert(0);
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManager::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _chronometer.enrol("LinearSolve", true);
  assert(_basicinitcalled == false);
  _basicinitcalled = true;
  _blockname="MultiLevelLinearSolver_";
  Alat::StringSet variableset(_variables);
  for(Alat::StringSet::const_iterator p = variableset.begin();p!=variableset.end();p++)
  {
    _blockname += *p;
  }
  // readParameters(parameterfile, blockname);
  readParameters(parameterfile, _blockname);

  int nlevels = getNLevels();
  for(int level = 0; level < nlevels; level++)
  {
    if( ( level == nlevels-1 )&&( _coarsesolver == "direct" ) )
    {
      bool integration = false;
      Fada::DirectSolver* directsolver = new Fada::DirectSolver( level, _ghostmatrix, _solvermanager, _variables);
      directsolver->newVisitorPointer() = new Fada::VisitorSolverManagerOneLevel(_solvermanager, level, _variables, integration);
      _linearsolvers.getLinearSolverPointer(level) = directsolver;
    }
    else
    {
      Fada::IterativeSolverSelector iterativesolverselector;
      if(_smootheriteration == "none")
      {
        _error_string("basicInit", "variable \"smootheriteration\" has to be set in block \""+ blockname+"\"");
      }
      else if(_smootheriteration == "simple")
      {
        _error_string("basicInit", "\"smootheriteration=simple\" is not possible for multigrid");
      }
      Alat::IterativeSolverWithPreconditioner*  iterativesolverwp = dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( iterativesolverselector.newSolver(_smootheriteration) );
      assert(iterativesolverwp);
      // std::cerr << "LinearSolverSolverManager::basicInit() _variables="<<_variables<<" level="<<level<<"\n";
      iterativesolverwp->newVisitorPointer() = new Fada::VisitorSolverManagerOneLevel(_solvermanager, level, _variables, _integration);

      Alat::StringVector bouts = Alat::Tokenize(_smoother, "_");
      int nbouts = bouts.size();
      if(nbouts != 2)
      {
        _error_string("basicInit", "smoother="+_smoother+" read in Block "+ blockname);
      }
      assert(bouts[1]=="jacobi");
      iterativesolverwp->newPreconditionerPointer() = new Fada::DomainDecomposition(bouts[1], level, _solvermanager, _ghostmatrix, _domainlinearsolver);
      iterativesolverwp->basicInit( parameterfile, blockname );

      _linearsolvers.getLinearSolverPointer(level) = iterativesolverwp;
    }
  }
  _multigrid.newVisitorPointer() = new Fada::VisitorSolverManagerMultiLevel(_solvermanager, 0, nlevels-1, _linearsolvers, _variables);
  _multigrid.setLinearSolverVector(&_linearsolvers);
  _multigrid.basicInit( parameterfile, _blockname);
}
