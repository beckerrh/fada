#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "Alat/stringdoublemap.h"
#include  "Alat/tokenize.h"
#include  "Fada/chronometertraits.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/domainsolver.h"
#include  "Fada/integratormanager.h"
#include  "Fada/modelinterface.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/preconditionersolvermanagerallvariables.h"
#include  "Fada/solvermanager.h"
#include  "Fada/timedata.h"
#include  "Fada/variablemanager.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "FadaMesh/meshinterface.h"
#include  "FadaMesh/meshcompositioninterface.h"
#include  "Alat/sorterinterface.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Alat/iterationinfo.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  <cassert>
#include  <cmath>
#include  <fstream>
#include  <iomanip>
#include  <sstream>
#ifdef  CLANG
#include  <algorithm>
#include  <numeric>
#else
#include  <ext/algorithm>
#include  <ext/numeric>
#endif
#include  "Fada/directsolver.h"
#include  "Fada/iterativesolverselector.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
SolverManager::~SolverManager(){}

SolverManager::SolverManager() : Alat::InterfaceBase(), _chronometer("SolverManager"), _time(0.0)
{
  _chronometer.enrol("BasicInit", true);
  _chronometer.enrol("ConstructLinearSolver", true);
  _chronometer.enrol("ConstructMatrix", true);
  _chronometer.enrol("LinearSolve", true);
  _chronometer.enrol("Memory", true);
  _chronometer.enrol("InputOutput", true);
  _chronometer.enrol("PostProcess", true);
  _chronometer.enrol("Form", true);
  _chronometer.enrol("Linearization", true);
  _chronometer.enrol("Rhs", true);
}

SolverManager::SolverManager( const SolverManager& solvermanager) : Alat::InterfaceBase(solvermanager)
{
  assert(0);
}

SolverManager& SolverManager::operator=( const SolverManager& solvermanager)
{
  Alat::InterfaceBase::operator=(solvermanager);
  assert(0);
  return *this;
}

std::string SolverManager::getName() const
{
  return "SolverManager";
}

/*---------------------------------------------------------*/
int SolverManager::getNlevels() const
{
  return _nlevels;
}

/*---------------------------------------------------------*/
void SolverManager::printChronometer(std::string filename) const
{
  std::ofstream file( filename.c_str() );
  _chronometer.print(file);
  printSolverChronometer(file);
  file.close();
}

/*---------------------------------------------------------*/
std::ostream& SolverManager::printLoopInformation(std::ostream& os) const
{
  os << "|>~~~ SolverManager\n";
  os << "|>~~~ \"linearsolver\"=" << _linearsolver << " ";
  os << "|>~~~ LinearSolvers:\n";
  for(GhostLinearSolverAgent::const_iterator p = _ghost_linear_solver_agent.begin(); p != _ghost_linear_solver_agent.end(); p++)
  {
    os << p->first << " ==> \"" << p->second->getName() << "\"\n";
    p->second->printLoopInformation(os);
  }
  os << "\n";
  os << "|>~~~ Matrices n = " << _ghost_matrix_agent.size() << "\n";
  for(GhostMatrixAgent::const_iterator p = _ghost_matrix_agent.begin(); p != _ghost_matrix_agent.end(); p++)
  {
    os << p->first  << "\n";
  }
  os << "|>~~~ Vectors= " << _ghost_vector_agent.size() << "\n";
  // os << "|>~~~ " << _ghost_vector_agent.statistics();
  for(GhostVectorAgent::const_iterator p = _ghost_vector_agent.begin(); p != _ghost_vector_agent.end(); p++)
  {
    os << p->first  << "\n";
  }
  _model->printLoopInformation(os);
  getDomainSolver(0)->printLoopInformation(os);
  return os;
}

/*--------------------------------------------------------------------------*/
void SolverManager::initSolverChronometer() const
{
  Fada::chronometer_traits<Fada::DomainSolver>::init();
}

void SolverManager::printSolverChronometer(std::ostream& os) const
{
  Fada::chronometer_traits<Fada::DomainSolver>::print(os);
}

/*--------------------------------------------------------------------------*/
Fada::DomainSolverInterface* SolverManager::newDomainSolver(const FadaMesh::MeshInterface* mesh) const
{
  return new Fada::DomainSolver;
}

/*---------------------------------------------------------*/
void SolverManager::basicInit(Fada::ModelInterface* model, const FadaMesh::MeshCompositionInterface* meshcomposition, const Alat::IoManager& io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile)
{
  // std::cerr << "SolverManager::basicInit() DEBUT\n";
  initSolverChronometer();
  _chronometer.start("BasicInit");
  _looptype = looptype;
  _model = model;
  assert(meshcomposition);
  _meshcomposition = meshcomposition;
  _parameterfile = parameterfile;
  _io_manager = &io_manager;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("linearsolver", &_linearsolver, "direct");
  dataformathandler.insert("ncellsdirect", &_ncellsdirect, 200);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "SolverManager", 0);
  int ndomains = meshcomposition->getNDomains();
  assert(ndomains==1);
  _nlevels = meshcomposition->getMesh(0)->getNLevels();
  int ncells = 0;
  for(int idomain = 0; idomain < ndomains; idomain++)
  {
    assert(meshcomposition->getMesh(idomain)->getNLevels() == _nlevels);
    ncells += meshcomposition->getMesh(idomain)->getNCells();
  }
  if(ncells < _ncellsdirect)
  {
    _linearsolver = "direct";
  }
  // std::cerr << "@@@@@@@@  SolverManager::basicInit() _nlevels " << _nlevels << "\n";
  for(int i = 0; i < ndomains; i++)
  {
    meshcomposition->getMesh(i)->setResolution(0);
  }
  _domainsolvers.set_size(ndomains);
  for(int i = 0; i < ndomains; i++)
  {
    getDomainSolverPointer(i) = newDomainSolver( getMeshComposition()->getMesh(i) );
    getDomainSolver(i)->defineIntegratorsAndVariables( _model, getMeshComposition()->getMesh(i) );
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->basicInit(_model, getMeshComposition()->getMesh(i), getIoManager(), looptype, getParameterFile() );
  }
  _initDomainsOfVar();
  _ppvarswithoutfem.clear();
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    const VariablesMap& variables = getDomainSolver(i)->getVariableManager()->getPostProcess();
    for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
    {
      const VariableInterface* var = p->second;
      if(var->getFemName() == "none")
      {
        _ppvarswithoutfem[p->first].insert(i);
      }
    }
  }
  _onlydirectsolvers = true;
  if(_linearsolver != "direct")
  {
    Alat::StringVector bouts = Alat::Tokenize(_linearsolver, "|");
    int nbouts = bouts.size();
    if(nbouts <= 1)
    {
      _error_string("basicInit", "in Block SolverManager linearsolver=richardson|U:mg");
    }
    for(int i = 1; i < nbouts; i++)
    {
      Alat::StringVector petitsbouts = Alat::Tokenize(bouts[i], ":");
      int npetitsbouts = petitsbouts.size();
      assert(npetitsbouts == 2);
      if(petitsbouts[1] != "direct")
      {
        _onlydirectsolvers = false;
        break;
      }
    }
  }
  _chronometer.stop("BasicInit");
  // std::cerr << "SolverManager::basicInit() FIN\n";
}

/*--------------------------------------------------------------------------*/
void SolverManager::_initDomainsOfVar()
{
  _variables.clear();
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    const VariableManager* variablemanager = getDomainSolver(i)->getVariableManager();
    const VariablesMap& unknowns = variablemanager->getUnknowns();
    for(VariablesMap::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
    {
      std::string varname = p->second->getVarName();
      // _domainsofvar[varname].insert(i);
      _variables.insert(varname);
    }
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::addLinearDomainSolvers(const Alat::GhostLinearSolver& linearsolver)
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->registerLinearSolver(linearsolver);
  }
  _domainSolversOfSolver.insert(linearsolver);
}

/*--------------------------------------------------------------------------*/
void SolverManager::_reInitSansSolvers()
{
  // std::cerr << "@@@@@@@@  SolverManager::reInit( DEBUT _nlevelsignore " << _nlevelsignore << "\n";
  // std::cerr << "MultiBlockSolverManager::reInit( BEGIN\n";
  _chronometer.start("Memory");

  if(not _onlydirectsolvers)
  {
    Alat::GhostVector vector("umg", "unknowns", -1);
    vector.setDescription("allvars");
    registerVector(vector);
    Alat::GhostVector guold("timesteps_0", "unknowns");
    Alat::GhostVector guveryold("timesteps_1", "unknowns");
    if( _ghost_vector_agent.exists(guold) )
    {
      Alat::GhostVector vector("uoldmg", "unknowns", -1);
      vector.setDescription("allvars");
      registerVector(vector);
    }
    if( _ghost_vector_agent.exists(guveryold) )
    {
      Alat::GhostVector vector("uveryoldmg", "unknowns", -1);
      vector.setDescription("allvars");
      registerVector(vector);
    }
  }

  // We cannot regisrer before in 'registerVector' since the BlockSolvers did not yet exist !
  // So we do it now.
  for(GhostVectorAgent::const_iterator p = _ghost_vector_agent.begin(); p != _ghost_vector_agent.end(); p++)
  {
    // std::cerr << "##### registering vector " <<  p->first.getName() << "\n";
    for(int i = 0; i < getNDomainSolvers(); i++)
    {
      getDomainSolver(i)->registerVector(p->first);
    }
  }
  _chronometer.stop("Memory");
  // std::cerr << "MultiBlockSolverManager::reInit( END\n";
  // std::cerr << "@@@@@@@@  SolverManager::reInit( FIN _nlevelsignore " << _nlevelsignore << "\n";
}

/*--------------------------------------------------------------------------*/
void SolverManager::reInit()
{
  _reInitSansSolvers();
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->reInit();
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::reInitForInterpolation(const FadaMesh::MeshCompositionInterface* meshcomposition)
{
  _reInitSansSolvers();
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->reInitForInterpolation( meshcomposition->getMesh(i) );
  }
}

/*--------------------------------------------------------------------------*/
Alat::LinearSolverInterface* SolverManager::newLinearSolver(const Alat::GhostLinearSolver& ghostlinearsolver)
{
  // std::cerr << "### SolverManager::newLinearSolver("<<"\n";
  std::string linearsolvername = _linearsolver;
  Alat::Map<Alat::StringVector, std::string> solverofvariables;
  if(linearsolvername == "direct")
  {
    linearsolvername = "simple";
    Alat::StringVector variables(_variables);
    solverofvariables[variables] = "direct";
  }
  else
  {
    Alat::StringVector bouts = Alat::Tokenize(_linearsolver, "|");
    int nbouts = bouts.size();
    assert(nbouts > 1);
    linearsolvername = bouts[0];
    for(int i = 1; i < nbouts; i++)
    {
      Alat::StringVector petitsbouts = Alat::Tokenize(bouts[i], ":");
      int npetitsbouts = petitsbouts.size();
      assert(npetitsbouts == 2);
      solverofvariables[Alat::Tokenize(petitsbouts[0], ",")] = petitsbouts[1];
    }
    // std::cerr << "solverofvariables="<<solverofvariables<<"\n";
  }
  Fada::IterativeSolverSelector iterativesolverselector;
  Alat::IterativeSolverWithPreconditioner*  iterativesolverwp = dynamic_cast<Alat::IterativeSolverWithPreconditioner* >( iterativesolverselector.newSolver(linearsolvername) );
  assert(iterativesolverwp);
  int level = 0;
  assert(ghostlinearsolver.getLevel() == level);
  bool integration = false;
  iterativesolverwp->newVisitorPointer() = new Fada::VisitorSolverManagerOneLevel(this, level, _variables, integration);
  iterativesolverwp->newPreconditionerPointer() = new PreconditionerSolverManagerAllvariables(solverofvariables, ghostlinearsolver.getMatrix(), this);
  iterativesolverwp->basicInit( getParameterFile(), "SolverManager" );

  return iterativesolverwp;
}

/*--------------------------------------------------------------------------*/
void SolverManager::reInitMatrixAndLinearSolver()
{
  if(_looptype != FadaEnums::StaticLoop and _looptype != FadaEnums::DynamicLoop)
  {
    return;
  }
  // std::cerr << "MultiBlockSolverManager::reInit( BEGIN\n";
  _chronometer.start("Memory");
  for(GhostLinearSolverAgent::iterator p = _ghost_linear_solver_agent.begin(); p != _ghost_linear_solver_agent.end(); p++)
  {
    p->second = newLinearSolver(p->first);
  }
  for(GhostMatrixAgent::const_iterator p = _ghost_matrix_agent.begin(); p != _ghost_matrix_agent.end(); p++)
  {
    for(int i = 0; i < getNDomainSolvers(); i++)
    {
      getDomainSolver(i)->registerMatrix(p->first);
    }
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->reInitMatrices();
  }
  for(GhostLinearSolverAgent::iterator p = _ghost_linear_solver_agent.begin(); p != _ghost_linear_solver_agent.end(); p++)
  {
    p->second->reInit();
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->reInitLinearSolvers();
  }
  _chronometer.stop("Memory");
}

/*--------------------------------------------------------------------------*/
void SolverManager::reInitVectorForInterpolation(Alat::GhostVector& u) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->reInitVectorForInterpolation(u);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::restartLinearSolver(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& gu)
{
  constructJacobianMatrix(status, A, gu);
  constructLinearSolver(linearsolver, gu);
}

/*--------------------------------------------------------------------------*/
void SolverManager::constructLinearSolver(Alat::GhostLinearSolver& linearsolver, const Alat::GhostVector& u)
{
  _chronometer.start("ConstructLinearSolver");
  getLinearSolver(linearsolver)->compute();
  getLinearSolver(linearsolver)->restart();
  {
    for(int i = 0; i < getNDomainSolvers(); i++)
    {
      getDomainSolver(i)->constructMultigridTransfer( linearsolver.getMatrix() );
      for(GhostLinearSolverSet::iterator p = _domainSolversOfSolver.begin(); p != _domainSolversOfSolver.end(); p++)
      {
        getDomainSolver(i)->constructLinearSolver(*p);
      }
    }
  }
  _chronometer.stop("ConstructLinearSolver");
}

// /*--------------------------------------------------------------------------*/
// void SolverManager::matrixVectorProductCoupling(int i, int level, const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const
// {
//   return;
//   assert(0);
// //   for(int pos = _domainneighbours.rowstart(i); pos < _domainneighbours.rowstop(i); pos++)
// //   {
// //     int j = _domainneighbours.col(pos);
// //     if(i == j)
// //     {
// //       continue;
// //     }
// // // #ifdef  CLANG
// // //     std::pair<int, int> domains = std::make_pair<int, int>( std::min(i, j), fmax(i, j) );
// // // #else
// // //     std::pair<int, int> domains = std::make_pair( std::min(i, j), fmax(i, j) );
// // // #endif
// //     Alat::IntPair domains = Alat::makePair( std::min(i, j), std::max(i, j) );
// //     int ic = _couplingofdomains[domains];
// //     int il = _domainsofcoupling[ic][0];
// //     if(il == i)
// //     {
// //       getCouplingSolver(ic)->matrixVectorProductLeft(level, A, y, x, d);
// //     }
// //     else
// //     {
// //       getCouplingSolver(ic)->matrixVectorProductRight(level, A, y, x, d);
// //     }
// //   }
// }

/*--------------------------------------------------------------------------*/
void SolverManager::setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setVariableVectorToAll(u, ui);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setVariableVectorFromAll(ui, u);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::matrixVectorProduct(const Alat::GhostMatrix& A, Alat::GhostVector& y, const Alat::GhostVector& x, double d) const
{
  assert(0);
  // std::cerr << "SolverManager::matrixVectorProduct() y="<<y<<" x="<<x<<"\n";
  int level = x.getLevel();
  assert(y.getLevel() == level);
  assert( level == A.getLevel() );
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->matrixVectorProduct(level, A, y, x, d);
  }
}

/*---------------------------------------------------------*/
void SolverManager::readUnknownVariables(Alat::GhostVector& v, int number)
{
  _chronometer.start("InputOutput");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->readUnknownVariables(v, number);
  }
  _time = getDomainSolver(0)->getTime();
  _deltat = getDomainSolver(0)->getDeltaT();
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/

void SolverManager::writeVariablesInfo() const
{
  _chronometer.start("InputOutput");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->writeVariablesInfo();
  }
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/

void SolverManager::setLavrentievParameter(double parameter) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setLavrentievParameter(parameter);
  }
}

/*---------------------------------------------------------*/
void SolverManager::writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& gv, int number) const
{
  _chronometer.start("InputOutput");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->writeUnknownVariables(help, gv, number);
  }
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/
void SolverManager::writePostProcessVariables(const Alat::GhostVector& v, int number) const
{
  _chronometer.start("InputOutput");
  Alat::DoubleVector est;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->writePostProcessVariables(est, v, number);
  }

  Alat::SystemVector vectorallvars;
  computePostProcessVector(vectorallvars, v);

  int n = vectorallvars.size()+est.n_elem;
  arma::Col<double> vec(n);
  Alat::StringVector names(n);

  // std::cerr << "SolverManager::writePostProcessVariables() " << est[0] << "\n";

  for(int i=0;i<est.n_elem;i++)
  {
    vec[i] = est[i];
    std::stringstream ss;
    ss << i;
    names[i] = "estimator"+ ss.str();
  }
  int i=est.n_elem;
  for(Alat::SystemVector::iterator p = vectorallvars.begin(); p != vectorallvars.end(); p++)
  {
    const Alat::VariableVector* varvector = dynamic_cast<const Alat::VariableVector*>( p->second );
    // std::cerr << "?????? " << p->first << " " << *varvector << "\n";
    vec[i] = (*varvector)(0,0);
    names[i] = p->first;
    i++;
  }


  std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::PostProcess, "ScalarsStatic.data");
  std::ofstream file( filename.c_str() );

  file << "# N " << names << "\n";
  file << getMeshComposition()->getNCells() << " ";
  file.precision(12);
  file.setf(arma::ios::scientific);
  vec.t().raw_print(file);
  file.close();


  // _writePostProcessScalarsHeader(_ppvarswithoutfem, file, "N");
  // _writePostProcessScalars(_ppvarswithoutfem, vectorallvars, file, "N");
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/
void SolverManager::_writePostProcessScalarsHeader(const Alat::Map<std::string, Alat::IntSet>& ppvarswithoutfem, std::ofstream& file, std::string first_column) const
{
  file << "# " <<first_column;
  for(Alat::Map<std::string, Alat::IntSet>::const_iterator p = _ppvarswithoutfem.begin(); p != _ppvarswithoutfem.end(); p++)
  {
    std::string varname = p->first;
    int idomain = *( p->second.begin() );
    const VariableManager* variablemanager = getDomainSolver(idomain)->getVariableManager();
    const VariableInterface* var = variablemanager->getPostProcess(varname);
    int icomp = 0;
    std::string type =  var->getPostProcType(icomp);
    if(type == "sep")
    {
      int ncomp = var->getNComponents();
      for(int icomp = 1; icomp < ncomp; icomp++)
      {
        assert(var->getPostProcType(icomp) == "sep");
      }
      for(Alat::IntSet::const_iterator pp = p->second.begin(); pp != p->second.end(); pp++)
      {
        int idomain = *pp;
        std::stringstream name;
        name <<p->first<< "_" << idomain;
        file << " "<<name.str();
      }
    }
    else
    {
      int ncomp = var->getNComponents();
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        // file << " " << p->first<<icomp;
        file << " " << var->getVisualizationName(icomp);
      }
    }
  }

  file << "\n";
}

/*---------------------------------------------------------*/
void SolverManager::_writePostProcessScalars(const Alat::Map<std::string, Alat::IntSet>& ppvarswithoutfem, const Alat::SystemVector& vectorallvars, std::ofstream& file, std::string first_column) const
{
  if(first_column == "t")
  {
    file << " " << std::setiosflags(std::ios::scientific) << std::setprecision(12) << getTime();
  }
  else if(first_column == "N")
  {
    file << " " << getMeshComposition()->getNCells();
  }
  else
  {
    assert(0);
  }


  for(Alat::Map<std::string, Alat::IntSet>::const_iterator p = _ppvarswithoutfem.begin(); p != _ppvarswithoutfem.end(); p++)
  {
    std::string varname = p->first;
    int idomain = *( p->second.begin() );
    const VariableManager* variablemanager = getDomainSolver(idomain)->getVariableManager();
    const VariableInterface* var = variablemanager->getPostProcess(varname);
    int icomp = 0;
    std::string type =  var->getPostProcType(icomp);
    // std::cerr << "@@@@@@@@@@@@@@@ name=" << p->first<<"\n";
    if(type == "sep")
    {
      int ncomp = var->getNComponents();
      for(int icomp = 1; icomp < ncomp; icomp++)
      {
        assert(var->getPostProcType(icomp) == "sep");
      }
      for(Alat::IntSet::const_iterator pp = p->second.begin(); pp != p->second.end(); pp++)
      {
        int idomain = *pp;
        std::stringstream name;
        name <<p->first<< "_" << idomain;
        varname = name.str();
        const Alat::VariableVector* varvector = dynamic_cast<const Alat::VariableVector*>( vectorallvars[varname] );
        for(int icomp = 0; icomp < varvector->getNComponents(); icomp++)
        {
          for(Alat::VariableVector::const_iterator pv = varvector->begin(icomp); pv != varvector->end(icomp); pv++)
          {
            file << " " << std::setiosflags(std::ios::scientific) << std::setprecision(12) << *pv;
          }
        }
      }
    }
    else
    {
      const Alat::VariableVector* varvector = dynamic_cast<const Alat::VariableVector*>( vectorallvars[varname] );
      for(int icomp = 0; icomp < varvector->getNComponents(); icomp++)
      {
        for(Alat::VariableVector::const_iterator pv = varvector->begin(icomp); pv != varvector->end(icomp); pv++)
        {
          file << " " << std::setiosflags(std::ios::scientific) << std::setprecision(12) << *pv;
        }
      }
    }
  }
  file << "\n";
}

/*---------------------------------------------------------*/
void SolverManager::writePostProcessVariablesDynamic(const Alat::GhostVector& v, int number) const
{
  _chronometer.start("InputOutput");
  std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::PostProcess, "ScalarsDynamic.data");
  std::ofstream file;
  if(number == 0)
  {
    file.open( filename.c_str() );
    _writePostProcessScalarsHeader(_ppvarswithoutfem, file);
    _chronometer.stop("InputOutput");
    return;
  }
  else
  {
    file.open( filename.c_str(), std::ios::app);
  }

  Alat::SystemVector vectorallvars;
  computePostProcessVector(vectorallvars, v);

  _writePostProcessScalars(_ppvarswithoutfem, vectorallvars, file);
  file.close();
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/
void SolverManager::computePostProcessVector(Alat::SystemVector& vectorallvars, const Alat::GhostVector& v) const
{
  Alat::StringSet ppvarnames;
  for(Alat::Map<std::string, Alat::IntSet>::const_iterator p = _ppvarswithoutfem.begin(); p != _ppvarswithoutfem.end(); p++)
  {
    std::string varname = p->first;
    int idomain = *( p->second.begin() );
    const VariableManager* variablemanager = getDomainSolver(idomain)->getVariableManager();
    const VariableInterface* var = variablemanager->getPostProcess(varname);
    int icomp = 0;
    std::string type =  var->getPostProcType(icomp);

    if(type == "sep")
    {
      int ncomp = var->getNComponents();
      for(int icomp = 1; icomp < ncomp; icomp++)
      {
        assert(var->getPostProcType(icomp) == "sep");
      }
      for(Alat::IntSet::const_iterator pp = p->second.begin(); pp != p->second.end(); pp++)
      {
        int idomain = *pp;
        std::stringstream name;
        name <<p->first<< "_" << idomain;
        ppvarnames.insert( name.str() );
      }
    }
    else
    {
      ppvarnames.insert(p->first);
    }
  }

  vectorallvars.set_size(ppvarnames);
  for(Alat::SystemVector::iterator p = vectorallvars.begin(); p != vectorallvars.end(); p++)
  {
    p->second = new Alat::VariableVector;
  }

  for(Alat::Map<std::string, Alat::IntSet>::const_iterator p = _ppvarswithoutfem.begin(); p != _ppvarswithoutfem.end(); p++)
  {
    Alat::VariableVector* varvectorpointer;
    std::string varname;
    bool first = true;
    int ncomp;
    int nforvar;
    const VariableInterface* var;
    for(Alat::IntSet::const_iterator pp = p->second.begin(); pp != p->second.end(); pp++)
    {
      varname = p->first;
      int idomain = *pp;
      const VariableManager* variablemanager = getDomainSolver(idomain)->getVariableManager();
      var = variablemanager->getPostProcess(varname);
      // int iblock = variablemanager->getIndexOfPostProcessVariable(varname);
      ncomp = var->getNComponents();
      nforvar = var->getN();
      std::string type =  var->getPostProcType(0);

      if(type == "sep")
      {
        std::stringstream name;
        name << varname<<"_" << idomain;
        varname = name.str();
        first = true;
      }

      varvectorpointer = dynamic_cast<Alat::VariableVector*>( vectorallvars[varname] );
      Alat::VariableVector& varvector  = *varvectorpointer;

      if(first)
      {
        nforvar = var->getN();
        varvector.set_size( ncomp, nforvar);
        varvector.zeros();
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          std::string type =  var->getPostProcType(icomp);
          if(type == "max")
          {
            varvector(icomp, 0) = std::numeric_limits<double>::lowest();
          }
          else if(type == "min")
          {
            varvector(icomp, 0) = std::numeric_limits<double>::max();
          }
        }
        first = false;
      }
      else
      {
        assert(varvector.getNComponents() == ncomp);
        assert(varvector.n() == nforvar);
      }


      const Alat::SystemVector* vav = dynamic_cast<const Alat::SystemVector*>( getDomainSolver(idomain)->getVector(v) );
      if(not vav)
      {
        const Fada::MultiLevelVector* vmg = dynamic_cast<const Fada::MultiLevelVector*>( getDomainSolver(idomain)->getVector(v) );
        int level = 0;
        vav = dynamic_cast<const Alat::SystemVector*>( vmg->getVector(level) );
      }
      assert(vav);
      if(vav->size() == 0)
      {
        assert(0);
        continue;
      }
      const Alat::VariableVector* vv = dynamic_cast<const Alat::VariableVector*>( vav->getVector(p->first) );
      assert(vv);
      assert(vv->getNComponents() == ncomp);
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        std::string type =  var->getPostProcType(icomp);
        if( ( type == "L2" ) || ( type == "H1" ) || ( type == "L1" ) || ( type == "W1" ) )
        {
          varvector(icomp, 0) += ( *vv )( icomp, 0 );
        }
        else if(type == "C0")
        {
          varvector(icomp, 0) = fmax( varvector(icomp, 0), ( *vv )( icomp, 0 ) );
        }
        else if(type == "OS")
        {
          varvector(icomp, 0) = ( *vv )( icomp, 0 );
          // varvector(icomp, 0) = fmax( varvector(icomp, 0), ( *vv )( icomp, 0 ) );
          // std::cerr << " @@ " << ( *vv )( icomp, 0 ) << " ;;;;; " << varvector(icomp, 0) << "\n";
        }
        else if(type == "US")
        {
          varvector(icomp, 0) = ( *vv )( icomp, 0 );
          // varvector(icomp, 0) = fmin( varvector(icomp, 0), ( *vv )( icomp, 0 ) );
          // std::cerr << " ## " << ( *vv )( icomp, 0 ) << " ;;;;; " << varvector(icomp, 0) << "\n";
        }
        else if(type == "sum")
        {
          assert(nforvar == 1);
          for(int i = 0; i < nforvar; i++)
          {
            varvector(icomp, i) += ( *vv )( icomp, i );
          }
        }
        else if(type == "sep")
        {
          for(int i = 0; i < nforvar; i++)
          {
            varvector(icomp, i) =  ( *vv )( icomp, i );
          }
        }
        else
        {
          assert(0);
        }
      }
    }
    Alat::VariableVector& varvector  = *varvectorpointer;
    for(int icomp = 0; icomp < ncomp; icomp++)
    {
      std::string type =  var->getPostProcType(icomp);
      if( ( type == "L2" ) || ( type == "H1" ) )
      {
        varvector(icomp, 0) = sqrt( varvector(icomp, 0) );
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
std::ostream& SolverManager::vectorWrite(std::ostream& os, const Alat::GhostVector& r) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    os << "Domain "<< i << "\n";
    getDomainSolver(i)->vectorWrite(os, r);
  }
  return os;
}

/*--------------------------------------------------------------------------*/
void SolverManager::setVectorFromAllVariables( Alat::GhostVector& gu, const Alat::GhostVector& guall) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setVectorFromAllVariables(gu, guall);
  }
}

void SolverManager::setVectorToAllVariables( Alat::GhostVector& guall, const Alat::GhostVector& gu) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setVectorToAllVariables(guall, gu);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::vectorZero(Alat::GhostVector& f) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->vectorZero(f);
  }
}

/*--------------------------------------------------------------------------*/

void SolverManager::vectorAdd(Alat::GhostVector& y, double s, const Alat::GhostVector& x) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->vectorAdd(y, s, x);
  }
}

/*--------------------------------------------------------------------------*/

void SolverManager::vectorEqual(Alat::GhostVector& y, const Alat::GhostVector& x) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->vectorEqual(y, x);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::vectorNormPerVariable(Alat::StringDoubleMap& rnorm, const Alat::GhostVector& r) const
{
  _initStringDoubleMapForVectors(rnorm);
  int level = 0;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->vectorScalarProductPerVariables(level, rnorm, r, r);
  }
  for(Alat::StringDoubleMap::iterator p = rnorm.begin(); p != rnorm.end(); p++)
  {
    p->second = sqrt(p->second);
  }
}

/*--------------------------------------------------------------------------*/
double SolverManager::vectorNorm(const Alat::GhostVector& y) const
{
  return sqrt( scalarProduct(y, y) );
}

/*--------------------------------------------------------------------------*/
double SolverManager::scalarProduct(const Alat::GhostVector& x, const Alat::GhostVector& y) const
{
  double ps = 0.0;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    ps += getDomainSolver(i)->vectorScalarProduct(x, y);
  }
  return ps;
}

/*--------------------------------------------------------------------------*/

void SolverManager::matrixZero(Alat::GhostMatrix& A) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->matrixZero(A);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& f) const
{
  _chronometer.start("Rhs");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->constructRightHandSide(status, f);
    getDomainSolver(i)->strongDirichletVectorSolution(f);
  }
  _chronometer.stop("Rhs");
}

/*--------------------------------------------------------------------------*/
void SolverManager::constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const
{
  _chronometer.start("Form");
  status = AlatEnums::ResidualStatusOk;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->constructForm(status, f, u);
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->strongDirichletVectorZero(f);
  }
  _chronometer.stop("Form");
}

/*--------------------------------------------------------------------------*/
void SolverManager::computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  _chronometer.start("Linearization");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->computeLinearization(status, f, u, du);
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->strongDirichletVectorZero(f);
  }
  _chronometer.stop("Linearization");
}

/*--------------------------------------------------------------------------*/
void SolverManager::integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const
{
  // std::cerr << "SolverManager::integrateTimeRhs() d="<<d<<"\n";
  _chronometer.start("Rhs");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->integrateTimeRhs(status, f, u, bdf, d);
  }
  _chronometer.stop("Rhs");
}

/*--------------------------------------------------------------------------*/
void SolverManager::initSolution(Alat::GhostVector& u)
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->initSolution(u);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, const Alat::GhostVector& u)
{
  // std::cerr << "SolverManager::constructJacobianMatrix()\n";
  _chronometer.start("ConstructMatrix");
  double tstart, tend, tdiff;
  {
    for(int i = 0; i < getNDomainSolvers(); i++)
    {
      getDomainSolver(i)->constructJacobianMatrix(status, A, u);
    }
  }
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->strongDirichletMatrix(A);
  }
  _chronometer.stop("ConstructMatrix");
}

/*--------------------------------------------------------------------------*/
void SolverManager::postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level) const
{
  _chronometer.start("PostProcess");
  vectorZero(f);
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->postProcess(status, f, u, uold, level);
  }
  _chronometer.stop("PostProcess");
}

/*--------------------------------------------------------------------------*/
void SolverManager::integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const
{
  _chronometer.start("PostProcess");
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->integrateInTimePostProcess(ptime, p);
  }
  _chronometer.stop("PostProcess");
}

/*---------------------------------------------------------*/
void SolverManager::setMeshDecomposition(const FadaMesh::MeshCompositionInterface* meshcomposition)
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setMesh( meshcomposition->getMesh(i) );
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->interpolateSolution(ufine, ucoarse);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::strongDirichletVectorSolution(Alat::GhostVector& u) const
{
  int level = 0;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->strongDirichletVectorSolution(u);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::newtonProject(Alat::GhostVector& gu) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->newtonProject(gu);
  }
}

/*--------------------------------------------------------------------------*/
void SolverManager::beforeTimestep(Alat::GhostVector& gu) const
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->beforeTimestep(gu);
  }
}

/*---------------------------------------------------------*/
void SolverManager::_initStringDoubleMapForVectors(Alat::StringDoubleMap& map) const
{
  map.clear();
  for(Alat::StringSet::const_iterator p = _variables.begin(); p != _variables.end(); p++)
  {
    map[*p] = 0.0;
  }
}

/*---------------------------------------------------------*/
void SolverManager::computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  norms.zeros();
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->computeNormSquared(status, norms, u, du);
  }
}

/*---------------------------------------------------------*/
double SolverManager::computeTimeEstimator(const Alat::GhostVector& u, const Alat::GhostVector& prev, Alat::GhostVector& h1, Alat::GhostVector& h2) const
{
  int level = 0;
  Alat::StringDoubleMap delta_norm(_variables), norm(_variables);
  AlatEnums::residualstatus status;
  vectorEqual( h1, u );
  vectorAdd( h1, -1.0, prev );
  // std::cerr << "SolverManager::computeTimeEstimator() u="<<vectorNorm(u) <<"\n";
  // std::cerr << "SolverManager::computeTimeEstimator() prev="<<vectorNorm(prev) <<"\n";
  // std::cerr << "SolverManager::computeTimeEstimator() h1="<<vectorNorm(h1) <<"\n";
  computeNormSquared(status, delta_norm, u, h1);
  computeNormSquared(status, norm, u, u);

  double max = 0.0;
  for(Alat::StringDoubleMap::iterator p = delta_norm.begin(); p != delta_norm.end(); p++)
  {
    max = fmax( max, sqrt(p->second)/sqrt(norm[p->first]) );
  }
  return max;
}

/*---------------------------------------------------------*/
const Alat::IoManager* SolverManager::getIoManager() const
{
  assert(_io_manager);
  return _io_manager;
}

const Alat::ParameterFile* SolverManager::getParameterFile() const
{
  return _parameterfile;
}

const FadaMesh::MeshCompositionInterface* SolverManager::getMeshComposition() const
{
  return _meshcomposition;
}

const Alat::LinearSolverInterface* SolverManager::getLinearSolver(const Alat::GhostLinearSolver& v) const
{
  return _ghost_linear_solver_agent(v);
}

Alat::LinearSolverInterface* SolverManager::getLinearSolver(const Alat::GhostLinearSolver& v)
{
  return _ghost_linear_solver_agent(v);
}

DomainSolverInterface*& SolverManager::getDomainSolverPointer(int i)
{
  assert( i < _domainsolvers.size() );
  return _domainsolvers[i];
}

DomainSolverInterface* SolverManager::getDomainSolver(int i)
{
  assert( i < _domainsolvers.size() );
  return _domainsolvers[i];
}

const DomainSolverInterface* SolverManager::getDomainSolver(int i) const
{
  assert( i < _domainsolvers.size() );
  return _domainsolvers[i];
}

int SolverManager::getNDomainSolvers() const
{
  return _domainsolvers.size();
}

void SolverManager::registerVector(const Alat::GhostVector& v)
{
  _ghost_vector_agent.enrol(v);
}

void SolverManager::registerVectorIfNotExists(const Alat::GhostVector& v)
{
  if( _ghost_vector_agent.exists(v) )
  {
    return;
  }
  _ghost_vector_agent.enrol(v);
}

void SolverManager::registerMatrix(Alat::GhostMatrix& matrix)
{
  if(_onlydirectsolvers)
  {
    assert(matrix.getLevel() == 0);
  }
  if(matrix.getLevel() == -1)
  {
    assert(not _onlydirectsolvers);
  }
  if(not _onlydirectsolvers)
  {
    matrix.setLevel(-1);
  }
  _ghost_matrix_agent.enrol(matrix);
}

void SolverManager::registerLinearSolver(const Alat::GhostLinearSolver& v)
{
  _ghost_linear_solver_agent.enrol(v);
}

double SolverManager::getTime() const
{
  return _time;
}

double SolverManager::getDeltaT() const
{
  return _deltat;
}

void SolverManager::setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit)
{
  // std::cerr << "SolverManager::setTimeInfo() time="<<time<<" deltat="<<deltat<<" deltatold="<<deltatold<<" masscoefimplicit="<<masscoefimplicit<<"\n";
  _time = timedata.time;
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setTimeInfo(timedata, masscoefimplicit);
  }
}

void SolverManager::setTimeScheme(std::string time_discretization)
{
  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->setTimeScheme(time_discretization);
  }
}

/*--------------------------------------------------------------------------*/
int SolverManager::linearSolve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, const Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& u, const Alat::GhostVector& f) const
{
  _chronometer.start("LinearSolve");

  // std::cerr << " SolverManager::linearSolve() " << *getLinearSolver(linearsolver)->getIterationInfo() << "\n";
  getLinearSolver(linearsolver)->solve(status, A, u, f);

  for(int i = 0; i < getNDomainSolvers(); i++)
  {
    getDomainSolver(i)->getMesh()->setResolution(0);
  }
  _chronometer.stop("LinearSolve");
  // std::cerr << "SolverManager::linearSolve() linearsolver="<<getLinearSolver(linearsolver)->getName()<<"\n";
  return getLinearSolver(linearsolver)->getNumberOfIterations();
}
