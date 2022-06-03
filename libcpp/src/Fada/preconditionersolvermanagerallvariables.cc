#include  "Fada/preconditionersolvermanagerallvariables.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/map.h"
#include  "Fada/directsolver.h"
#include  "Fada/linearsolversolvermanager.h"
#include  "Fada/linearsolversolvermanagerAV.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
PreconditionerSolverManagerAllvariables::~PreconditionerSolverManagerAllvariables()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    if( _linearsolvers[i] )
    {
      delete _linearsolvers[i];
      _linearsolvers[i] = NULL;
    }
  }
}

PreconditionerSolverManagerAllvariables::PreconditionerSolverManagerAllvariables(const Alat::Map<Alat::StringVector, std::string>& solvernames, const Alat::GhostMatrix& ghostmatrix, Fada::SolverManager* solvermanager) : Alat::Preconditioner(), _ghostmatrix(ghostmatrix), _solvermanager(solvermanager)
{
  int n = solvernames.size();
  _variablenams.set_size(n);
  _solvernames.set_size(n);
  int count = 0;
  for(Alat::Map<Alat::StringVector, std::string>::const_iterator p = solvernames.begin(); p != solvernames.end(); p++)
  {
    _solvernames[count] = p->second;
    _variablenams[count].resize( p->first.size() );
    _variablenams[count] = p->first;
    count++;
  }
}

PreconditionerSolverManagerAllvariables::PreconditionerSolverManagerAllvariables( const PreconditionerSolverManagerAllvariables& preconditionersolvermanagerallvariables) : Alat::Preconditioner(preconditionersolvermanagerallvariables)
{
  ( *this ).operator=(preconditionersolvermanagerallvariables);
}

PreconditionerSolverManagerAllvariables& PreconditionerSolverManagerAllvariables::operator=( const PreconditionerSolverManagerAllvariables& preconditionersolvermanagerallvariables)
{
  Alat::Preconditioner::operator=(preconditionersolvermanagerallvariables);
  assert(0);
  return *this;
}

std::string PreconditionerSolverManagerAllvariables::getName() const
{
  return "PreconditionerSolverManagerAllvariables";
}

PreconditionerSolverManagerAllvariables* PreconditionerSolverManagerAllvariables::clone() const
{
  return new PreconditionerSolverManagerAllvariables(*this);
}

/*--------------------------------------------------------------------------*/
int PreconditionerSolverManagerAllvariables::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
std::ostream& PreconditionerSolverManagerAllvariables::printLoopInformation(std::ostream& os) const
{
  int nsolvers = _linearsolvers.size();
  os << getName() << " nsolvers=" << nsolvers <<"\n";
  for(int isolver = 0; isolver < nsolvers; isolver++)
  {
    os << isolver << " ";
    _linearsolvers[isolver]->printLoopInformation(os);
  }
  return os;
}

/*--------------------------------------------------------------------------*/
void PreconditionerSolverManagerAllvariables::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  Preconditioner::basicInit(parameterfile, blockname, visitor);

  const Fada::VisitorSolverManagerOneLevel* visitorsm = dynamic_cast<const Fada::VisitorSolverManagerOneLevel*>( visitor );
  assert(visitorsm);
  int level = visitorsm->getVectorLevel();
  bool integration = false;

  int nsolvers = _solvernames.size();
  _linearsolvers.set_size(nsolvers);
  for(int isolver = 0; isolver < nsolvers; isolver++)
  {
    std::string solvername = _solvernames[isolver];
    const Alat::StringVector& variables = _variablenams[isolver];
    // std::cerr << "PreconditionerSolverManagerAllvariables::basicInit() solvername="<<solvername  <<" variables="<<variables<<"\n";
    if(solvername == "direct")
    {
      Fada::DirectSolver* directsolver =  new Fada::DirectSolver( level, _ghostmatrix, _solvermanager, variables);
      _linearsolvers[isolver] = directsolver;
    }
    else if(solvername == "mg")
    {
      int nlevels = _solvermanager->getNlevels();
      assert(_ghostmatrix.getLevel() == -1);
      std::stringstream ss;
      ss<<"LinearSolverSolverManager"<<"_"<<isolver;
      std::string name = ss.str();
      int level = -1;
      Alat::GhostLinearSolver domainlinearsolver(name, _ghostmatrix, level);
      domainlinearsolver.setVariables(variables);
      domainlinearsolver.setDescription("some_variables");
      _solvermanager->addLinearDomainSolvers(domainlinearsolver);
      _linearsolvers[isolver] = new Fada::LinearSolverSolverManager(variables, nlevels, domainlinearsolver, _solvermanager);
      // _linearsolvers[isolver] = new Fada::LinearSolverSolverManagerAV(nlevels, domainlinearsolver, _solvermanager);
    }
    else
    {
			std::cerr << "unknown solvername: " << solvername << "\n";
      assert(0);
    }
    Alat::IterativeSolverWithVisitor* solver = dynamic_cast<Alat::IterativeSolverWithVisitor*>( _linearsolvers[isolver] );
    if(solver)
    {
      solver->newVisitorPointer() = new Fada::VisitorSolverManagerOneLevel(_solvermanager, level, variables, integration);
    }
  }
  for(int isolver = 0; isolver < nsolvers; isolver++)
  {
    _linearsolvers[isolver]->basicInit(parameterfile, blockname);
  }
  _memory.set_size( 2*nsolvers );
  for(int isolver = 0; isolver < nsolvers; isolver++)
  {
    int level = 0;
    std::string type = "unknowns";
    std::stringstream ss( getName() );
    ss << isolver;
    _memory[2*isolver] = Alat::GhostVector( ss.str()+"f", type, level );
    _memory[2*isolver+1] = Alat::GhostVector( ss.str()+"u", type, level );
    _memory[2*isolver].setVariables(_variablenams[isolver]);
    _memory[2*isolver+1].setVariables(_variablenams[isolver]);
    _memory[2*isolver].setDescription("somevars");
    _memory[2*isolver+1].setDescription("somevars");
    _solvermanager->registerVector(_memory[2*isolver]);
    _solvermanager->registerVector(_memory[2*isolver+1]);
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerSolverManagerAllvariables::reInit()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->reInit();
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerSolverManagerAllvariables::computePreconditioner()
{
  for(int i = 0; i < _linearsolvers.size(); i++)
  {
    _linearsolvers[i]->compute();
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerSolverManagerAllvariables::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  _solvermanager->vectorZero(u);
  for(int isolver = 0; isolver < _linearsolvers.size(); isolver++)
  {
    // std::cerr << "PreconditionerSolverManagerAllvariables::solveApproximate() solving with " << _linearsolvers[isolver]->getName() << " for variables "<<_variablenams[isolver]<<"\n";

    Alat::GhostVector& ui = _memory[2*isolver];
    Alat::GhostVector& fi = _memory[2*isolver+1];
    _solvermanager->setVariableVectorFromAll(fi, f);
    _solvermanager->setVariableVectorFromAll(ui, u);
    for(int jsolver = 0; jsolver < isolver; jsolver++)
    {
      Alat::GhostVector& uj = _memory[2*jsolver];
      // _solvermanager->setVariableVectorFromAll(uj, u);
      getVisitor()->matrixVectorProduct(A, fi, uj, -1.0);
    }
    _linearsolvers[isolver]->solve(status, A, ui, fi);
    _solvermanager->setVariableVectorToAll(u, ui);
  }
}
