#include  "Alat/filescanner.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "Fada/linearsolvervectorinterface.h"
#include  "Fada/multigrid.h"
#include  "Alat/preconditionerinterface.h"
#include  "Alat/vectorinterface.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
Multigrid::~Multigrid()
{
  if(_deletemultilevellinearsolver)
  {
    delete _linearsolvervector;
    _linearsolvervector = NULL;
  }
}

Multigrid::Multigrid(std::string dirname) : Alat::IterativeSolverWithVisitor(), _linearsolvervector(NULL), _initialized(false), _deletemultilevellinearsolver(false), _dirname(dirname)
{
  _cycle = 1;
}

Multigrid::Multigrid( const Multigrid& multigrid) : Alat::IterativeSolverWithVisitor(multigrid)
{
  assert(0);
}

Multigrid& Multigrid::operator=( const Multigrid& multigrid)
{
  Alat::IterativeSolverWithVisitor::operator=(multigrid);
  assert(0);
  return *this;
}

std::string Multigrid::getName() const
{
  return "Multigrid";
}

Multigrid* Multigrid::clone() const
{
  assert(0);
//return new Multigrid(*this);
}

std::string Multigrid::getInterfaceName() const
{
  return "Multigrid";
}

/*--------------------------------------------------------------------------*/
std::ostream& Multigrid::printLoopInformation(std::ostream& os) const
{
  IterativeSolverWithVisitor::printLoopInformation(os)<<" ";
  os << " niterpre/niterpost/nitercoarse " <<_niterpre << "/"<<_niterpost << "/" << _nitercoarse<<"\n";
  _linearsolvervector->printLoopInformation(os);
  return os;
}

/*--------------------------------------------------------------------------*/
Fada::Chronometer Multigrid::_chronometer("Multigrid");
void Multigrid::printChronometer(std::ostream& os)
{
  _chronometer.print(os);
}
void Multigrid::initChronometer()
{
  _chronometer.enrol("solvecoarse", true);
  _chronometer.enrol("smooth", true);
  _chronometer.enrol("transfer", true);
  _chronometer.enrol("residual", true);
}

/*--------------------------------------------------------------------------*/
void Multigrid::setsmoothtype(std::string smoothtype)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
int Multigrid::getNVectors() const
{
  return 3;
}

/*--------------------------------------------------------------------------*/
const Fada::VisitorMultigridInterface* Multigrid::getVisitor() const
{
  const Fada::VisitorMultigridInterface* visitor = dynamic_cast<const Fada::VisitorMultigridInterface*>( IterativeSolverWithVisitor::getVisitor() );
  assert(visitor);
  return visitor;
}

Fada::VisitorMultigridInterface* Multigrid::getVisitor()
{
  Fada::VisitorMultigridInterface* visitor = dynamic_cast<Fada::VisitorMultigridInterface*>( IterativeSolverWithVisitor::getVisitor() );
  assert(visitor);
  return visitor;
}

/*--------------------------------------------------------------------------*/
LinearSolverVectorInterface* Multigrid::getLinearSolverVector()
{
  return _linearsolvervector;
}

/*--------------------------------------------------------------------------*/
void Multigrid::setLinearSolverVector(LinearSolverVectorInterface* linearsolvervectorinterface)
{
  _deletemultilevellinearsolver = false;
  _linearsolvervector = linearsolvervectorinterface;
  // for(int level = 0; level < _linearsolvervector->getNLevels(); level++)
  // {
  //   std::cerr << "#### Multigrid::setLinearSolverVector() " << _linearsolvervector->getLinearSolver(level)->getName() << "\n";
  // }
}

/*--------------------------------------------------------------------------*/
void Multigrid::newLinearSolverVector(LinearSolverVectorInterface* linearsolvervectorinterface)
{
  _deletemultilevellinearsolver = true;
  _linearsolvervector = linearsolvervectorinterface;
  // std::cerr << "Multigrid::setLinearSolverVector() " << _linearsolvervector->getName() << "\n";
}

/*--------------------------------------------------------------------------*/
void Multigrid::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  Alat::IterativeSolverWithVisitor::basicInit(parameterfile, blockname);

  int printstepsmoother;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("niterpre", &_niterpre, 2);
  dataformathandler.insert("niterpost", &_niterpost, 2);
  dataformathandler.insert("nitercoarse", &_nitercoarse, 1);
  dataformathandler.insert("printstepsmoother", &printstepsmoother, 0);
  dataformathandler.insert("cycle", &_cycle, 1);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, blockname, 0);
  // std::cerr << "****** MULTIGRID visitor=" << getVisitor()->getName()<<" blockname " << blockname << "\n";
  // std::cerr << "****** MULTIGRID _linearsolvervector->getNLevels() " << _linearsolvervector->getNLevels() << "\n";
  // std::cerr << "****** MULTIGRID printstepsmoother " << printstepsmoother<< "\n";
  // assert(0);
  _maxlevel =  getVisitor()->getMaxLevel();
  _minlevel =  getVisitor()->getMinLevel();
  assert(_minlevel <= _maxlevel);
  std::string id = "*multigrid* " + blockname + " (" + getVisitor()->getName() + ") ";
  getIterationInfo()->setId(id);
  assert(_linearsolvervector);
  if( not _linearsolvervector->allSolversSet() )
  {
    _initialized = false;
    return;
  }
  else
  {
    _initialized = true;
  }
  for(int level = 0; level < _linearsolvervector->getNLevels(); level++)
  {
    Alat::IterationInfo* infosmoother = _linearsolvervector->getLinearSolver(level)->getIterationInfo();
    // std::cerr << "\t??? level="<< level << " infosmoother="<<infosmoother<<"\n";
    if(infosmoother)
    {
      infosmoother->getRTol() = getIterationInfo()->getRTol();
      infosmoother->getGlobalTol() = getIterationInfo()->getGlobalTol();
      infosmoother->getRTol() = 0.0;
      infosmoother->getPrintStep() = printstepsmoother;
      std::stringstream ss;
      std::string namesolver = _linearsolvervector->getLinearSolver(level)->getName();
      std::string namepreconditioner = "none";
      std::string namevisitor = "none";
      Alat::IterativeSolverWithPreconditioner*  iterativesolverwp = dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( _linearsolvervector->getLinearSolver(level) );
      if(iterativesolverwp)
      {
        namepreconditioner = iterativesolverwp->getPreconditioner()->getName();
        namevisitor = iterativesolverwp->getVisitor()->getName();
      }
      ss<<"\t*smooth:prec* "<< level << " " << namesolver << " (" << namevisitor << ") : " << namepreconditioner << " ";
      infosmoother->setId( ss.str() );
    }
  }
  // assert(_linearsolvervector->getNLevels()!=2);
  // for(int level = 0; level < _linearsolvervector->getNLevels(); level++)
  // {
  //   std::cerr << "####1 Multigrid::setLinearSolverVector() " << _linearsolvervector->getLinearSolver(level)->getName() << "\n";
  // }

  // std::cerr << "Multigrid::basicInit() info="<<*getIterationInfo()<<"\n";
  // assert(0);
}

/*--------------------------------------------------------------------------*/
void Multigrid::reInit()
{
  _linearsolvervector->reInit();
}
void Multigrid::compute()
{
  _linearsolvervector->compute();
}
void Multigrid::computePreconditioner()
{
  _linearsolvervector->compute();
}
// void Multigrid::restart()
// {
//   _linearsolvervector->restart();  
// }

/*--------------------------------------------------------------------------*/
void Multigrid::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  solveApproximate(status, _minlevel, A, u, f, iteration);
}
void Multigrid::solveApproximate(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  assert(_initialized);
  Alat::GhostVector& _f = getMemory(0);
  Alat::GhostVector& _u = getMemory(1);
  Alat::GhostVector& _v = getMemory(2);
  getVisitor()->setMultiGridVector(level, _f, f);
  getVisitor()->setMultiGridVector(level, _u, u);
  getVisitor()->vectorZero(level, _u);
  const Alat::IterationInfo& info = *getIterationInfo();
  info.reset();
  step(status, level, level, _maxlevel, A, _u, _f, _v, info);
  getVisitor()->setVector(level, u, _u);
}

/*--------------------------------------------------------------------------*/
void Multigrid::solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  solve(status, _minlevel, A, u, f);
}

/*--------------------------------------------------------------------------*/
void Multigrid::solve(AlatEnums::iterationstatus& status, int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  assert(_initialized);
  const Alat::IterationInfo& info = *getIterationInfo();
  Alat::GhostVector& _f = getMemory(0);
  Alat::GhostVector& _u = getMemory(1);
  Alat::GhostVector& _v = getMemory(2);  
  getVisitor()->setMultiGridVector(level, _f, f);
  getVisitor()->setMultiGridVector(level, _u, u);
  // getVisitor()->vectorZero(level, _u);
  info.reset();
  // getVisitor()->residual(level, A, _v, _u, _f);
  // double res = getVisitor()->vectorNorm(level, _v);
  // std::cerr << "??? Multigrid::solve() res=" << res << "\n";
  // info.checkIteration(status, res);
  // if( ( status == AlatEnums::IterationStatusConverged )or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
  // {
  //   return;
  // }
  // info.getNumberOfIterations()++;
  // std::cerr << " MG " << info << "\n";
  for(int it = 0;; it++)
  {
    double res = step(status, level, level, _maxlevel, A, _u, _f, _v, info);
    info.checkIteration(status, res, it);
    info.getNumberOfIterations()++;
    if( ( status == AlatEnums::IterationStatusConverged )or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
    {
      getVisitor()->setVector(level, u, _u);
      // std::cerr << "Multigrid info = " << *getIterationInfo() << "\n";
		  status = AlatEnums::IterationStatusConverged;
      return;
    }
  }
  status = AlatEnums::IterationStatusDiverged;
}

/*--------------------------------------------------------------------------*/
void Multigrid::smoothcoarse(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const
{
  Alat::IterationInfo* infosmoother = _linearsolvervector->getLinearSolver(level)->getIterationInfo();
  if(infosmoother)
  {
    infosmoother->getMaxiter() = niter;
  }
  getVisitor()->setLevel(level);
  Alat::IterativeSolverWithPreconditioner*  iterativesolverwithpreconditioner =  dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( _linearsolvervector->getLinearSolver(level) );
  if(iterativesolverwithpreconditioner)
  {
    iterativesolverwithpreconditioner->getPreconditioner()->setsmoothtype("coarse");
  }
  // getVisitor()->vectorZero(level, u);
  _linearsolvervector->getLinearSolver(level)->solve(_statussmoother, A, u, f);
}

/*--------------------------------------------------------------------------*/
void Multigrid::smoothpre(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const
{
  // std::cerr << "@@@@ MultiGrid::smoothpre() u="<< u << " f="<< f<< "\n";
  Alat::IterationInfo* infosmoother = _linearsolvervector->getLinearSolver(level)->getIterationInfo();
  if(infosmoother)
  {
    infosmoother->getMaxiter() = niter;
    // std::cerr << "???? Multigrid::smoothpre() info "<< *infosmoother << "\n";
  }
  getVisitor()->setLevel(level);
  Alat::IterativeSolverWithPreconditioner*  iterativesolverwithpreconditioner =  dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( _linearsolvervector->getLinearSolver(level) );
  if(iterativesolverwithpreconditioner)
  {
    iterativesolverwithpreconditioner->getPreconditioner()->setsmoothtype("pre");
  }
  // getVisitor()->vectorZero(level, u);
  _linearsolvervector->getLinearSolver(level)->solve(_statussmoother, A, u, f);
}

/*--------------------------------------------------------------------------*/
void Multigrid::smoothpost(int level, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int niter) const
{
  Alat::IterationInfo* infosmoother = _linearsolvervector->getLinearSolver(level)->getIterationInfo();
  if(infosmoother)
  {
    infosmoother->getMaxiter() = niter;
  }
  getVisitor()->setLevel(level);
  Alat::IterativeSolverWithPreconditioner*  iterativesolverwithpreconditioner =  dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( _linearsolvervector->getLinearSolver(level) );
  if(iterativesolverwithpreconditioner)
  {
    iterativesolverwithpreconditioner->getPreconditioner()->setsmoothtype("post");
  }
  _linearsolvervector->getLinearSolver(level)->solve(_statussmoother, A, u, f);
}

/*--------------------------------------------------------------------------*/
double Multigrid::step(AlatEnums::iterationstatus& status, int level, int minlevel, int maxlevel, const Alat::GhostMatrix& A, Alat::GhostVector& u, Alat::GhostVector& f, Alat::GhostVector& v, const Alat::IterationInfo& info) const
{
	double res;
  getVisitor()->setLevel(level);
  // std::cerr << "@@@@ MultiGrid::step() maxlevel minlevel level " << maxlevel << " " << minlevel << " " << level << " smoother " <<  _linearsolvervector->getLinearSolver(level)->getName() << " u="<< u << " f="<< f<< " v="<< v<< "\n";
  // std::cerr << "@@@@ MultiGrid::step() maxlevel minlevel level " << maxlevel << " " << minlevel << " " << level << " smoother " <<  _linearsolvervector->getLinearSolver(level)->getName() << "\n";
  if(level == maxlevel)
  {
    // std::cerr << "solveCoarse before: f u " << getVisitor()->vectorNorm(level, f) << " " << getVisitor()->vectorNorm(level, u) << "\n";
    _chronometer.start("solvecoarse");
    smoothcoarse(level, A, u, f, _nitercoarse);
    _chronometer.stop("solvecoarse");
    // std::cerr << "solveCoarse after: f u " << getVisitor()->vectorNorm(level, f) << " " << getVisitor()->vectorNorm(level, u) << "\n";

    if(level == minlevel)
    {
      _chronometer.start("residual");
      getVisitor()->vectorZero(level, v);
      getVisitor()->residual(level, A, v, u, f);
      _chronometer.stop("residual");
      // std::cerr << "@@@residual: v u " << getVisitor()->vectorNorm(level, v) << " " << getVisitor()->vectorNorm(level, u) << "\n";
      Alat::IterativeSolverWithVisitor* iterativesolverwv = dynamic_cast<Alat::IterativeSolverWithVisitor*>( _linearsolvervector->getLinearSolver(level) );
      assert(iterativesolverwv);
      // std::cerr << "@@@residual: v u "; iterativesolverwv->getVisitor()->getDomainVector(0,v)->writeAscii(std::cerr); std::cerr << "\n";

      res = getVisitor()->vectorNorm(level, v);
      // double res = getVisitor()->vectorNorm(level, v);
      // info.checkIteration(status, res);
      // // std::cerr << info.getNumberOfIterations() << " " << res << "\n";
      // // assert(0);
      // if( ( status == AlatEnums::IterationStatusConverged )or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
      // {
      //   return;
      // }
      // info.getNumberOfIterations()++;
    }
  }
  else
  {
    // std::cerr << "smooth before pre: f u " << getVisitor()->vectorNorm(level, f) << " " << getVisitor()->vectorNorm(level, u) << "\n";
    _chronometer.start("smooth");
    smoothpre(level, A, u, f, _niterpre);
    _chronometer.stop("smooth");

    _chronometer.start("residual");
    getVisitor()->residual(level, A, v, u, f);
    // getVisitor()->smoothInterfaceOnLevel(level, v);
    // std::cerr << "residual: visistor="<< getVisitor()->getName()<<" v="<< getVisitor()->vectorNorm(level, v) << " u=" << getVisitor()->vectorNorm(level, u) << "\n";
    _chronometer.stop("residual");

    // if(level == minlevel)
    // {
    //   // std::cerr << "residual: v u " << getVisitor()->vectorNorm(level, v) << " " << getVisitor()->vectorNorm(level, u) << "\n";
    //   res = getVisitor()->vectorNorm(level, v);
    //   // double res = getVisitor()->vectorNorm(level, v);
    //   // std::cerr << "info.getNumberOfIterations()="<<info.getNumberOfIterations()<<"\n";
    //   // info.checkIteration(status, res);
    //   // // std::cerr << " MG " << info.getNumberOfIterations() << " done  " << done <<  " res " << res << "\n";
    //   // if( ( status == AlatEnums::IterationStatusConverged )or ( status == AlatEnums::IterationStatusDiverged ) or (status == AlatEnums::IterationStatusMaxIter) )
    //   // {
    //   //   return;
    //   // }
    //   // info.getNumberOfIterations()++;
    // }

    // getVisitor()->vectorZero(level+1, f);
    _chronometer.start("transfer");
    getVisitor()->restrict(level+1, f, v );
    // std::cerr << "restrict: f fine coarse " << getVisitor()->vectorNorm(level, v) << " --> " << getVisitor()->vectorNorm(level+1, f) << "\n";
    // getVisitor()->DirichletZero(level+1, f);
    getVisitor()->vectorZero(level+1, u);
    _chronometer.stop("transfer");

    for(int i = 0; i < _cycle; i++)
    {
      res = step(status, level+1, minlevel, maxlevel, A, u, f, v, info);
      if(status == AlatEnums::IterationStatusProblem)
      {
				assert(0);
        return res;
      }
    }
    // std::cerr << "@@@@@ MG before addUpdate/prolongate u=" << getVisitor()->vectorNorm(level+1, u) << " level=" << level+1 <<  "\n";
    _chronometer.start("transfer");
    getVisitor()->vectorZero(level, v);
    getVisitor()->prolongate(level, v, u);
    getVisitor()->setLevel(level);
    // getVisitor()->vectorAdd(level, u, 1.0, v);
    _linearsolvervector->getLinearSolver(level)->addUpdate(status, v, u);
    _chronometer.stop("transfer");
    // std::cerr << "@@@@@ MG before addUpdate v=" << getVisitor()->vectorNorm(level, v) << " u=" << getVisitor()->vectorNorm(level, u) << " level=" << level <<  "\n";
    if(status == AlatEnums::IterationStatusProblem)
    {
			assert(0);
      return res;
    }
    // getVisitor()->vectorAdd(level, u, 1.0, v);
    _chronometer.start("smooth");
    smoothpost(level, A, u, f, _niterpost);
    _chronometer.stop("smooth");
    if(level == minlevel)
    {
      res = getVisitor()->vectorNorm(level, v);
    }
		return res;
  }
}
