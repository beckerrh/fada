#include  "Alat/filescanner.h"
#include  "Alat/linearsolverinterface.h"
#include  "Alat/newtondata.h"
#include  "Alat/nonlinearsolver.h"
#include  "Alat/nonlinearsolvervisitorinterface.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <sstream>
#include  <limits>

using namespace Alat;

int NonlinearSolver::_totalids = 0;

/*--------------------------------------------------------------------------*/
NonlinearSolver::~NonlinearSolver()
{
  if(_deletevisitor && _visitor)
  {
    delete _visitor;
    _visitor = NULL;
  }
}

NonlinearSolver::NonlinearSolver() : NonlinearSolverInterface(), _visitor(NULL), _deletevisitor(true), _basicinitcalled(false), _info(), _newmatrix(true), _lintol(0.0), _dxnorm(0.0)
{
  _id = _totalids;
  _totalids++;
}

NonlinearSolver::NonlinearSolver( const NonlinearSolver& nonlinearsolver) : NonlinearSolverInterface(nonlinearsolver)
{
  ( *this ).operator=(nonlinearsolver);
}

NonlinearSolver& NonlinearSolver::operator=( const NonlinearSolver& nonlinearsolver)
{
  NonlinearSolverInterface::operator=(nonlinearsolver);
  assert(0);
  return *this;
}

std::string NonlinearSolver::getName() const
{
  return "NonlinearSolver";
}

NonlinearSolver* NonlinearSolver::clone() const
{
  assert(0);
  // return new NonlinearSolver(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& NonlinearSolver::printLoopInformation(std::ostream& os) const
{
  os << "\"" << getName() << "\" ";
  getVisitor()->printLoopInformation(os);
  os << " info: ";  
  getIterationInfo().printLoopInformation(os);
  os << " ";
  return os;
}

/*--------------------------------------------------------------------------*/
Alat::NonlinearSolverVisitorInterface*& NonlinearSolver::newVisitorPointer()
{
  assert(_visitor == NULL);
  _deletevisitor = true;
  return _visitor;
}

void NonlinearSolver::setVisitorPointer(Alat::NonlinearSolverVisitorInterface* visitor)
{
  _visitor = visitor;
  _deletevisitor = false;
}

const Alat::NonlinearSolverVisitorInterface* NonlinearSolver::getVisitor() const
{
  assert(_visitor);
  return _visitor;
}

Alat::NonlinearSolverVisitorInterface* NonlinearSolver::getVisitor()
{
  assert(_visitor);
  return _visitor;
}

const IterationInfo& NonlinearSolver::getIterationInfo() const
{
  return _info;
}

IterationInfo& NonlinearSolver::getIterationInfo()
{
  return _info;
}

/*--------------------------------------------------------------------------*/
Alat::GhostVector& NonlinearSolver::getMemory(int i) const
{
  if( i >= _memory.size() )
  {
    _error_string("getMemory", "", "too small memory");
  }
  return _memory[i];
}

void NonlinearSolver::memory()
{
  _memory.set_size( getNVectors() );
  std::string type = getVisitor()->getVectorType();
  int level = getVisitor()->getVectorLevel();
  // std::cerr << "IterativeSolverWithVisitor::memory() in " << getName() << " visitor = "<<getVisitor()->getName() << " " << getNVectors()  << " of type " << type << "\n";
  for(int i = 0; i < _memory.size(); i++)
  {
    std::stringstream ss;
    ss<<getName()<<"_"<<_id << "_" <<getVisitor()->getName()<<"_memory_"<<i;
    _memory[i] = Alat::GhostVector( ss.str(), type, level );
    // std::cerr << "newVector: " << _memory[i] << "\n";
    getVisitor()->newVector(&_memory[i]);
  }
  // assert(0);
}

/*--------------------------------------------------------------------------*/
void NonlinearSolver::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  assert(_basicinitcalled == false);
  std::string paramblockname = blockname;
  if(blockname == "none")
  {
    paramblockname = getName();
  }
  // std::cerr << "NonlinearSolver::basicInit() " << getName() << " blockname " << blockname<< " paramblockname " << paramblockname <<"\n";
  // assert(0);

  // propres parametres
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("lineartoleranceincrease", &_lineartoleranceincrease, 0.2);
  dataformathandler.insert("rhomatrix", &_rhomatrix, 0.1);
  dataformathandler.insert("maxnlinesearch", &_maxnlinesearch, 10);
  dataformathandler.insert("omegalinesearch", &_omegalinesearch, 0.7);
  dataformathandler.insert("monotonyfactor", &_monotonyfactor, 1.0);
  dataformathandler.insert("printlinesearch", &_printlinesearch, false);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, paramblockname, 0);

  // visiteur
  getVisitor()->basicInit(parameterfile, paramblockname);
  // info
  getIterationInfo().basicInit(parameterfile, blockname);
	getIterationInfo().setId(getName());

  memory();
  _basicinitcalled = true;


  // std::cerr << "NonlinearSolver::basicInit() _monotonyfactor="<<_monotonyfactor<<" _maxnlinesearch="<<_maxnlinesearch<<"\n";
  // assert(0);
}

/*--------------------------------------------------------------------------*/
bool NonlinearSolver::_checkIteration(Alat::NewtonData& newtondata, const Alat::IterationInfo& info) const
{
  AlatEnums::newtonstatus& status = newtondata.getNewtonStatus();

  std::stringstream infobuffer;
  infobuffer << " "<< std::setiosflags(std::ios::scientific) << std::setprecision(3) << _dxnorm;
  if(newtondata.use_linesearch)
  {
    infobuffer << " " << std::setw(2) << newtondata.irelax;
  }
  if(newtondata.use_regularization)
  {
    infobuffer << " ["<< std::setiosflags(std::ios::scientific) << std::setprecision(2) << newtondata.modelfactor <<" "<<newtondata.regularization << "]";
  }
  if(_newmatrix)
  {
    infobuffer << " M";
  }
  else
  {
    infobuffer << " -";
  }
  infobuffer <<  " (" << _lintol << " " << std::setw(3) << _nliniter << ")\n";
  info._printbuff = infobuffer.str();
  info.checkIteration(_status_iteration, newtondata.residual, newtondata.iteration);
  if(newtondata.residual>std::numeric_limits<double>::max())
  {
    std::cerr << "Je ne mange pas de nan !\n";
    assert(0);
  }
  if(_status_iteration == AlatEnums::IterationStatusConverged)
  {
    status = AlatEnums::NewtonStatusConverged;
  }
  else if(_status_iteration == AlatEnums::IterationStatusDiverged)
  {
    status = AlatEnums::NewtonStatusDiverged;
  }
  double rho = info.getLastReductionRate();
  // std::cerr << "NonlinearSolver::_checkIteration() rho="<<rho<<"\n";
  if(rho < _rhomatrix)
  {
    _newmatrix = false;
  }
  else
  {
    _newmatrix = true;
  }

  // if(newtondata.iteration)
  // {
  //   double rho = info.getLastReductionRate();
    _lintol = fmin(rho, 1.0)*_lineartoleranceincrease;
  // }
  // else
  // {
  //   _lintol = _lineartoleranceincrease;
  // }
  _lintol = fmax( _lintol, 0.99*info.getMissingRate() );
  // std::cerr << "newtondata.iteration="<<newtondata.iteration<< " _newmatrix="<<_newmatrix<<" rho="<<rho<<" _rhomatrix="<<_rhomatrix<<" _lintol"<<_lintol<<" _lineartoleranceincrease"<<_lineartoleranceincrease<<"\n";
  // std::cerr << "NonlinearSolver==_checkIteration() status="<<AlatEnums::newtonStatusToString(status)<<"\n";
  if( status == AlatEnums::NewtonStatusConverged or status == AlatEnums::NewtonStatusDiverged or (newtondata.use_linesearch and status == AlatEnums::NewtonStatusMaxLineSearchAttained) )
  {
    return true;
  }
  else
  {
    if(_newmatrix)
    {
     newtondata.getNRedoMatrix()++;
    }
   return false;
  }
}

/*--------------------------------------------------------------------------*/
void NonlinearSolver::_lineSearchMonotonicity(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f) const
{
  newtondata.irelax = 0;
  _omega = 1.0;
  double omegaold;
  while(newtondata.residual > _monotonyfactor*newtondata.residual_old)
  {
    if(newtondata.irelax == _maxnlinesearch)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusMaxLineSearchAttained;
      return;
      // _error_string("_lineSearchMonotonicity", "maxiter linesearch attained", _maxnlinesearch);
    }
    newtondata.irelax++;
    if(_printlinesearch)
    {
      std::cerr << "irelax="<<newtondata.irelax << " res="<<newtondata.residual << "resold="<<newtondata.residual_old  << "_monotonyfactor*resold="<<_monotonyfactor*newtondata.residual_old <<"\n";
    }
    omegaold = _omega;
    _omega = omegaold*_omegalinesearch;
    // std::cerr << "omegaold="<<omegaold << " omega="<<omega<<"\n";
    getVisitor()->vectorAdd(u, _omega-omegaold, du);
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
  }
}

/*--------------------------------------------------------------------------*/
void NonlinearSolver::_lineSearchArmijo(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f, double kappa) const
{
  //
  // on veut  \phi(x+\omega*dx) \le \phi(x) + c1 \omega* dx*\phi'(x)dx
  // avec \phi(x) = \fra12\|b-f(x)\|^2, donc  \phi'(x) = -r*f'(x)dx
  // cela donne
  // res^2 \le resold^2 - c1*\omega\kappa \qquad  \kappa=r*f'(x)dx
  //
  newtondata.irelax = 0;
  _omega = 1.0;
  double omegaold;
  bool notok = ( newtondata.residual*newtondata.residual > newtondata.residual_old*newtondata.residual_old - 0.0001*_omega*kappa );
  while(notok)
  {
    // std::cerr << "_lineSearchArmijo() res="<<res <<" resold="<<resold<<" kappa="<<kappa<< " omega="<<omega<<"\n";
    if(newtondata.irelax == _maxnlinesearch)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusMaxLineSearchAttained;
      return;
      // _error_string("_lineSearchArmijo", "maxiter linesearch attained", _maxnlinesearch);
    }
    newtondata.irelax++;
    omegaold = _omega;
    _omega = omegaold*_omegalinesearch;
    getVisitor()->vectorAdd(u, _omega-omegaold, du);
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    notok = ( newtondata.residual*newtondata.residual > newtondata.residual_old*newtondata.residual_old - 0.0001*_omega*kappa );
  }
}

/*--------------------------------------------------------------------------*/
void NonlinearSolver::_lineSearchWolfe(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& du, Alat::GhostVector& r, const Alat::GhostVector& f, double kappa) const
{
  // de plus (par rapport à Armijo) on demande
  //    \phi'(x+omega*dx)dx \ge c2   \phi'(x)dx
  //    \|\phi'(x+omega*dx)dx\| \le c2   \|\phi'(x)dx\|
  newtondata.irelax = 0;
  _omega = 1.0;
  double omegaold;
  Alat::GhostVector& h = getMemory(2);
  AlatEnums::residualstatus status_linearization = AlatEnums::ResidualStatusOk;

  getVisitor()->vectorZero(h);
  getVisitor()->computeLinearization(newtondata.getResidualStatus(), h, u, du);
  if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
  {
    _error_string( "_lineSearchWolfe", "linearization not ok", AlatEnums::residualStatusToString(status_linearization) );
  }
  double kappanew = getVisitor()->scalarProduct(r, h);
  // std::cerr << "_lineSearchArmijo() res="<<res <<" resold="<<resold<<" kappa="<<kappa<<" kappanew="<<kappanew<< " omega="<<omega<<"\n";

  // bool notok = ( res*res > resold*resold - 0.0001*omega*kappa ) or ( kappanew < 0.1*kappa );
  bool notok = ( newtondata.residual*newtondata.residual > newtondata.residual_old*newtondata.residual_old - 0.0001*_omega*kappa )or ( fabs(kappanew) < 0.1*fabs(kappa) );
  while(notok)
  {
    // std::cerr << "_lineSearchArmijo() res="<<res <<" resold="<<resold<<" kappa="<<kappa<<" kappanew="<<kappanew<< " omega="<<omega<<"\n";
    if(newtondata.irelax == _maxnlinesearch)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusMaxLineSearchAttained;
      return;
      // _error_string("_lineSearchWolfe", "maxiter linesearch attained", _maxnlinesearch);
    }
    newtondata.irelax++;
    omegaold = _omega;
    _omega = omegaold*_omegalinesearch;
    getVisitor()->vectorAdd(u, _omega-omegaold, du);
    newtondata.residual = getVisitor()->residual(newtondata.getResidualStatus(), r, u, f);
    getVisitor()->vectorZero(h);
    getVisitor()->computeLinearization(newtondata.getResidualStatus(), h, u, du);
    if(newtondata.getResidualStatus() != AlatEnums::ResidualStatusOk)
    {
      newtondata.getNewtonStatus() = AlatEnums::NewtonStatusLinearNotOk;
      assert(0);
      return;
      // _error_string( "_lineSearchWolfe", "linearization not ok", AlatEnums::residualStatusToString(status_linearization) );
    }
    kappanew = getVisitor()->scalarProduct(r, h);
    notok = ( newtondata.residual*newtondata.residual > newtondata.residual_old*newtondata.residual_old - 0.0001*_omega*kappa )or ( fabs(kappanew) < 0.1*fabs(kappa) );
  }
}

/*--------------------------------------------------------------------------*/

double NonlinearSolver::getRhoMatrix() const
{
  return _rhomatrix;
}
