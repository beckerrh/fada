#include  "Fada/chronometer.h"
#include  "Fada/domainsolver.h"
#include  "Alat/filescanner.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/multilevelmatrix.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Fada/visitordomainsolveronelevel.h"
#include  "Alat/tokenize.h"
#include  "Fada/iterativesolverselector.h"
#include  "Fada/multilevellinearsolver.h"
#include  "Fada/preconditionerdomainsolver.h"
#include  "Alat/umfsolversomevariables.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelLinearSolver::~MultiLevelLinearSolver()
{
  for(int i = 0; i < size(); i++)
  {
    if( ( *this )[i] )
    {
      delete ( *this )[i];
      ( *this )[i] = NULL;
    }
  }
}

MultiLevelLinearSolver::MultiLevelLinearSolver(int nlevels, const Alat::GhostLinearSolver& linearsolver) : Alat::Vector<Alat::LinearSolverInterface*>(nlevels), Fada::MultiLevelSolverInterface(), _linearsolver(linearsolver)
{}

// MultiLevelLinearSolver::MultiLevelLinearSolver( const MultiLevelLinearSolver& multilevellinearsolver) : Alat::Vector<Alat::LinearSolverInterface*>(multilevellinearsolver), Fada::MultiLevelSolverInterface(multilevellinearsolver)
// {
//   assert(0);
// }

MultiLevelLinearSolver& MultiLevelLinearSolver::operator=( const MultiLevelLinearSolver& multilevellinearsolver)
{
  Fada::MultiLevelSolverInterface::operator=(multilevellinearsolver);
  assert(0);
  return *this;
}

std::string MultiLevelLinearSolver::getName() const
{
  std::stringstream ss;
  ss <<  "MultiLevelLinearSolver_nlevels="<< getNLevels()<<"/variables="<<_linearsolver.getVariables();
  return ss.str();
}

MultiLevelLinearSolver* MultiLevelLinearSolver::clone() const
{
  assert(0);
//return new MultiLevelLinearSolver(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& MultiLevelLinearSolver::printLoopInformation(std::ostream& os) const
{
  os << getName() << "\n";
  for( int level = 0; level < getNLevels(); level++ )
  {
    os << level << " : ";
    getLinearSolver( level)->printLoopInformation(os);
    os << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
int MultiLevelLinearSolver::getNLevels() const
{
  return Alat::Vector<Alat::LinearSolverInterface*>::size();
}

bool MultiLevelLinearSolver::allSolversSet() const
{
  for( int level = 0; level < getNLevels(); level++ )
  {
    if( ( *this )[level] == NULL )
    {
      return false;
    }
  }
  return true;
}

/*--------------------------------------------------------------------------*/
Alat::LinearSolverInterface*& MultiLevelLinearSolver::getLinearSolverPointer(int level)
{
  assert( level < getNLevels() );
  return ( *this )[level];
}

void MultiLevelLinearSolver::setLinearSolverPointer(int level, Alat::LinearSolverInterface* solver)
{
  assert( level < getNLevels() );
  ( *this )[level] = solver;
  // std::cerr << "MultiLevelLinearSolver::setLinearSolverPointer() level " << level << " " << getLinearSolver(level)->getName() << "\n";
}

Alat::LinearSolverInterface* MultiLevelLinearSolver::getLinearSolver(int level)
{
  assert( level < getNLevels() );
  assert( ( *this )[level] );
  return ( *this )[level];
}

const Alat::LinearSolverInterface* MultiLevelLinearSolver::getLinearSolver(int level) const
{
  assert( level < getNLevels() );
  assert( ( *this )[level] );
  return ( *this )[level];
}

/*--------------------------------------------------------------------------*/
void MultiLevelLinearSolver::reInit()
{
  for(int level = 0; level < getNLevels(); level++)
  {
    getLinearSolver(level)->reInit();
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelLinearSolver::compute()
{
  for(int level = 0; level < getNLevels(); level++)
  {
    // std::cerr << "MultiLevelLinearSolver::compute() getNLevels()="<<getNLevels()<<" level="<<level<<"\n";
    getLinearSolver(level)->compute();
  }
}

bool MultiLevelLinearSolver::hasIterationInfo() const
{
  return false;
}

/*--------------------------------------------------------------------------*/
void MultiLevelLinearSolver::readParameters(const Alat::ParameterFile* parameterfile)
{
  std::string variablesorder;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("variablesorder", &variablesorder);
  dataformathandler.insert("linearsolver", &_linearsolvername, "none");
  dataformathandler.insert("coarsesolver", &_coarsesolver, "direct");
  dataformathandler.insert("nmaxforcoarsesolver", &_nmaxforcoarsesolver, 500);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, _blockname, 0);

  if(_linearsolvername == "none")
  {
    _error_string("readParameters", "please give \"linearsolver\" in block \"" + _blockname+"\"");
  }
  Alat::StringVector bouts = Alat::Tokenize(variablesorder, ":");
  int nbouts = bouts.size();
  Alat::StringSet linearsolvervars = _linearsolver.getVariables();
  if( nbouts != linearsolvervars.size() )
  {
    _error_string("readParameters", "please give \"variablesorder\" in block \""+_blockname+"\"");
  }
  _variables.set_size(nbouts);
  for(int i = 0; i < nbouts; i++)
  {
    assert( linearsolvervars.find(bouts[i]) != linearsolvervars.end() );
    _variables[i] = bouts[i];
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelLinearSolver::reInit(const Alat::ParameterFile* parameterfile, const Alat::GhostMatrix& ghostmatrix, Fada::DomainSolverInterface* solver)
{
  _blockname = "DomainSolver_";
  Alat::StringSet variableset=_linearsolver.getVariables();
  for(Alat::StringSet::const_iterator p = variableset.begin(); p != variableset.end(); p++)
  {
    _blockname += *p;
  }
  readParameters(parameterfile);

  int nlevels = getNLevels();
  Alat::StringSet linearsolvervars = _linearsolver.getVariables();
  // std::cerr << "MultiLevelLinearSolver::reInit( nlevels = "  <<"\n";
  for(int level = 0; level < nlevels; level++)
  {
    const Alat::SystemMatrixInterface* matrix =  solver->getMatrix(level, ghostmatrix);
    // assert(linearsolvervars==matrix->getVariables());
    std::string linearsolvername;
    solver->getMesh()->setResolution(level);
    int n = solver->getMesh()->getNCells();
    // std::cerr << "MultiLevelLinearSolver::reInit( level = "<< level <<  " n="<<matrix->getMatrix(0,0)->getN() <<"\n";
    // std::cerr << "MultiLevelLinearSolver::reInit( level = "<< level <<  " _linearsolvername="<<_linearsolvername <<"\n";
    // std::cerr << "MultiLevelLinearSolver::reInit( n = "<< n <<  " _nmaxforcoarsesolver="<<_nmaxforcoarsesolver <<"\n";
    if(level == nlevels-1 or n < _nmaxforcoarsesolver)
    {
      linearsolvername = _coarsesolver;
    }
    else
    {
      linearsolvername = _linearsolvername;
    }
    // std::cerr << "MultiLevelLinearSolver::reInit( level = "<< level <<  " linearsolvername="<<linearsolvername <<"\n";
    Fada::IterativeSolverSelector iterativesolverselector;
    Alat::LinearSolverInterface* iterativesolver = iterativesolverselector.newSolver(linearsolvername);

    Alat::IterativeSolverWithVisitor* iterativesolverwv = dynamic_cast<Alat::IterativeSolverWithVisitor*>( iterativesolver );
    assert(iterativesolverwv);
    iterativesolverwv->newVisitorPointer() = new Fada::VisitorDomainSolverOneLevel(solver, level, _variables);

    Alat::UmfSolverSomeVariables* umfsolver = dynamic_cast< Alat::UmfSolverSomeVariables*>( iterativesolver );
    if(umfsolver)
    {
      int nvars = _variables.size();
      Alat::IntVector nofvars, ncompofvars;
      nofvars.set_size(nvars);
      ncompofvars.set_size(nvars);
      for(int ii = 0; ii < nvars; ii++)
      {
        std::string varname = _variables[ii];
        nofvars[ii] = solver->getNOfVar(level, varname);
        ncompofvars[ii] = solver->getNcompOfVar(varname);
      }
      
      umfsolver->setMatrix( level, matrix, _variables, ncompofvars, nofvars);
    }
    else
    {
      Alat::IterativeSolverWithPreconditioner*  iterativesolverwp = dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( iterativesolverwv );
      if(iterativesolverwp)
      {
        iterativesolverwp->newPreconditionerPointer() = new Fada::PreconditionerDomainSolver( matrix, _variables );
        iterativesolverwp->basicInit( solver->getParameterFile(), _blockname);
      }
    }
    // UmfSolverAllVariables* umfsolver = dynamic_cast< UmfSolverAllVariables*>( iterativesolver );
    getLinearSolverPointer(level) = iterativesolver;
    solver->getMesh()->setResolution( level );
    getLinearSolver(level)->reInit();
  }
}
