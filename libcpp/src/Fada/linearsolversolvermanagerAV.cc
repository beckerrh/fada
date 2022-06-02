#include  "Fada/chronometer.h"
#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "Fada/domaindecomposition.h"
#include  "Fada/solvermanagerinterface.h"
#include  "Fada/visitorsolvermanagermultilevel.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/tokenize.h"
#include  "Fada/directsolver.h"
#include  "Fada/iterativesolverselector.h"
#include  "Fada/linearsolversolvermanagerAV.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearSolverSolverManagerAV::~LinearSolverSolverManagerAV()
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

LinearSolverSolverManagerAV::LinearSolverSolverManagerAV(int nlevels, const Alat::GhostLinearSolver& domainlinearsolver,  Fada::SolverManagerInterface* solvermanager) :  Alat::LinearSolverInterface(), Alat::PreconditionerInterface(), _nlevels(nlevels), _domainlinearsolver(domainlinearsolver), _ghostmatrix( domainlinearsolver.getMatrix() ), _solvermanager(solvermanager)
{}

LinearSolverSolverManagerAV::LinearSolverSolverManagerAV( const LinearSolverSolverManagerAV& mlsolversolvermanager) : Alat::LinearSolverInterface(mlsolversolvermanager), Alat::PreconditionerInterface(mlsolversolvermanager), _nlevels(mlsolversolvermanager._nlevels), _domainlinearsolver(mlsolversolvermanager._domainlinearsolver), _ghostmatrix(mlsolversolvermanager._ghostmatrix), _solvermanager(mlsolversolvermanager._solvermanager),  _linearsolvers(mlsolversolvermanager._linearsolvers)
{
  assert(0);
}

LinearSolverSolverManagerAV& LinearSolverSolverManagerAV::operator=( const LinearSolverSolverManagerAV& mlsolversolvermanager)
{
  Alat::LinearSolverInterface::operator=(mlsolversolvermanager);
  assert(0);
  return *this;
}

std::string LinearSolverSolverManagerAV::getName() const
{
  return "LinearSolverSolverManagerAV";
}

LinearSolverSolverManagerAV* LinearSolverSolverManagerAV::clone() const
{
  assert(0);
//return new LinearSolverSolverManagerAV(*this);
}

std::string LinearSolverSolverManagerAV::getInterfaceName() const
{
  return "LinearSolverSolverManagerAV";
}

/*--------------------------------------------------------------------------*/
std::ostream& LinearSolverSolverManagerAV::printLoopInformation(std::ostream& os) const
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->printLoopInformation(os);
  }
  return os;
}

/*--------------------------------------------------------------------------*/
int LinearSolverSolverManagerAV::getNVectors() const
{
  assert(0);
  return 0;
}

int LinearSolverSolverManagerAV::getNLevels() const
{
  return _nlevels;
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManagerAV::reInit()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->reInit();
  }
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManagerAV::compute()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->compute();
  }
}

void LinearSolverSolverManagerAV::computePreconditioner()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->computePreconditioner();
  }
}

void LinearSolverSolverManagerAV::restart()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->restart();
  }
}

bool LinearSolverSolverManagerAV::hasIterationInfo() const
{
  return true;
}

void LinearSolverSolverManagerAV::setsmoothtype(std::string smoothtype)
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->setsmoothtype(smoothtype);
  }
}

void LinearSolverSolverManagerAV::setTolerance(double rtol, double gtol)
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->getIterationInfo()->getRTol() = rtol;
    _linearsolvers[i]->getIterationInfo()->getGlobalTol() = gtol;
  }
}

int LinearSolverSolverManagerAV::getNumberOfIterations() const
{
  int niterations = 0;
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    niterations += _linearsolvers[i]->getIterationInfo()->getNumberOfIterations();
  }
  return niterations;
}

/*--------------------------------------------------------------------------*/
void LinearSolverSolverManagerAV::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    assert(_memory[2*i].getVariables() == Alat::StringSet(_variables[i]));
    assert(_memory[2*i+1].getVariables() == Alat::StringSet(_variables[i]));
    _solvermanager->setVectorFromAllVariables(_memory[2*i], f);
    _linearsolvers[i]->solve(status, A, _memory[2*i+1], _memory[2*i]);
    _solvermanager->setVectorToAllVariables(u, _memory[2*i+1]);
  }
}

void LinearSolverSolverManagerAV::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

void LinearSolverSolverManagerAV::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  assert(visitor == NULL);
  std::string variables;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("variables", &variables, "none");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, blockname, 0);
  if(variables == "none")
  {
    _error_string("basicInit", "variables=V:P|T should be given in block "+ blockname);
  }
  Alat::StringVector bouts = Alat::Tokenize(variables, "|");
  int nbouts = bouts.size();
  _variables.set_size(nbouts);
  for(int ibout = 0; ibout < nbouts; ibout++)
  {
    Alat::StringVector petitsbouts = Alat::Tokenize(bouts[ibout], ":");
    int npetitsbouts = petitsbouts.size();
    _variables[ibout].set_size(npetitsbouts);
    for(int ipetitbout = 0; ipetitbout < npetitsbouts; ipetitbout++)
    {
      _variables[ibout][ipetitbout] = petitsbouts[ipetitbout];
    }
  }
  _linearsolvers.set_size(nbouts);
  for(int i = 0; i < nbouts; i++)
  {
    _linearsolvers[i] = new LinearSolverSolverManager(_variables[i], _nlevels, _domainlinearsolver, _solvermanager);
  }
  _memory.set_size( 2*nbouts );
  for(int i = 0; i < nbouts; i++)
  {
    std::string blocknamei = blockname+"_";
    for(int ii = 0; ii < _variables[i].size(); ii++)
    {
      blocknamei += _variables[i][ii];
    }
    std::cerr << "blocknamei="<<blocknamei<<"\n";
    assert(0);
    // _linearsolvers[i]->basicInit(parameterfile, blocknamei, visitor);

    std::string type = "unknowns";
    std::stringstream ss( getName() );
    ss << i;
    _memory[2*i] = Alat::GhostVector( ss.str()+"f", type, _variables[i] );
    _memory[2*i+1] = Alat::GhostVector( ss.str()+"u", type, _variables[i] );
    _solvermanager->registerVector(_memory[2*i]);
    _solvermanager->registerVector(_memory[2*i+1]);
  }
}
