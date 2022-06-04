#include  "Alat/assemblevector.h"
#include  "Alat/dataformathandler.h"
#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "Alat/stringdoublemap.h"
#include  "Alat/tokenize.h"
#include  "Fada/applicationinterface.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/dofinformation.h"
#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofinformationwithoutfem.h"
#include  "Fada/domainintegrationloopinterface.h"
#include  "Fada/modelinterface.h"
#include  "Fada/domainsolver.h"
#include  "Fada/feminterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/femmanagerinterface.h"
#include  "Fada/integrationloopinterface.h"
#include  "Fada/iterativesolverselector.h"
#include  "Fada/multilevellinearsolver.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelsolverinterface.h"
#include  "Fada/multileveltransferallvariables.h"
#include  "Fada/multileveltransferallvariablesinterface.h"
#include  "Fada/multileveltransfercgp12d.h"
#include  "Fada/multileveltransferfromtransferconstructor.h"
#include  "Fada/multileveltransfersinglewithoutfem.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/preconditionerselector.h"
#include  "Fada/problemdata.h"
#include  "Fada/timedata.h"
#include  "Fada/unstructuredintegrationloop.h"
#include  "Fada/unstructuredmultilevelmatrixtransfer.h"
#include  "FadaMesh/boundaryinfo.h"
#include  "FadaMesh/meshinterface.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/quadtotri.h"
#include  "FadaMesh/refineinfo.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/variablediagonalmatrix.h"
#include  "Alat/variablematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  <cassert>
#include  <cmath>
#include  <fstream>
#include  <sstream>
#ifdef  CLANG
#include  <algorithm>
#include  <numeric>
#else
#include  <ext/algorithm>
#include  <ext/numeric>
#endif

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainSolver::~DomainSolver()
{
  for(GhostMatrixAgent::iterator p = _ghost_matrix_agent.begin(); p != _ghost_matrix_agent.end(); p++)
  {
    if(p->second)
    {
      delete p->second;
      p->second = NULL;
    }
  }
  for(GhostVectorAgent::iterator p = _ghost_vector_agent.begin(); p != _ghost_vector_agent.end(); p++)
  {
    if(p->second)
    {
      delete p->second;
      p->second = NULL;
    }
  }
  for(GhostLinearSolverAgent::iterator p = _ghost_linear_solver_agent.begin(); p != _ghost_linear_solver_agent.end(); p++)
  {
    if(p->second)
    {
      delete p->second;
      p->second = NULL;
    }
  }
}

DomainSolver::DomainSolver() : Solver(), DomainSolverInterface(), _model(NULL), _domainintegrationloop(NULL), _femmanagerR(NULL), _varscaleafterinitialize(false)
{}
DomainSolver::DomainSolver( const DomainSolver& domainsolver) : Solver(domainsolver), DomainSolverInterface(domainsolver)
{
  assert(0);
}

DomainSolver& DomainSolver::operator=( const DomainSolver& domainsolver)
{
  assert(0);
  return *this;
}

std::string DomainSolver::getName() const
{
  return "DomainSolver";
}

DomainSolver* DomainSolver::clone() const
{
  assert(0);
//   return new DomainSolver(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& DomainSolver::printLoopInformation(std::ostream& os) const
{
  os << "\t|>======DomainSolver Model="<<_model->getName()<<"======<|\n";
  _integratormanager.printLoopInformation(os);

  // _ghost_linear_solver_agent.printLoopInformation(os);
  // _ghost_matrix_agent.printLoopInformation(os);
  // _ghost_vector_agent.printLoopInformation(os);
  // os << _ghost_vector_agent.statistics();

  getBoundaryManager()->writeBoundaryConditions(os);

  // _variablemanager.printLoopInformation(os);

  _model->printLoopInformation(os);
  return os;
}

/*-------------------------------------------------------------------*/
Fada::Chronometer DomainSolver::_chronometer("DomainSolver");
void DomainSolver::initChronometer()
{
  _chronometer.enrol("basicInit", true);
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

void DomainSolver::printChronometer(std::ostream& os)
{
  // _chronometer.print(std::cerr);
  _chronometer.print(os);
}

/*--------------------------------------------------------------------------*/
Fada::DomainIntegrationLoopInterface* DomainSolver::newDomainIntegrationLoop() const
{
  return new Fada::UnStructuredIntegrationLoop();
}

/*--------------------------------------------------------------------------*/
Fada::DofInformationInterface* DomainSolver::newDofInformationWithFem(const Fada::FemInterface* fem) const
{
  return new Fada::DofInformation;
}

/*--------------------------------------------------------------------------*/
Fada::FemManagerInterface* DomainSolver::newFemManager() const
{
  return new Fada::FemManager;
}

/*--------------------------------------------------------------------------*/
int DomainSolver::getNcompOfVar(std::string varname) const
{
  return _ncompOfVars[varname];
}

int DomainSolver::getNOfVar(int level, std::string varname) const
{
  return _nOfVars[level][varname];
}

const Alat::StringDoubleVectorMap& DomainSolver::getVariablesScales() const
{
  return getModel()->getVariablesScales();
}

/*--------------------------------------------------------------------------*/
const DomainIntegrationLoopInterface* DomainSolver::getDomainIntegrationLoop() const
{
  return _domainintegrationloop;
}

DomainIntegrationLoopInterface* DomainSolver::getDomainIntegrationLoop()
{
  return _domainintegrationloop;
}

Fada::DofInformationInterface* DomainSolver::newDofInformation(const Fada::FemInterface* fem) const
{
  if(not fem)
  {
    return new DofInformationWithoutFem;
  }
  else
  {
    return newDofInformationWithFem(fem);
  }
}

void DomainSolver::addPostProcessVariable(Fada::VariableInterface* variable)
{
  _variablemanager.addAdditionalPostProcessVariable(variable);
}

/*---------------------------------------------------------*/
void DomainSolver::defineIntegratorsAndVariables(Fada::ModelInterface* model, const FadaMesh::MeshInterface* mesh)
{
  _model = model;
  getModel()->defineVariables( getVariableManager() );
  Fada::ProblemData* problemdata = getModel()->getProblemData();
  if( getModel()->getApplication()->hasExactSolutions() )
  {
    const VariablesMap& unknowns = getVariableManager()->getUnknowns();
    for(VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
    {
      std::string varname = p->first;
      std::string varname_exact = varname + "_exact";
      int ncomp = getVariableManager()->getUnknown(varname)->getNComponents();
      problemdata->setData(varname_exact, "ExactSolution") = new DataFunctionVector(ncomp);
      Fada::DataFunctionVector* uex = dynamic_cast<Fada::DataFunctionVector*>( problemdata->getData(varname_exact, "ExactSolution" ) );
      assert(uex);
    }
  }
  getModel()->getApplication()->defineProblemData( problemdata );
  getModel()->defineParametersForProblemData();
  getModel()->getProblemData()->basicInit();

  getModel()->defineIntegrators( getIntegratorManager() );
  getModel()->getApplication()->defineIntegrators( getIntegratorManager() );

  getVariableManager()->defineDataAndPostProcessVariables( getIntegratorManager(), getModel(), mesh, this);
}

/*--------------------------------------------------------------------------*/
Fada::MultiLevelTransferAllVariablesInterface* DomainSolver::newMultiLevelTransferAllVariables()
{
  if(_mgtransfer == "matrix")
  {
    return new Fada::UnstructuredMultiLevelMatrixTransfer();
  }
  else
  {
    const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( getMesh() );
    if(not mlmesh)
    {
      return NULL;
    }
    return new MultiLevelTransferAllVariables;
  }
}

/*--------------------------------------------------------------------------*/
Fada::MultiLevelTransferSingleFemInterface* DomainSolver::newMultiLevelTransferSingleFem(const Fada::FemInterface* fem) const
{
  if(fem->getName() == "FemP12d")
  {
    return new Fada::MultiLevelTransferCgP12d();
  }
  else if(fem->getName() != "none")
  {
    return new Fada::MultiLevelTransferFromTransferConstructor();
  }
  else
  {
    return new MultiLevelTransferSingleWithoutFem();
  }
}

/*---------------------------------------------------------*/
void DomainSolver::basicInit(Fada::ModelInterface* model, const FadaMesh::MeshInterface* mesh, const Alat::IoManager* io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile)
{
  _chronometer.start("basicInit");
  Solver::basicInit(model, mesh, io_manager, looptype, parameterfile);

  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("matrixstorage", &_matrixstorage, "full");
  dataformathandler.insert("mgtransfer", &_mgtransfer, "galerkin");
  dataformathandler.insert("linalg", &_linalg, "structured");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Solver", 0);
  _model = model;

  getBoundaryManager()->basicInit(parameterfile, model);
  getBoundaryManager()->checkBoundaryConditions( getVariableManager()->getUnknowns(), getMesh()->getBoundaryInfo() );
  getModel()->setRunDir( io_manager->getRunDir() );
  _femmanager = newFemManager();
  getFemManager()->basicInit( getModel()->getProblemData(), getVariableManager(), mesh);
  _femmanagerR = _femmanager->clone();
  getDofManagerAllVariables()->basicInit( this, getMesh() );
  // getModel()->setIntegratorData( getIntegratorManager() );
  getIntegratorManager()->setProblemData( getModel()->getProblemData() );
  getIntegratorManager()->initIntegrators(this);
  getIntegratorManager()->defineIntegratorMaps();

  const VariablesMap& unknowns = getVariableManager()->getUnknowns();
  _multileveltransferallvariables = newMultiLevelTransferAllVariables();

  Alat::StringDoubleVectorMap& variablescales = getModel()->getVariablesScales();
  if(variablescales.size() == 0)
  {
    _varscaleafterinitialize = true;
    for(VariablesMap::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
    {
      variablescales[p->first].set_size( p->second->getNComponents() );
      for(int icomp = 0; icomp < p->second->getNComponents(); icomp++)
      {
        variablescales[p->first][icomp] = 1.0;
      }
    }
  }
  _domainintegrationloop = newDomainIntegrationLoop();
  if(_domainintegrationloop == NULL)
  {
    _error_string("basicInit", "integrationloop has to be in the basicInit() of derived Solver before calling this function!");
  }
  _domainintegrationloop->basicInit(this);


  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    _ncompOfVars[p->first] = p->second->getNComponents();
  }
  int nlevels  = getNLevels();
  _nOfVars.set_size( nlevels );
  // std::cerr << "DomainSolver::basicInit() mesh->getName()  nlevels " << mesh->getName() << " " << nlevels << "\n";
  // assert(0);
  for(int level = 0; level < nlevels; level++)
  {
    getMesh()->setResolution(level);
    for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
    {
      getDofManagerAllVariables()->setMeshLevel(level);
      _nOfVars[level][p->first] = getDofManagerAllVariables()->getDofInformationOfVariable( p->second->getVarName() )->ndof();
    }
  }
  getMesh()->setResolution(0);

  // std::cerr << " DomainSolver::basicInit(): _nOfVars = " << _nOfVars << "\n";
  // std::cerr << " DomainSolver::basicInit(): _ncompOfVars = " << _ncompOfVars << "\n";
  _chronometer.stop("basicInit");
}

/*---------------------------------------------------------*/
void DomainSolver::reInit()
{
  _chronometer.start("Memory");
  for(GhostVectorAgent::iterator p = _ghost_vector_agent.begin(); p != _ghost_vector_agent.end(); p++)
  {
    if(p->second == NULL)
    {
      p->second = newVector( p->first );
    }
    reInitVector( p->second, p->first);
  }
  Alat::GhostVector gdata("data", "data");
  if( _ghost_vector_agent.exists(gdata) )
  {
    getDomainIntegrationLoop()->setDataVector( getVector(gdata) );
  }
  if(_multileveltransferallvariables)
  {
    _multileveltransferallvariables->basicInit( this, getMesh(), *getVariableManager(), getDofManagerAllVariables() );
  }
  _chronometer.stop("Memory");
}

/*---------------------------------------------------------*/
void DomainSolver::reInitForInterpolation(const FadaMesh::MeshInterface* mesh)
{
  _chronometer.start("Memory");
  for(GhostVectorAgent::iterator p = _ghost_vector_agent.begin(); p != _ghost_vector_agent.end(); p++)
  {
    if(p->second == NULL)
    {
      p->second = newVector( p->first );
    }
    reInitVector( p->second, p->first);
  }
  Alat::GhostVector gdata("data", "data");
  if( _ghost_vector_agent.exists(gdata) )
  {
    getDomainIntegrationLoop()->setDataVector( getVector(gdata) );
  }
  Solver::reInitForInterpolation(mesh);
  _chronometer.stop("Memory");
}

/*---------------------------------------------------------*/
void DomainSolver::reInitMatrices()
{
  for(GhostMatrixAgent::iterator p = _ghost_matrix_agent.begin(); p != _ghost_matrix_agent.end(); p++)
  {
    assert(p->second == NULL);
    p->second = newMatrix( p->first );
    reInitMatrix( p->second, p->first );
  }
}

Alat::MatrixInterface* DomainSolver::newMatrix(const Alat::GhostMatrix& gmatrix)
{
  if(gmatrix.getLevel() == -1)
  {
    return new Fada::MultiLevelMatrix;
  }
  else
  {
    return new Alat::MatrixAllVariables;
  }
}

/*---------------------------------------------------------*/
void DomainSolver::reInitMatrix(Alat::MatrixInterface* matrix, const Alat::GhostMatrix& gmatrix)
{
  Fada::MultiLevelMatrix* multilevelmatrix = dynamic_cast<Fada::MultiLevelMatrix*>( matrix );
  if(multilevelmatrix)
  {
    int nlevels  = getNLevels();
    multilevelmatrix->set_size(nlevels);
    for(int level = 0; level < nlevels; level++)
    {
      multilevelmatrix->getMatrixPointer(level) = newLevelMatrix(level, gmatrix);
      getDofManagerAllVariables()->setMeshLevel(level);
      reInitLevelMatrix(level, multilevelmatrix->getMatrix(level), gmatrix);
    }
  }
  else
  {
    Alat::SystemMatrixInterface* smatrix = dynamic_cast<Alat::SystemMatrixInterface*>( matrix );
    assert(smatrix);
    int level = gmatrix.getLevel();
    getDofManagerAllVariables()->setMeshLevel(level);
    reInitLevelMatrix(level, smatrix, gmatrix);
  }
}

/*---------------------------------------------------------*/
void DomainSolver::matrixZero(Alat::GhostMatrix& A) const
{
  _ghost_matrix_agent(A)->zeros();
}

bool DomainSolver::existsMatrix(Alat::GhostMatrix& matrix) const
{
  return _ghost_matrix_agent.exists(matrix);
}

/*---------------------------------------------------------*/
void DomainSolver::reInitLinearSolvers()
{
  _chronometer.start("Memory");
  for(GhostLinearSolverAgent::iterator p = _ghost_linear_solver_agent.begin(); p != _ghost_linear_solver_agent.end(); p++)
  {
    assert(p->second == NULL);
    {
      p->second = newLinearSolver( p->first);
    }
    reInitLinearSolver(p->first);
  }
  _chronometer.stop("Memory");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::registerVector(const Alat::GhostVector& vector)
{
  assert( not _ghost_vector_agent.exists(vector) );
  Alat::GhostVector vector2(vector);
  if(vector.getType() == "unknowns")
  {
    if(vector.getDescription() == "allvars")
    {
      vector2.setVariables( getVariableManager()->getUnknownsNames() );
    }
    else
    {
      Alat::StringSet s1 = vector.getVariables();
      Alat::StringSet s2 = getVariableManager()->getUnknownsNames();
      Alat::StringSet unknownvars;
      std::set_intersection ( s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter( unknownvars, unknownvars.begin() ) );
      vector2.setVariables( unknownvars );
    }
  }
  else if(vector.getType() == "postprocess")
  {
    // std::cerr << "vector="<<vector<<"\n";
    vector2.setVariables( getVariableManager()->getPostProcessNames() );
    // std::cerr << "vector2="<<vector2<<"\n";
  }
  else if(vector.getType() == "data")
  {
    vector2.setVariables( getVariableManager()->getDataNames() );
  }
  else
  {
    std::cerr << "type="<< vector.getType() <<"\n";
    std::cerr << "variables="<< vector.getVariables() <<"\n";
    assert(0);
  }
  _ghost_vector_agent.enrol(vector2);
}

/*--------------------------------------------------------------------------*/
Alat::VectorInterface* DomainSolver::newVector(const Alat::GhostVector& v)
{
  if(v.getLevel() == -1)
  {
    return new Fada::MultiLevelVector;
  }
  else
  {
    return new Alat::SystemVector;
  }
}

/*--------------------------------------------------------------------------*/
Alat::VariableVectorInterface* DomainSolver::newVariableVector(const Fada::VariableInterface* var) const
{
  return new Alat::VariableVector( var->getNComponents() );
}

/*---------------------------------------------------------*/
Alat::SystemVector* DomainSolver::newLevelVector(const Alat::GhostVector& v) const
{
  return new Alat::SystemVector;
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitVariableVector(int level, Alat::VariableVectorInterface* v, const Fada::VariableInterface* var) const
{
  getMesh()->setResolution( level );
  getDofManagerAllVariables()->setMeshLevel(level);
  int n = getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() )->ndof();
  v->set_size(var->getNComponents(), n);
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitVariableMatrix(int level, Alat::VariableMatrixInterface* matrix, const Fada::VariableInterface* vari, const Fada::VariableInterface* varj) const
{
  Alat::VariableDiagonalMatrix* diagonalmatrix = dynamic_cast<Alat::VariableDiagonalMatrix*>( matrix );
  getMesh()->setResolution( level );
  getDofManagerAllVariables()->setMeshLevel(level);
  if(diagonalmatrix)
  {
    // std::cerr << "UnstructuredDomainSolver::reInitVariableMatrix() diagonalmatrix level "  << getDofInformationOfVariable(vari)->ndof() << " " << level << "\n";
    // diagonalmatrix->set_size( getMesh(), getDofInformationOfVariable(vari), getDofInformationOfVariable(varj) );
    int n = getDofManagerAllVariables()->getDofInformationOfVariable( vari->getVarName() )->ndof();
    diagonalmatrix->set_size( n );
  }
  else
  {
    // std::cerr << "UnstructuredDomainSolver::reInitVariableMatrix() level " << level << " varnamei=" << vari->getVarName()<<" varnamej="<<varj->getVarName()<<" ndofi=" << getDofInformationOfVariable(vari)->ndof()<<" ndofj=" << getDofInformationOfVariable(varj)->ndof() << "\n";
    int n = getDofManagerAllVariables()->getDofInformationOfVariable( vari->getVarName() )->ndof();
    getDomainIntegrationLoop()->matrixConnectivityPerLevel( vari->getVarName(), varj->getVarName(), getIntegratorManager(), level, n, matrix);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitVariableVectorForInterpolation(int level, Alat::VariableVectorInterface* v, const Fada::VariableInterface* var, const FadaMesh::MeshInterface* mesh) const
{
  mesh->setResolution( level );
  int n = getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() )->ndof(mesh);
  // std::cerr << "UnstructuredDomainSolver::reInitVariableVectorForInterpolation() " << n << "\n";
  v->set_size(var->getNComponents(), n);
}

/*---------------------------------------------------------*/
void DomainSolver::reInitLevelVector(int level, Alat::SystemVector* v, const Alat::GhostVector& gv) const
{
  // std::cerr << "DomainSolver::reInitLevelVector() gv="<<gv<< " level="<<level<<"\n";
  const std::string& name = gv.getName();
  const std::string& type = gv.getType();

  Alat::SystemVector* vav = dynamic_cast<Alat::SystemVector*>( v );
  assert(vav);

  vav->set_size( gv.getVariables() );
  for(Alat::SystemVector::iterator p = vav->begin(); p != vav->end(); p++)
  {
    const VariableInterface* variable;
    if(type == "unknowns")
    {
      variable = getVariableManager()->getUnknown(p->first);
    }
    else if(type == "postprocess")
    {
      variable = getVariableManager()->getPostProcess(p->first);
    }
    else
    {
      variable = getVariableManager()->getData(p->first);
    }
    std::string femname = variable->getFemName();
    if(femname == "none")
    {
      int ncomp = variable->getNComponents();
      // p->second = new Alat::VariableVector(ncomp);
      // p->second->set_size( variable->getN() );
      p->second = new Alat::VariableVector(ncomp, variable->getN());
    }
    else
    {
      p->second = newVariableVector(variable);
      reInitVariableVector( level, p->second, variable );
    }
  }
}

/*---------------------------------------------------------*/
void DomainSolver::reInitVector(Alat::VectorInterface* v, const Alat::GhostVector& gv)
{
  Fada::MultiLevelVector* vmg = dynamic_cast<Fada::MultiLevelVector*>( v );
  if(vmg)
  {
    assert(gv.getLevel() == -1);
    int nlevels  = getNLevels();
    vmg->set_size(nlevels);
    for(int level = 0; level < nlevels; level++)
    {
      vmg->getVectorPointer(level) = newLevelVector(gv);
      reInitLevelVector(level, vmg->getVector(level), gv);
    }
  }
  else
  {
    Alat::SystemVector* vs = dynamic_cast<Alat::SystemVector*>( v );
    assert(vs);
    int level = gv.getLevel();
    reInitLevelVector(level, vs, gv);
  }
}

/*---------------------------------------------------------*/
void DomainSolver::integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const
{
  _chronometer.start("PostProcess");
  const Alat::SystemVector* vp = dynamic_cast<const Alat::SystemVector*>( getVector(p) );
  assert(vp);
  Alat::SystemVector* vptime = dynamic_cast< Alat::SystemVector*>( getVector(ptime) );
  assert(vptime);
  const VariablesMap& variables = getVariableManager()->getPostProcess();
  if( vp->getVariables() != getVariableManager()->getPostProcessNames() )
  {
    std::cerr << "DomainSolver::integrateInTimePostProcess() vp->getVariables()="<<vp->getVariables()<<" getVariableManager()->getPostProcessNames()="<<getVariableManager()->getPostProcessNames()<<"\n";
    std::cerr << "vptime->getVariables()="<<vptime->getVariables() << "\n";
    assert(0);
  }
  double dt = getDeltaT();
  // std::cerr << "DomainSolver::integrateInTimePostProcess() dt " << dt << "\n";
  for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    VariableInterface* var = p->second;
    const Alat::VariableVector* vpvar = dynamic_cast<const Alat::VariableVector*>( vp->getVector(p->first) );
    assert(vpvar);
    if( var->timeIntegral() )
    {
      // if(first)
      // {
      //   vptime->getVector(p->first)->zeros();
      // }
      // else
      {
        vptime->getVector(p->first)->add( dt, vp->getVector(p->first) );
      }
      // std::cerr << "@ Solver::integrateInTimePostProcess() dt " << " ?? " << dt << " ?? " <<*dvp  << " ==> " << *dvptime << "\n";
    }
    else
    {
      vptime->getVector(p->first)->equal( vp->getVector(p->first) );
      // std::cerr << "@ Solver::integrateInTimePostProcess() dt " << " ?? " << dt << " vptime " <<*dvp  << " ==> dvptime " << *dvptime << "\n";
    }
  }
  _chronometer.stop("PostProcess");
}

/*---------------------------------------------------------*/
void DomainSolver::writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& v, int number) const
{
  _chronometer.start("InputOutput");
  const Fada::MultiLevelVector* vmg = dynamic_cast<const Fada::MultiLevelVector*>( getVector(v) );
  if(vmg)
  {
    Fada::MultiLevelVector* hmg = dynamic_cast<Fada::MultiLevelVector*>( getVector(help) );
    assert(hmg);
    int nlevels = vmg->nlevels();
    assert( nlevels == hmg->nlevels() );
    assert( nlevels == getMesh()->getNLevels() );
    for(int level = 0; level < nlevels; level++)
    {
      std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::Unknowns, "U", number);
      filename = getIoManager()->getFileNameOnLevel( filename, level ) + ".h5";
      _writeUnknownVariables(hmg->getVector(level), vmg->getVector(level), filename);
    }
  }
  else
  {
    const Alat::SystemVector* vs = dynamic_cast<const Alat::SystemVector*>( getVector(v) );
    assert(vs);
    Alat::SystemVector* h = dynamic_cast<Alat::SystemVector*>( getVector(help) );
    assert(h);
    std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::Unknowns, "U", number);
    filename = getIoManager()->getFileNameOnLevel( filename, 0 )+".h5";
    _writeUnknownVariables(h, vs, filename);
  }
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/
void DomainSolver::readUnknownVariables(Alat::GhostVector& gv, int number)
{
  _chronometer.start("InputOutput");
  assert( _ghost_vector_agent.exists(gv) );
  Fada::MultiLevelVector* vmg = dynamic_cast< Fada::MultiLevelVector*>( getVector(gv) );
  if(vmg)
  {
    int nlevels = vmg->nlevels();
    for(int level = 0; level < nlevels; level++)
    {
      std::string filename = getIoManager()->getFileNameIn(number);
      filename = getIoManager()->getFileNameOnLevel( filename, level ) + ".h5";
      _readUnknownVariables(vmg->getVector(level), filename);
    }
  }
  else
  {
    Alat::SystemVector* v = dynamic_cast<Alat::SystemVector*>( getVector(gv) );
    assert(v);
    std::string filename = getIoManager()->getFileNameIn(number);
    filename = getIoManager()->getFileNameOnLevel( filename, 0 )+".h5";
    _readUnknownVariables(v, filename);
  }
  _chronometer.stop("InputOutput");
}

/*---------------------------------------------------------*/
void DomainSolver::_writeUnknownVariables(Alat::SystemVector* h, const Alat::SystemVector* vs, std::string filename) const
{
  // std::cerr << "_writeUnknownVariables filename="<<filename<<"\n";
  h->equal(vs);
  h->scalePerVariables( getVariablesScales() );


  // std::string timedataset = "DataTime";
  // hsize_t dimsf[1];                  // dataset dimensions
  // dimsf[0] = 2;
  // const H5std_string datasetname(timedataset);
  // H5::IntType datatype( H5::PredType::NATIVE_DOUBLE );
  // datatype.setOrder( H5T_ORDER_LE );
  // H5::DataSpace fspace(1, dimsf);       //file dataspace
  // H5::DataSet dataset = file.createDataSet( datasetname, datatype, fspace );
  // Alat::DoubleVector timedata(2);
  // timedata[0] = getTime();
  // timedata[1] = getDeltaT();
  // dataset.write( reinterpret_cast<const double*>( &( *timedata.begin() ) ), H5::PredType::NATIVE_DOUBLE, fspace, fspace);
  // dataset.close();

  const VariableManager* variablemanager = getVariableManager();
  const VariablesMap& variables = variablemanager->getUnknowns();
  Alat::Map<std::string, Alat::StringVector> blocknames;
  for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const VariableInterface* var = p->second;
    var->getVisualizationNames(blocknames[p->first]);
    assert( p->first == p->second->getVarName() );
  }
  h->savehdf5(filename, blocknames);

  // const H5std_string h5filename(filename);
  // H5::H5File file( h5filename, H5F_ACC_TRUNC );
  // for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  // {
  //   Alat::StringVector blocknames;
  //   const VariableInterface* var = p->second;
  //   var->getVisualizationNames(blocknames);
  //   assert( p->first == p->second->getVarName() );
  //   h->getVector(p->first)->writeInFile(file, blocknames);
  // }
  // file.close();
}

/*---------------------------------------------------------*/
void DomainSolver::_readUnknownVariables(Alat::SystemVector* v, std::string filename)
{
  // std::cerr << "_readUnknownVariables filename="<<filename<<"\n";
  // const H5std_string h5filename(filename);
  // H5::H5File file( h5filename,  H5F_ACC_RDONLY );

  // hsize_t dimsf[1];
  // std::string timedataset = "DataTime";
  // const H5std_string datasetname(timedataset);
  // H5::DataSet dataset = file.openDataSet( datasetname );
  // H5::DataSpace fspace = dataset.getSpace();
  // fspace.getSimpleExtentDims( dimsf, NULL);
  // Alat::DoubleVector timedatavector(2);
  // dataset.read(reinterpret_cast<double*> ( &( *timedatavector.begin() ) ), H5::PredType::NATIVE_DOUBLE, fspace, fspace );
  // dataset.close();
  // Fada::TimeData timedata;
  // timedata.time = timedatavector[0];
  // timedata.deltat = timedatavector[1];
  // timedata.deltatold = 0.0;
  // double masscoefimplicit = 0.0;
  // setTimeInfo( timedata, masscoefimplicit );
  //

  // const VariableManager* variablemanager = getVariableManager();
  // const VariablesMap& variables = variablemanager->getUnknowns();
  // for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  // {
  //   Alat::StringVector blocknames;
  //   const VariableInterface* var = p->second;
  //   var->getVisualizationNames(blocknames);
  //   assert( p->first == p->second->getVarName() );
  //   v->getVector(p->first)->readFromFile(file, blocknames);
  // }

  const VariableManager* variablemanager = getVariableManager();
  const VariablesMap& variables = variablemanager->getUnknowns();
  Alat::Map<std::string, Alat::StringVector> blocknames;
  for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const VariableInterface* var = p->second;
    var->getVisualizationNames(blocknames[p->first]);
    assert( p->first == p->second->getVarName() );
  }
  v->loadhdf5(filename, blocknames);

  v->scalePerVariablesInverse( getVariablesScales() );
}

/*---------------------------------------------------------*/
void DomainSolver::writePostProcessVariables(Alat::DoubleVector& est, const Alat::GhostVector& gv, int number) const
{
  _chronometer.start("InputOutput");
  const Alat::SystemVector* v = dynamic_cast<const Alat::SystemVector*>( getVector(gv) );
  assert(v);
  std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::PostProcess, "P", number);
  std::string filenameestimator = getIoManager()->getResultsDir() + "/" + "estimator";
  filename = getIoManager()->getFileNameOnLevel( filename, 0 )+".h5";


  const VariableManager* variablemanager = getVariableManager();
  const VariablesMap& variables = variablemanager->getPostProcess();
  Alat::Map<std::string, Alat::StringVector> blocknames;
  for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const VariableInterface* var = p->second;
    var->getVisualizationNames(blocknames[p->first]);
    assert( p->first == p->second->getVarName() );
    std::string varname = var->getVarName();
    if(varname.substr(0,4)=="Est_")
    {
      const Alat::VariableVector* varvec = dynamic_cast<const Alat::VariableVector*>(v->getVector(p->first));
      assert(varvec);

      // std::cerr << "DomainSolver::writePostProcessVariables() " << varvec->norm() << "\n";

      std::ofstream fileestimator( filenameestimator.c_str() );
      varvec->saveFada(fileestimator, "binary");
      if(est.size()!=varvec->getNComponents())
      {
        est.set_size(varvec->getNComponents());
        est.zeros();
      }
      for(int icomp=0;icomp<varvec->getNComponents();icomp++)
      {
        // est[icomp] += varvec->norm(icomp);
        est[icomp] += varvec->sum(icomp);
      }
    }
  }
  v->savehdf5(filename, blocknames);
  // return;
  //
  //
  //
  //
  //
  // // std::cerr << "??? DomainSolver::writePostProcessVariables() filename " << filename << " " << filenameestimator << "\n";
  // // std::cerr << "??? DomainSolver::writePostProcessVariables() *v " << v->writeAscii(std::cerr) << "\n";
  // const H5std_string h5filename(filename);
  // H5::H5File file( h5filename, H5F_ACC_TRUNC );
  // std::string timedataset = "DataTime";
  // hsize_t dimsf[1];                    // dataset dimensions
  // dimsf[0] = 2;
  // const H5std_string datasetname(timedataset);
  // H5::IntType datatype( H5::PredType::NATIVE_DOUBLE );
  // datatype.setOrder( H5T_ORDER_LE );
  // H5::DataSpace fspace(1, dimsf);       //file dataspace
  // H5::DataSet dataset = file.createDataSet( datasetname, datatype, fspace );
  // Alat::DoubleVector timedata(2);
  // timedata[0] = getTime();
  // timedata[1] = getDeltaT();
  // dataset.write( reinterpret_cast<const double*>( &( *timedata.begin() ) ), H5::PredType::NATIVE_DOUBLE, fspace, fspace);
  // dataset.close();
  //
  // const VariableManager* variablemanager = getVariableManager();
  // const VariablesMap& variables = variablemanager->getPostProcess();
  // for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
  // {
  //   Alat::StringVector blocknames;
  //   const VariableInterface* var = p->second;
  //   var->getVisualizationNames(blocknames);
  //   std::string femname = p->second->getFemName();
  //   std::string varname = var->getVarName();
  //   // std::cerr << "SolutionIo::writePostProcess() name " << varname << " -> " << femname << "\n";
  //   if(varname.substr(0,4)=="Est_")
  //   {
  //     // std::cerr << "SolutionIo::writePostProcess() filenameestimator " << filenameestimator << "\n";
  //     const Alat::VariableVector* varvec = dynamic_cast<const Alat::VariableVector*>(v->getVector(p->first));
  //     assert(varvec);
  //     std::ofstream fileestimator( filenameestimator.c_str() );
  //     varvec->saveFada(fileestimator, "binary");
  //     if(est.size()!=varvec->getNComponents())
  //     {
  //       est.set_size(varvec->getNComponents());
  //       est.zeros();
  //     }
  //     for(int icomp=0;icomp<varvec->getNComponents();icomp++)
  //     {
  //       est[icomp] += varvec->sum(icomp);
  //     }
  //   }
  //   if(femname != "none")
  //   {
  //     v->getVector(p->first)->writeInFile(file, blocknames);
  //   }
  // }
  //
  //
  //
  // file.close();
  _chronometer.stop("InputOutput");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level) const
{
  _chronometer.start("PostProcess");

  const Fada::MultiLevelVector* umg = dynamic_cast<const Fada::MultiLevelVector*>( getVector(u) );
  const Fada::MultiLevelVector* uoldmg = dynamic_cast<const Fada::MultiLevelVector*>(  getVector(uold) );
  Fada::MultiLevelVector* fmg = dynamic_cast< Fada::MultiLevelVector*>( getVector(f) );
  if(level == -1)
  {
    assert(umg);
    assert(uoldmg);
    assert(fmg);
    int nlevels = getNLevels();
    assert(umg->nlevels() == nlevels);
    assert(uoldmg->nlevels() == nlevels);
    assert(fmg->nlevels() == nlevels);
    for(int level = 0; level < nlevels; level++)
    {
      getDomainIntegrationLoop()->postProcess( status, getVector(f), getVector(u), getVector(uold), level);
    }
  }
  else
  {
    // const Alat::SystemVector* uall;
    // const Alat::SystemVector* uoldall;
    // Alat::SystemVector* fall;
    // if(umg)
    // {
    //   assert(uoldmg);
    //   assert(fmg);
    //   uall = umg->getVector(level);
    //   uoldall = uoldmg->getVector(level);
    //   fall = fmg->getVector(level);
    // }
    // else
    // {
    //   uall = dynamic_cast<const Alat::SystemVector*>( getVector(u) );
    //   uoldall = dynamic_cast<const Alat::SystemVector*>( getVector(uold) );
    //   fall = dynamic_cast< Alat::SystemVector*>( getVector(f) );
    // }
    // assert(uall);
    // assert(fall);
    getDomainIntegrationLoop()->postProcess( status, getVector(f), getVector(u), getVector(uold), level);
  }


  const Alat::SystemVector* uall = dynamic_cast<const Alat::SystemVector*>( getVector(u) );
  Alat::SystemVector* fall = dynamic_cast< Alat::SystemVector*>( getVector(f) );

  if(not uall)
  {
    assert(level > -1);
    assert(uoldmg);
    assert(fmg);
    uall = dynamic_cast<const Alat::SystemVector*>( umg->getVector(level) );
    // uoldall = dynamic_cast<const Alat::SystemVector*>( uoldmg->getVector(level));
    fall = dynamic_cast< Alat::SystemVector*>( fmg->getVector(level) );
  }
  assert(uall);
  assert(fall);
  for(Alat::SystemVector::const_iterator p = fall->begin(); p != fall->end(); p++)
  {
    Alat::StringVector bouts = Alat::Tokenize(p->first, "_");
    // std::cerr << "bouts="<<bouts<<"\n";
    if(bouts.size() != 3)
    {
      continue;
    }
    assert(bouts[0] == "Err");
    std::string type = bouts[2];
    if(type == "US" or type == "OS")
    {
      Alat::VariableVector* fv = dynamic_cast<Alat::VariableVector*>( p->second );
      assert(fv);
      const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>( uall->getVector(bouts[1]) );
      assert(uv);
      int ncomp = p->second->getNComponents();
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        if(type == "US")
        {
          double umin = uv->min(icomp);
          double us = ( *fv )(icomp, 0)-umin;
          if(us <= 0.0)
          {
            us = 0.0;
          }
          ( *fv )(icomp, 0) = us;
          std::cerr << "DomainSolver::postProcess() US " << ( *fv )(icomp, 0) << "\n";
        }
        else if(type == "OS")
        {
          double umax = uv->max(icomp);
          double os = umax - ( *fv )(icomp, 0);
          if(os <= 0.0)
          {
            os = 0.0;
          }
          ( *fv )(icomp, 0) = os;
          std::cerr << "DomainSolver::postProcess() OS " << ( *fv )(icomp, 0) << "\n";
        }
      }
    }
  }

  _chronometer.stop("PostProcess");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& f) const
{
  _chronometer.start("Rhs");
  Fada::MultiLevelVector* fmg = dynamic_cast<Fada::MultiLevelVector*>( getVector(f) );
  if(fmg)
  {
    int nlevels = getNLevels();
    for(int level = 0; level < nlevels; level++)
    {
      getDomainIntegrationLoop()->constructRightHandSide( status, fmg->getVector(level), level );
    }
  }
  else
  {
    getDomainIntegrationLoop()->constructRightHandSide( status, getVector(f) );
  }
  _chronometer.stop("Rhs");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::initSolution(Alat::GhostVector& u)
{
  // getVector(u)->zeros();
  Fada::MultiLevelVector* umg = dynamic_cast<Fada::MultiLevelVector*>( getVector(u) );
  if(umg)
  {
    int nlevels = getNLevels();
    std::cerr << "DomainSolver::testProlongate() " << nlevels-1 << " : " << umg->getVector(nlevels-1)->norm()<<"\n";
    for(int level = 0; level < nlevels; level++)
    {
      getDomainIntegrationLoop()->initSolution( umg, level);
      Alat::SystemVector* uall = umg->getVector(level);
      if(_varscaleafterinitialize)
      {
        Alat::StringSet variables = getVariableManager()->getUnknownsNames();
        if(level == 0)
        {
          Alat::StringDoubleVectorMap maxvalues;
          for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
          {
            Alat::VariableVectorInterface* uvar = uall->getVector(*p);
            int ncomp = uvar->getNComponents();
            maxvalues[*p].set_size(ncomp);
            for(int icomp = 0; icomp < ncomp; icomp++)
            {
              double max = uvar->maxnorm(icomp);
              maxvalues[*p][icomp] = max;
            }
          }
          std::cerr << "DomainSolver::initSolution() maxvalues="<<maxvalues<<"\n";
          getModel()->setVariablesScales(maxvalues);
        }

        const Alat::StringDoubleVectorMap& variablescales = getModel()->getVariablesScales();
        std::cerr << "DomainSolver::initSolution() variablescales="<<variablescales<<"\n";
        getDomainIntegrationLoop()->setVariablesScales(variablescales);

        for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
        {
          Alat::VariableVectorInterface* uvar = uall->getVector(*p);
          uvar->scaleinv(variablescales[*p]);
        }
      }
    }
    return;
  }



  getDomainIntegrationLoop()->initSolution( getVector(u) );

  if(_varscaleafterinitialize)
  {
    Alat::StringDoubleVectorMap maxvalues;
    Alat::SystemVector* uall = dynamic_cast<Alat::SystemVector*>( getVector(u) );
    if(not uall)
    {
      _error_string( "initSolution", getVector(u)->getName() );
    }
    Alat::StringSet variables = getVariableManager()->getUnknownsNames();
    for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
    {
      Alat::VariableVectorInterface* uvar = uall->getVector(*p);
      int ncomp = uvar->getNComponents();
      maxvalues[*p].set_size(ncomp);
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        double max = uvar->maxnorm(icomp);
        maxvalues[*p][icomp] = max;
      }
    }
    std::cerr << "DomainSolver::initSolution() maxvalues="<<maxvalues<<"\n";
    getModel()->setVariablesScales(maxvalues);
    const Alat::StringDoubleVectorMap& variablescales = getModel()->getVariablesScales();
    std::cerr << "DomainSolver::initSolution() variablescales="<<variablescales<<"\n";
    getDomainIntegrationLoop()->setVariablesScales(variablescales);

    for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
    {
      Alat::VariableVectorInterface* uvar = uall->getVector(*p);
      uvar->scaleinv(variablescales[*p]);
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const
{
  _chronometer.start("Form");
  Alat::GhostVector guold("timesteps_0", "unknowns");
  Alat::GhostVector guveryold("timesteps_1", "unknowns");
  const Alat::VectorInterface* uold = NULL;
  const Alat::VectorInterface* uveryold = NULL;
  if( _ghost_vector_agent.exists(guold) )
  {
    uold = getVector(guold);
  }
  if( _ghost_vector_agent.exists(guveryold) )
  {
    uveryold = getVector(guveryold);
  }
  getDomainIntegrationLoop()->constructForm(status, getVector(f), getVector(u), uold, uveryold);
  _chronometer.stop("Form");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  getDomainIntegrationLoop()->computeNormSquared( status, norms, getVector(u), getVector(du) );
}

/*--------------------------------------------------------------------------*/
void DomainSolver::computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& du) const
{
  _chronometer.start("Linearization");
  Alat::GhostVector guold("timesteps_0", "unknowns");
  Alat::GhostVector guveryold("timesteps_1", "unknowns");
  const Alat::VectorInterface* uold = NULL;
  const Alat::VectorInterface* uveryold = NULL;
  if( _ghost_vector_agent.exists(guold) )
  {
    uold = getVector(guold);
  }
  if( _ghost_vector_agent.exists(guveryold) )
  {
    uveryold = getVector(guveryold);
  }
  getDomainIntegrationLoop()->computeLinearization(status, getVector(f), getVector(u), getVector(du), uold, uveryold);
  _chronometer.stop("Linearization");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& urhs, double d) const
{
  _chronometer.start("Rhs");
  getDomainIntegrationLoop()->integrateTimeRhs(status, getVector(f), getVector(u), getVector(urhs), d);
  _chronometer.stop("Rhs");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& gA, const Alat::GhostVector& gu)
{
  _chronometer.start("ConstructMatrix");

  Alat::MatrixInterface* A = _ghost_matrix_agent(gA);
  assert(A);
  A->zeros();
  Alat::GhostVector guold("timesteps_0", "unknowns");
  Alat::GhostVector guveryold("timesteps_1", "unknowns");
  const Alat::VectorInterface* u = NULL;
  const Alat::VectorInterface* uold = NULL;
  const Alat::VectorInterface* uveryold = NULL;
  if(gA.getLevel() == -1)
  {
    Alat::GhostVector gumg("umg", "unknowns", -1);
    projectToAllLevels(gumg, gu);
    u = getVector(gumg);
    if( _ghost_vector_agent.exists(guold) )
    {
      Alat::GhostVector guoldmg("uoldmg", "unknowns", -1);
      projectToAllLevels(guoldmg, guold);
      uold = getVector(guoldmg);
    }
    if( _ghost_vector_agent.exists(guveryold) )
    {
      Alat::GhostVector guveryoldmg("uveruoldmg", "unknowns", -1);
      projectToAllLevels(guveryoldmg, guveryold);
      uveryold = getVector(guveryoldmg);
    }
  }
  else
  {
    u = getVector(gu);
    if( _ghost_vector_agent.exists(guold) )
    {
      uold = getVector(guold);
    }
    if( _ghost_vector_agent.exists(guveryold) )
    {
      uveryold = getVector(guveryold);
    }
  }
  getDomainIntegrationLoop()->constructJacobianMatrix(status, A, u, uold, uveryold );

  // Amg->getMatrix(0)->write(std::cerr);
  _chronometer.stop("ConstructMatrix");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::projectToAllLevels(Alat::GhostVector& gu, const Alat::GhostVector& gufine) const
{
  Fada::MultiLevelVector* umg = dynamic_cast<Fada::MultiLevelVector*>( getVector(gu) );
  assert(umg);
  umg->getVector(0)->equal( getVector(gufine) );
  projectToAllLevels(gu);
}

/*--------------------------------------------------------------------------*/
void DomainSolver::projectToAllLevels(Alat::GhostVector& gu) const
{
  Fada::MultiLevelVector* umg = dynamic_cast<Fada::MultiLevelVector*>( getVector(gu) );
  assert(umg);
  for(int level = 1; level < getNLevels(); level++)
  {
    _multileveltransferallvariables->project( level, umg->getVector(level), umg->getVector(level-1) );
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::restrict ( int level, Alat::GhostVector& gu, const Alat::GhostVector& gf ) const
{
  // std::cerr << "BlockSolver::restrict() " << level-1 << " -> " << level << "\n";
  Fada::MultiLevelVector* u = dynamic_cast<Fada::MultiLevelVector*>( getVector(gu) );
  assert(u);
  const Fada::MultiLevelVector* f = dynamic_cast<const Fada::MultiLevelVector*>( getVector(gf) );
  assert(f);

  if( level < getMesh()->getNLevels() )
  {
    _multileveltransferallvariables->restrict ( level, u->getVector(level), f->getVector(level-1) );
  }
  else
  {
    assert(0);
    u->getVector(level)->equal( f->getVector(level-1) );
  }
}
/*--------------------------------------------------------------------------*/
void DomainSolver::prolongate(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf, double d) const
{
  // std::cerr << "BlockSolver::prolongate() " << level+1 << " -> " << level << "\n";
  Fada::MultiLevelVector* u = dynamic_cast<Fada::MultiLevelVector*>( getVector(gu) );
  assert(u);
  const Fada::MultiLevelVector* f = dynamic_cast<const Fada::MultiLevelVector*>( getVector(gf) );
  assert(f);

  if( level+1 < getMesh()->getNLevels() )
  {
    u->getVector(level)->zeros();
    _multileveltransferallvariables->prolongate(level, u->getVector(level), f->getVector(level+1), d);
  }
  else
  {
    assert(0);
    u->getVector(level)->equal( f->getVector(level+1) );
  }
}

/*--------------------------------------------------------------------------*/
Alat::LinearSolverInterface* DomainSolver::newLinearSolver(const Alat::GhostLinearSolver& linearsolver)
{
  return new Fada::MultiLevelLinearSolver( getNLevels(), linearsolver );
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitLinearSolver(const Alat::GhostLinearSolver& linearsolver)
{
  _chronometer.start("ConstructLinearSolver");
  Fada::MultiLevelSolverInterface* multilevellinearsolver = dynamic_cast<Fada::MultiLevelSolverInterface*>( getLinearSolver(linearsolver) );
  assert(multilevellinearsolver);
  multilevellinearsolver->reInit( getParameterFile(), linearsolver.getMatrix(), this);
  _chronometer.stop("ConstructLinearSolver");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::constructMultigridTransfer(const Alat::GhostMatrix& ghostmatrix)
{
  if(not _multileveltransferallvariables)
  {
    return;
  }
  assert(_multileveltransferallvariables);
  if( _multileveltransferallvariables->isConstant() )
  {
    return;
  }
  _chronometer.start("ConstructLinearSolver");
  const Fada::MultiLevelMatrix* mgmatrix = dynamic_cast<const Fada::MultiLevelMatrix*>( getMatrix( ghostmatrix ) );
  assert(mgmatrix);
  int nlevels = getNLevels();
  for(int level = 0; level < nlevels-1; level++)
  {
    _multileveltransferallvariables->constructTransfer( level, mgmatrix->getMatrix(level), mgmatrix->getMatrix(level+1) );
  }
  _chronometer.stop("ConstructLinearSolver");
}

/*--------------------------------------------------------------------------*/
void DomainSolver::constructLinearSolver(const Alat::GhostLinearSolver& linearsolver)
{
  _chronometer.start("ConstructLinearSolver");
  getLinearSolver(linearsolver)->compute();
  getLinearSolver(linearsolver)->restart();
  _chronometer.stop("ConstructLinearSolver");
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* DomainSolver::newVariableMatrix(int level, const std::string& storage, const Fada::VariableInterface* vari, const Fada::VariableInterface* varj) const
{
  int ncomp = vari->getNComponents();
  int mcomp = varj->getNComponents();
  return new Alat::VariableMatrix(ncomp, mcomp, storage);
}

/*--------------------------------------------------------------------------*/
Alat::SystemMatrixInterface* DomainSolver::newLevelMatrix(int level, const Alat::GhostMatrix& gmatrix) const
{
  return new Alat::MatrixAllVariables;
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitLevelMatrix(int level, Alat::SystemMatrixInterface* matrix, const Alat::GhostMatrix& gmatrix) const
{
  _chronometer.start("Memory");

  matrix->set_size( gmatrix.getVariablesOut(), gmatrix.getVariablesIn() );
  Alat::MatrixAllVariables* matrixallvariables = dynamic_cast<Alat::MatrixAllVariables*>( matrix );
  assert(matrixallvariables);
  const VariablesMap& unknowns = getVariableManager()->getUnknowns();
  for(Alat::MatrixAllVariables::iterator p = matrixallvariables->begin(); p != matrixallvariables->end(); p++)
  {
    p->second = newVariableMatrix(level, _matrixstorage, unknowns[p->first.first], unknowns[p->first.second]);
    reInitVariableMatrix( level, p->second, unknowns[p->first.first], unknowns[p->first.second] );
  }
  _chronometer.stop("Memory");
}

//
// /*--------------------------------------------------------------------------*/
// void DomainSolver::oswaldInterpolation( Alat::VariableVector* uvisu, const Alat::SystemVector* u, const Fada::VariableInterface* var) const
// {
//   std::string visutype = var->getVisualizationType();
//   assert ( visutype == "node" );
//
//   FemInterface* fem = getFemManager()->getFem(var->getVarName() )->clone();
//   const DofInformationInterface* dofinfo = getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() );
//
//   int ncomp = uvisu->getNComponents();
//   assert( ncomp == u->getNComponents() );
//
//   uvisu->zeros();
//   Alat::DoubleVector unode(ncomp);
//   Alat::AssembleVector uloc;
//   Alat::IntVector count( getMesh()->getNNodes() );
//   count.zeros();
//   Alat::Vector<Alat::Node> F( getMesh()->getNNodesPerCell(0) );
//   for(int iK = 0; iK < getMesh()->getNCells(); iK++)
//   {
//     Alat::IntVector indices;
//     dofinfo->getCellIndices(iK, indices);
//     int n = indices.size();
//     uloc.set_size(ncomp, n);
//     u->extract(uloc, indices);
//     getMesh()->getNodesOfCell(iK, F);
//     fem->reInitCell(F);
//     // std::cerr << " ???@@@@@@ getMesh()->getNNodes() " << getMesh()->getNNodes() << "\n";
//     for(int ii = 0; ii < getMesh()->getNNodesPerCell(iK); ii++)
//     {
//       int iN = getMesh()->getNodeIdOfCell(iK, ii);
//       count[iN]++;
//       const Alat::Node& node = getMesh()->getNode(iN);
//       // std::cerr << "*** Variable::ProjectToVisualizationData() node " << node << "\n";
//       fem->reInitPhysicalPoint(node);
//       fem->computeFunction(unode, uloc);
//       for(int icomp = 0; icomp < ncomp; icomp++)
//       {
//         ( *uvisu )( icomp, iN ) += unode[icomp];
//       }
//     }
//   }
//   for(int iN = 0; iN < getMesh()->getNNodes(); iN++)
//   {
//     double dn = (double) count[iN];
//     for(int icomp = 0; icomp < ncomp; icomp++)
//     {
//       ( *uvisu )( icomp, iN ) /= dn;
//     }
//   }
//   delete fem;
// }

/*--------------------------------------------------------------------------*/
void DomainSolver::interpolateSolution( Alat::GhostVector& unew, const Alat::GhostVector& uold) const
{
  int level = 0;
  getMesh()->setResolution(level);
  vectorZero(unew);
  Alat::SystemVector* unewall = NULL;
  const Alat::SystemVector* uoldall = NULL;

  Fada::MultiLevelVector* unewmg = dynamic_cast< Fada::MultiLevelVector*>( getVector(unew) );
  if(unewmg)
  {
    const Fada::MultiLevelVector* uoldmg = dynamic_cast<const Fada::MultiLevelVector*>( getVector(uold) );
    assert(uoldmg);
    unewall = unewmg->getVector(level);
    uoldall = uoldmg->getVector(level);
  }
  else
  {
    unewall = dynamic_cast<Alat::SystemVector*>( getVector(unew) );
    uoldall = dynamic_cast<const Alat::SystemVector*>( getVector(uold) );
  }
  assert(unewall);
  assert(uoldall);
  const VariablesMap& unknowns = getVariableManager()->getUnknowns();
  for(VariablesMap::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    // int ivar = getVariableManager()->getIndexOfUnknownVariable(p->first);
    VariableInterface* var = p->second;
    const DofInformationInterface* dofinformation = getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() );
    if(dofinformation->getFem() == NULL)
    {
      continue;
    }
    // std::cerr << "DomainSolver::interpolateSolution() " << p->first << "\n";
    // std::cerr << "DomainSolver::interpolateSolution() " << uoldall->getVector(p->first)->norm() << "\n";
    interpolateOneVariable( dofinformation, unewall->getVector(p->first), uoldall->getVector(p->first) );
    // std::cerr << "DomainSolver::interpolateSolution() " << unewall->getVector(p->first)->norm() << "\n";
  }
  if(unewmg)
  {
    assert(0);
    projectToAllLevels(unew);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::interpolateOneVariable(const DofInformationInterface* dofinformation, Alat::VariableVectorInterface* unew, const Alat::VariableVectorInterface* uold) const
{
  const Fada::FemInterface* FEM = dofinformation->getFem();
  if(FEM->getName() == "FemP12d")
  {
    _interpolateP1(unew, uold);
    return;
  }

  const FadaMesh::GeometryObject* geo = getRefinedMesh()->getGeometryObject("RefineInfo");
  const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  int ncomp = unew->getNComponents();
  int ncellsold = getMesh()->getNCells();
  Alat::AssembleVector uH, uh;
  Alat::IntVector olddof, newdof;
  Alat::DoubleVector scale;
  scale.set_size(ncomp);
  scale.fill(1.0);
  // std::cerr << "DomainSolver::interpolateOneVariable() ncellsold = " << ncellsold << "\n";
  // std::cerr << "DomainSolver::interpolateOneVariable() " << uold->norm() << "\n";
  for(int iK = 0; iK < ncellsold; iK++)
  {
    dofinformation->getCellIndicesInterpolate(iK, olddof);
    dofinformation->getCellIndicesInterpolateRefined( iK, newdof, getRefinedMesh(), refinfo );
    // std::cerr << "olddof = " << olddof << " newdof = " << newdof  << "\n";
    uH.set_size( ncomp, olddof.size() );
    uh.set_size( ncomp, newdof.size() );
    uold->extract(uH, olddof, scale);
    if( olddof.size() == newdof.size() )
    {
      uh = uH;
    }
    else
    {
      dofinformation->interpolate( iK, uh, uH);
    }
    std::cerr << "uh = " << uh << " uH = " << uH  << "\n";
    unew->set(uh, newdof);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::addDiagonalCoupling(int level, Alat::GhostMatrix& gA, const Alat::SystemMatrixInterface* matrix) const
{
  const Alat::MatrixAllVariables* matrixav = dynamic_cast<const Alat::MatrixAllVariables*>( matrix );
  Fada::MultiLevelMatrix* Amg = dynamic_cast<Fada::MultiLevelMatrix*>( _ghost_matrix_agent(gA) );
  if(Amg)
  {
    Alat::MatrixAllVariables* Aav = dynamic_cast<Alat::MatrixAllVariables*>( Amg->getMatrix(level) );
    assert(Aav);
    const Alat::MatrixAllVariables* matrixav = dynamic_cast<const Alat::MatrixAllVariables*>( matrix );
    Aav->addMatrix(matrixav);
  }
  else
  {
    Alat::MatrixAllVariables* Aav = dynamic_cast<Alat::MatrixAllVariables*>( _ghost_matrix_agent(gA) );
    assert(Aav);
    assert( level == gA.getLevel() );
    Aav->addMatrix(matrixav);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::reInitVectorForInterpolation(Alat::GhostVector& u) const
{
  assert( _ghost_vector_agent.exists(u) );
  Fada::MultiLevelVector* umg = dynamic_cast< Fada::MultiLevelVector*>( getVector(u) );
  if(umg)
  {
    int nlevels = getRefinedMesh()->getNLevels();
    umg->set_size(nlevels);
    u.setVariables( getVariableManager()->getUnknownsNames() );
    for(int level = 0; level < nlevels; level++)
    {
      if(umg->getVector(level) == NULL)
      {
        umg->getVectorPointer(level) = newLevelVector(u);
        // std::cerr << "DomainSolver::reInitVectorForInterpolation() u="<<u<<"\n";
        reInitLevelVector(level, umg->getVector(level), u);
      }
    }
    for(int level = 0; level < nlevels; level++)
    {
      Alat::SystemVector* uall = dynamic_cast<Alat::SystemVector*>( umg->getVector(level) );
      assert(uall);
      getRefinedMesh()->setResolution(level);
      const VariablesMap& unknowns = getVariableManager()->getUnknowns();
      uall->set_size( getVariableManager()->getUnknownsNames() );
      for(Alat::SystemVector::iterator p = uall->begin(); p != uall->end(); p++)
      {
        const VariableInterface* variable = getVariableManager()->getUnknown(p->first);
        p->second = newVariableVector( variable);
        reInitVariableVectorForInterpolation( level, p->second, variable, getRefinedMesh() );
      }
    }
  }
  else
  {
    Alat::SystemVector* uall = dynamic_cast<Alat::SystemVector*>( getVector(u) );
    assert(uall);
    int level = u.getLevel();
    getRefinedMesh()->setResolution(level);

    const VariablesMap& unknowns = getVariableManager()->getUnknowns();
    uall->set_size( getVariableManager()->getUnknownsNames() );
    for(Alat::SystemVector::iterator p = uall->begin(); p != uall->end(); p++)
    {
      const VariableInterface* variable = getVariableManager()->getUnknown(p->first);
      p->second = newVariableVector( variable);
      reInitVariableVectorForInterpolation( level, p->second, variable, getRefinedMesh() );
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& x, const Alat::GhostVector& b)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
bool DomainSolver::existsVector(const Alat::GhostVector& v) const
{
  return _ghost_vector_agent.exists(v);
}

/*---------------------------------------------------------*/
void DomainSolver::registerMatrix(const Alat::GhostMatrix& matrix)
{
  assert( not _ghost_matrix_agent.exists(matrix) );
  Alat::GhostMatrix matrix2(matrix);
  if(matrix.getDescription() == "allvars")
  {
    matrix2.setVariablesIn( getVariableManager()->getUnknownsNames() );
    matrix2.setVariablesOut( getVariableManager()->getUnknownsNames() );
  }
  else
  {
    assert(0);
  }
  _ghost_matrix_agent.enrol(matrix2);
}

/*--------------------------------------------------------------------------*/
void DomainSolver::registerLinearSolver(const Alat::GhostLinearSolver& v)
{
  Alat::GhostLinearSolver linearsolver(v);
  if(v.getDescription() == "allvars")
  {
    linearsolver.setVariables( getVariableManager()->getUnknownsNames() );
  }
  // else
  // {
  //   assert(0);
  // }
  _ghost_linear_solver_agent.enrol(linearsolver);
}

Alat::LinearSolverInterface* DomainSolver::getLinearSolver(const Alat::GhostLinearSolver& v) const
{
  return _ghost_linear_solver_agent(v);
}

bool DomainSolver::existsLinearSolver(Alat::GhostLinearSolver& v) const
{
  return _ghost_linear_solver_agent.exists(v);
}

/*--------------------------------------------------------------------------*/
BoundaryManager* DomainSolver::getBoundaryManager()
{
  return &_boundary_manager;
}

const BoundaryManager* DomainSolver::getBoundaryManager() const
{
  return &_boundary_manager;
}

VariableManager* DomainSolver::getVariableManager()
{
  return &_variablemanager;
}

DofManagerAllVariables* DomainSolver::getDofManagerAllVariables()
{
  return &_dof_manager;
}

const DofManagerAllVariables* DomainSolver::getDofManagerAllVariables() const
{
  return &_dof_manager;
}

const DofInformationInterface* DomainSolver::getDofInformationOfVariable(const Fada::VariableInterface* var) const
{
  return getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() );
}

FemManagerInterface* DomainSolver::getFemManager()
{
  assert(_femmanager);
  return _femmanager;
}

const FemManagerInterface* DomainSolver::getFemManager() const
{
  assert(_femmanager);
  return _femmanager;
}

const FemManagerInterface* DomainSolver::getFemManagerRight() const
{
  return _femmanagerR;
}

/*--------------------------------------------------------------------------*/
void DomainSolver::writeVariablesInfo() const
{
  // unknowns
  {
    std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::Unknowns, "UInfo" );
    // std::string domainfilename = Alat::IoManager::getFileNameOnLevel( filename, getIndex() );
    std::ofstream file( filename.c_str() );
    const VariablesMap& variables = getVariableManager()->getUnknowns();
    for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
    {
      VariableInterface* var = p->second;
      std::string visutype = var->getVisualizationType();
      Alat::StringVector varnames;
      var->getVisualizationNames(varnames);
      int ncomp = var->getNComponents();
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        file<<varnames[icomp]<<"  "<<visutype<<'\n';
      }
    }
    file.close();
  }
  // postprocess
  {
    std::string filename = getIoManager()->getFileNameOut(Alat::IoManager::PostProcess, "PInfo" );
    // std::string domainfilename = Alat::IoManager::getFileNameOnLevel(filename, getIndex() );
    std::ofstream file( filename.c_str() );
    const VariablesMap& variables = getVariableManager()->getPostProcess();
    for(VariablesMap::const_iterator p = variables.begin(); p != variables.end(); p++)
    {
      VariableInterface* var = p->second;
      std::string visutype = var->getVisualizationType();
      std::string femname = var->getFemName();
      if(femname == "none")
      {
        continue;
      }
      Alat::StringVector varnames;
      var->getVisualizationNames(varnames);
      int ncomp = var->getNComponents();
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        file<<varnames[icomp]<<"  "<<visutype<<'\n';
      }
    }
    file.close();
  }
}

/*--------------------------------------------------------------------------*/
Fada::ModelInterface*& DomainSolver::getModelPointer()
{
  return _model;
}

const Fada::ModelInterface* DomainSolver::getModel() const
{
  return _model;
}

Fada::ModelInterface* DomainSolver::getModel()
{
  return _model;
}

/*--------------------------------------------------------------------------*/
const VariableManager* DomainSolver::getVariableManager() const
{
  return &_variablemanager;
}

/*---------------------------------------------------------*/
const Alat::VariableMatrixInterface* DomainSolver::getMatrix(int level, const Alat::GhostMatrix& ghostmatrix, std::string outvar, std::string invar) const
{
  const Alat::SystemMatrixInterface* matrix = dynamic_cast<const Alat::SystemMatrixInterface*>( getMatrix(level, ghostmatrix) );
  assert(matrix);
  return matrix->getMatrix(outvar, invar);
}

/*---------------------------------------------------------*/
Alat::VariableMatrixInterface* DomainSolver::getMatrix(int level, Alat::GhostMatrix& ghostmatrix, std::string outvar, std::string invar) const
{
  Alat::SystemMatrixInterface* matrix = dynamic_cast< Alat::SystemMatrixInterface*>( getMatrix(level, ghostmatrix) );
  assert(matrix);
  return matrix->getMatrix(outvar, invar);
}

/*---------------------------------------------------------*/
Alat::SystemVector* DomainSolver::getVector(int level, Alat::GhostVector& ghostvector) const
{
  MultiLevelVector* mlvector = dynamic_cast<MultiLevelVector*>( _ghost_vector_agent(ghostvector) );
  if(mlvector)
  {
    return mlvector->getVector(level);
  }
  else
  {
    Alat::SystemVector* svector = dynamic_cast<Alat::SystemVector*>( _ghost_vector_agent(ghostvector) );
    assert( level == ghostvector.getLevel() );
    return svector;
  }
}

/*---------------------------------------------------------*/
const Alat::SystemVector* DomainSolver::getVector(int level, const Alat::GhostVector& ghostvector) const
{
  const MultiLevelVector* mlvector = dynamic_cast<const MultiLevelVector*>( _ghost_vector_agent(ghostvector) );
  if(mlvector)
  {
    return mlvector->getVector(level);
  }
  else
  {
    const Alat::SystemVector* svector = dynamic_cast<const Alat::SystemVector*>( _ghost_vector_agent(ghostvector) );
    assert( level == ghostvector.getLevel() );
    return svector;
  }
}

/*---------------------------------------------------------*/
const Alat::MatrixInterface* DomainSolver::getMatrix(const Alat::GhostMatrix& ghostmatrix) const
{
  return _ghost_matrix_agent(ghostmatrix);
}

Alat::MatrixInterface* DomainSolver::getMatrix(Alat::GhostMatrix& ghostmatrix) const
{
  // assert(0);
  return _ghost_matrix_agent(ghostmatrix);
}

Alat::SystemMatrixInterface* DomainSolver::getMatrix(int level, Alat::GhostMatrix& ghostmatrix) const
{
  Fada::MultiLevelMatrix* matrixmultilevel = dynamic_cast<Fada::MultiLevelMatrix*>( _ghost_matrix_agent(ghostmatrix) );
  if(matrixmultilevel)
  {
    return matrixmultilevel->getMatrix(level);
  }
  else
  {
    Alat::SystemMatrixInterface* smatrix = dynamic_cast<Alat::SystemMatrixInterface*>( _ghost_matrix_agent(ghostmatrix) );
    assert(smatrix);
    assert(ghostmatrix.getLevel() == level);
    return smatrix;
  }
}

const Alat::SystemMatrixInterface* DomainSolver::getMatrix(int level, const Alat::GhostMatrix& ghostmatrix) const
{
  const Fada::MultiLevelMatrix* matrixmultilevel = dynamic_cast<const Fada::MultiLevelMatrix*>( _ghost_matrix_agent(ghostmatrix) );
  if(matrixmultilevel)
  {
    return matrixmultilevel->getMatrix(level);
  }
  else
  {
    const Alat::SystemMatrixInterface* smatrix = dynamic_cast<const Alat::SystemMatrixInterface*>( _ghost_matrix_agent(ghostmatrix) );
    assert(smatrix);
    assert(ghostmatrix.getLevel() == level);
    return smatrix;
  }
}

/*--------------------------------------------------------------------------*/
const Alat::VectorInterface* DomainSolver::getVector(const Alat::GhostVector& v) const
{
  return _ghost_vector_agent(v);
}

Alat::VectorInterface* DomainSolver::getVector(Alat::GhostVector& v) const
{
  return _ghost_vector_agent(v);
}

bool DomainSolver::vectorExists(const Alat::GhostVector& v) const
{
  return _ghost_vector_agent.exists(v);
}

/*--------------------------------------------------------------------------*/
void DomainSolver::setVectorFromAllVariables( Alat::GhostVector& gu, const Alat::GhostVector& guall) const
{
  const Fada::MultiLevelVector* uallmg = dynamic_cast<const Fada::MultiLevelVector*>( getVector(guall) );
  assert(uallmg);
  Fada::MultiLevelVector* umg = dynamic_cast< Fada::MultiLevelVector*>( getVector(gu) );
  assert(umg);
  int level = 0;
  const Alat::SystemVector* uallav = dynamic_cast<const Alat::SystemVector*>( uallmg->getVector(level) );
  assert(uallav);
  Alat::SystemVector* uav = dynamic_cast<Alat::SystemVector*>( umg->getVector(level) );
  assert(uav);

  for(Alat::SystemVector::iterator p = uav->begin(); p != uav->end(); p++)
  {
    p->second->equal( uallav->getVector(p->first) );
  }
}

void DomainSolver::setVectorToAllVariables( Alat::GhostVector& guall, const Alat::GhostVector& gu) const
{
  Fada::MultiLevelVector* uallmg = dynamic_cast< Fada::MultiLevelVector*>( getVector(guall) );
  assert(uallmg);
  const Fada::MultiLevelVector* umg = dynamic_cast<const Fada::MultiLevelVector*>( getVector(gu) );
  assert(umg);
  int level = 0;
  Alat::SystemVector* uallav = dynamic_cast< Alat::SystemVector*>( uallmg->getVector(level) );
  assert(uallav);
  const Alat::SystemVector* uav = dynamic_cast<const Alat::SystemVector*>( umg->getVector(level) );
  assert(uav);

  for(Alat::SystemVector::const_iterator p = uav->begin(); p != uav->end(); p++)
  {
    uallav->getVector(p->first)->equal(p->second);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const
{
  assert( mgf.getVariables() == gf.getVariables() );
  assert( level == gf.getLevel() );
  Fada::MultiLevelVector* mf = dynamic_cast<Fada::MultiLevelVector*>( getVector(mgf) );
  assert(mf);
  const Alat::SystemVector* f = dynamic_cast<const Alat::SystemVector*>( getVector(gf) );
  assert(f);
  mf->getVector(level)->equal( f );
}

void DomainSolver::setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const
{
  assert( mgf.getVariables() == gf.getVariables() );
  assert( level == gf.getLevel() );
  const Fada::MultiLevelVector* mf = dynamic_cast<const Fada::MultiLevelVector*>( getVector(mgf) );
  assert(mf);
  Alat::SystemVector* f = dynamic_cast<Alat::SystemVector*>( getVector(gf) );
  assert(f);
  f->equal( mf->getVector(level) );
}

double DomainSolver::vectorNorm(const Alat::GhostVector& gu) const
{
  return getVector(gu)->norm();
}

void DomainSolver::vectorZero(Alat::GhostVector& gu) const
{
  getVector(gu)->zeros();
}

void DomainSolver::vectorEqual(Alat::GhostVector& gr, const Alat::GhostVector& gf) const
{
  getVector(gr)->equal( getVector(gf) );
}

void DomainSolver::vectorAdd(Alat::GhostVector& gp, double d, const Alat::GhostVector& gq) const
{
  getVector(gp)->add( d, getVector(gq) );
}

void DomainSolver::vectorScale(Alat::GhostVector& r, double d) const
{
  getVector(r)->scale( d);
}

double DomainSolver::vectorScalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& p) const
{
  return getVector(r)->scalarProduct( getVector(p) );
}

/*--------------------------------------------------------------------------*/
double DomainSolver::vectorNorm(int level, const Alat::GhostVector& gu) const
{
  return getVector(level, gu)->norm();
}

void DomainSolver::vectorZero(int level, Alat::GhostVector& gu) const
{
  getVector(level, gu)->zeros();
}

void DomainSolver::vectorEqual(int level, Alat::GhostVector& gr, const Alat::GhostVector& gf) const
{
  getVector(level, gr)->equal( getVector(level, gf) );
}

void DomainSolver::vectorAdd(int level, Alat::GhostVector& gp, double d, const Alat::GhostVector& gq) const
{
  getVector(level, gp)->add( d, getVector(level, gq) );
}

void DomainSolver::vectorScale(int level, Alat::GhostVector& r, double d) const
{
  getVector(level, r)->scale( d);
}

double DomainSolver::vectorScalarProduct(int level, const Alat::GhostVector& r, const Alat::GhostVector& p) const
{
  return getVector(level, r)->scalarProduct( getVector(level, p) );
}

void DomainSolver::vectorScalarProductPerVariables(int level, Alat::StringDoubleMap& sp, const Alat::GhostVector& p, const Alat::GhostVector& q) const
{
  const Alat::SystemVector* pvv = dynamic_cast<const Alat::SystemVector*>( getVector(level, p) );
  assert(pvv);
  const Alat::SystemVector* qvv = dynamic_cast<const Alat::SystemVector*>( getVector(level, q) );
  assert(qvv);

  // std::cerr << "DomainSolver::vectorScalarProductPerVariables() sp="<<sp<<"\n";
  for(Alat::SystemVector::const_iterator p = pvv->begin(); p != pvv->end(); p++)
  {
    sp[p->first] += p->second->scalarProduct( qvv->getVector(p->first) );
  }
  // std::cerr << "DomainSolver::vectorScalarProductPerVariables() sp="<<sp<<"\n";
}

/*--------------------------------------------------------------------------*/
std::ostream& DomainSolver::vectorWrite(std::ostream& os, const Alat::GhostVector& r) const
{
  getVector(r)->writeAscii(os);
  os << "\n";
  return os;
}

// void DomainSolver::_varname2index(Alat::IntVector& indices, const Alat::StringVector& names) const
// {
//   indices.set_size( names.size() );
//   for(int ivar = 0; ivar < names.size(); ivar++)
//   {
//     int ivarall = getVariableManager()->getIndexOfUnknownVariable(names[ivar]);
//     indices[ivar] = ivarall;
//   }
// }

/*--------------------------------------------------------------------------*/
void DomainSolver::setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const
{
  Alat::StringSet variables = ui.getVariables();
  Alat::SystemVector* us = dynamic_cast<Alat::SystemVector*>( getVector(u) );
  const Alat::SystemVector* uis = dynamic_cast<const Alat::SystemVector*>( getVector(ui) );
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    if( us->hasVector(*p) )
    {
      us->getVector(*p)->equal( uis->getVector(*p) );
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const
{
  Alat::StringSet variables = ui.getVariables();
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( getVector(u) );
  Alat::SystemVector* uis = dynamic_cast<Alat::SystemVector*>( getVector(ui) );
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    if( uis->hasVector(*p) )
    {
      uis->getVector(*p)->equal( us->getVector(*p) );
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::matrixVectorProduct(int level, const Alat::GhostMatrix& A,  Alat::GhostVector& gr, const Alat::GhostVector& gu, double d) const
{
  // std::cerr << "DomainSolver::matrixVectorProduct() gr="<<gr<<" gu="<<gu<<"\n";
  // std::cerr << "DomainSolver::matrixVectorProduct() r="; getVector(level, gr)->writeAscii(std::cerr); std::cerr << "\n";
  // std::cerr << "DomainSolver::matrixVectorProduct() u="; getVector(level, gu)->writeAscii(std::cerr); std::cerr << "\n";
  getMatrix(level, A)->matrixVectorProduct( getVector(level, gr),  getVector(level, gu), d);
  // std::cerr << "#DomainSolver::matrixVectorProduct() r="; getVector(level, gr)->writeAscii(std::cerr); std::cerr << "\n";
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
//
// Partie Dirichlet fort
//
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletMatrix(Alat::GhostMatrix& gA) const
{
  Fada::MultiLevelMatrix* Amg = dynamic_cast<Fada::MultiLevelMatrix*>( getMatrix(gA) );
  if(Amg)
  {
    for(int level = 0; level < Amg->nlevels(); level++)
    {
      Alat::MatrixAllVariables* matrix = dynamic_cast<Alat::MatrixAllVariables*>( Amg->getMatrix(level) );
      assert(matrix);
      strongDirichletMatrix(level, matrix);
    }
  }
  else
  {
    Alat::MatrixAllVariables* matrix = dynamic_cast<Alat::MatrixAllVariables*>( getMatrix(gA) );
    assert(matrix);
    int level = gA.getLevel();
    strongDirichletMatrix(level, matrix);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletMatrix(int level, Alat::MatrixAllVariables* matrix) const
{
  const Fada::VariablesMap& unknowns = getVariableManager()->getUnknowns();
  const FadaMesh::BoundaryInfo* boundaryinfo = getMesh()->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  const Fada::DofManagerAllVariables* dofmanager = getDofManagerAllVariables();
  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    const Fada::VariableInterface* var = p->second;
    std::string varname = var->getVarName();
    int ncomp = var->getNComponents();
    for(int ic = 0; ic < colors.size(); ic++)
    {
      int color = colors[ic];
      bool strongdirichlet = getBoundaryManager()->boundaryConditionIsStrong(varname, color);
      bool strongnormaldirichlet = getBoundaryManager()->boundaryConditionIsStrongNormal(varname, color);

      if( ( not strongnormaldirichlet ) and ( not strongdirichlet ) )
      {
        continue;
      }
      // std::cerr << "DomainSolver::strongDirichletMatrix() unknowns="  << unknowns<<"\n";
      // assert(0);
      // std::cerr << "DomainSolver::strongDirichletMatrix() varname="  << varname<< " color="<<color<<"strongdirichlet="<<strongdirichlet<<"\n";
      // int ivar = getVariableManager()->getIndexOfUnknownVariable( var->getVarName() );
      // int nvars =  getVariableManager()->getNUnknownVariables();
      const Fada::DofInformationInterface* dofinfo = dofmanager->getDofInformationOfVariable( varname );
      // MESH SET LEVEL !!!!!!
      getMesh()->setResolution(level);
      dofmanager->setMeshLevel(level);
      // Alat::SystemMatrixInterface* smatrix = matrixallvariables->getMatrix(ivar, ivar);
      // assert(smatrix);
      const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
      const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
      const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
      if(strongnormaldirichlet)
      {
        _strongNormalDirichletMatrix(varname, ncomp, matrix, dofinfo, sides, cells, sideids);
      }
      else if(strongdirichlet)
      {
        _strongDirichletMatrix(varname, ncomp, matrix, dofinfo, sides, cells, sideids);
      }
    }
  }
}

void DomainSolver::_strongNormalDirichletMatrix(const std::string& varname, int ncomp, Alat::MatrixAllVariables* matrixallvariables, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const
{
  assert( ncomp == getMesh()->getDimension() );
  Fada::FemInterface* fem = _femmanager->getFem(varname );
  Alat::Vector<Alat::Node> F( getMesh()->getNNodesPerCell(0) );
  // Alat::SystemMatrixInterface* smatrix = matrixallvariables->getMatrix(ivar, ivar);
  // assert(smatrix);

  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    int iK = cells[i];
    int iil = sideids[i];
    getMesh()->getNodesOfCell(iK, F);
    getFemManager()->initCellAndSide( F, iil, getMesh()->cellIsCurved(iK) );
    fem->reInitCellAndSide( F, iil);
    Alat::Node vhat;
    vhat.x() = 0.5;
    fem->reInitReferencePointBoundary(vhat);
    Alat::Node normal = fem->getNormal();
    // std::cerr << "normal=" << normal<<"\n";
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);

    int vcomp = 0;
    if( fabs( normal.x() ) < fabs( normal.y() ) )
    {
      vcomp = 1;
    }
    for(int ii = 0; ii < ids.size(); ii++)
    {
      for(Alat::MatrixAllVariables::iterator p = matrixallvariables->begin(); p != matrixallvariables->end(); p++)
      {
        p->second->zeroLine(ids[ii], vcomp);
        if(p->first.first == p->first.second)
        {
          p->second->setElement( ids[ii], vcomp, 0, normal.x() );
          p->second->setElement( ids[ii], vcomp, 1, normal.y() );
        }
      }
    }
  }
}

void DomainSolver::_strongDirichletMatrix(const std::string& varname, int ncomp, Alat::MatrixAllVariables* matrixallvariables, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const
{
  // std::cerr << "DomainSolver::_strongDirichletMatrix() varname="<<varname << "\n";
  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);
    for(int ii = 0; ii < ids.size(); ii++)
    {
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        for(Alat::MatrixAllVariables::iterator p = matrixallvariables->begin(); p != matrixallvariables->end(); p++)
        {
          p->second->zeroLine(ids[ii], icomp);
          if(p->first.first == p->first.second)
          {
            p->second->setElement( ids[ii], icomp, icomp, 1.0 );
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletVectorSolution(Alat::GhostVector& f) const
{
  Fada::MultiLevelVector* fmg = dynamic_cast<Fada::MultiLevelVector*>( getVector(f) );
  if(fmg)
  {
    for(int level = 0; level < fmg->nlevels(); level++)
    {
      strongDirichletVectorSolution( level, fmg->getVector(level) );
    }
  }
  else
  {
    Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( getVector(f) );
    assert(fs);
    strongDirichletVectorSolution(f.getLevel(), fs);
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletVectorSolution(int level, Alat::SystemVector* f) const
{
  const Fada::VariablesMap& unknowns = getVariableManager()->getUnknowns();
  const FadaMesh::BoundaryInfo* boundaryinfo = getMesh()->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  const Fada::DofManagerAllVariables* dofmanager = getDofManagerAllVariables();
  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    const Fada::VariableInterface* var = p->second;
    std::string varname = var->getVarName();
    int ncomp = var->getNComponents();
    for(int ic = 0; ic < colors.size(); ic++)
    {
      int color = colors[ic];
      bool strongdirichlet = getBoundaryManager()->boundaryConditionIsStrong(varname, color);
      bool strongnormaldirichlet = getBoundaryManager()->boundaryConditionIsStrongNormal(varname, color);
      // std::cerr << "DomainSolver::strongDirichletVectorSolution() color="<<color<<"strongdirichlet="<<strongdirichlet<<"\n";

      if( ( not strongnormaldirichlet ) and ( not strongdirichlet ) )
      {
        continue;
      }
      std::string bdrycond = getBoundaryManager()->boundaryCondition(varname, color);
      const Fada::DataInterface* data = getModel()->getProblemData(varname, bdrycond);
      const Fada::DirichletInterface* dirichlet = dynamic_cast<const Fada::DirichletInterface*>( data );
      if(not dirichlet)
      {
        dirichlet = &_dirichletzero;
      }
      // std::cerr << "varname="<<varname<<" bdrycond="<<bdrycond<<" dirichlet="<< dirichlet->getName()<< "data"<<data<<"\n";
      // int ivar = getVariableManager()->getIndexOfUnknownVariable( var->getVarName() );
      // Alat::VariableVector* vvector = dynamic_cast<Alat::VariableVector*>( systemvector->getVector(ivar) );
      Alat::VariableVector* vvector = dynamic_cast<Alat::VariableVector*>( f->getVector( var->getVarName() ) );
      assert(vvector);
      const Fada::DofInformationInterface* dofinfo = dofmanager->getDofInformationOfVariable( varname );
      getMesh()->setResolution(level);
      dofmanager->setMeshLevel(level);
      const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
      const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
      const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
      if(strongnormaldirichlet)
      {
        _strongNormalDirichletVector(varname, ncomp, color, vvector, dofinfo, sides, cells, sideids, dirichlet);
      }
      else if(strongdirichlet)
      {
        _strongDirichletVector(varname, ncomp, color, vvector, dofinfo, sides, cells, sideids, dirichlet);
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::_strongNormalDirichletVector(const std::string& varname, int ncomp, int color, Alat::VariableVector* vvector, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids, const Fada::DirichletInterface* dirichlet) const
{
  // Fada::FemInterface* fem = getFemManager()->getFem(varname );
  Fada::FemInterface* fem = _femmanager->getFem(varname );
  Alat::Vector<Alat::Node> F( getMesh()->getNNodesPerCell(0) );
  Alat::DoubleVector udir(ncomp);
  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    int iK = cells[i];
    int iil = sideids[i];
    getMesh()->getNodesOfCell(iK, F);
    getFemManager()->initCellAndSide( F, iil, getMesh()->cellIsCurved(iK) );
    fem->reInitCellAndSide( F, iil);
    Alat::Node vhat;
    vhat.x() = 0.0;
    fem->reInitReferencePointBoundary(vhat);
    Alat::Node normal = fem->getNormal();
    // std::cerr << "normal=" << normal<<"\n";
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);

    int vcomp = 0;
    if( fabs( normal.x() ) < fabs( normal.y() ) )
    {
      vcomp = 1;
    }
    for(int ii = 0; ii < ids.size(); ii++)
    {
      udir.zeros();
      if(dirichlet)
      {
        Alat::Node coords = getMesh()->getNode(ids[ii]);
        dirichlet->getValue( udir, color, normal, coords.x(), coords.y(), coords.z(), getTime() );
      }
      ( *vvector )(vcomp, ids[ii]) = udir[vcomp];
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::_strongDirichletVector(const std::string& varname, int ncomp, int color, Alat::VariableVector* vvector, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids, const Fada::DirichletInterface* dirichlet) const
{
  // std::cerr << "DomainSolver::_strongDirichletVector() " << "\n";
  Fada::FemInterface* fem = _femmanager->getFem(varname );
  // Fada::FemInterface* fem = getFemManager()->getFem(varname );
  Alat::Vector<Alat::Node> F( getMesh()->getNNodesPerCell(0) );
  Alat::DoubleVector udir(ncomp);
  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    int iK = cells[i];
    int iil = sideids[i];
    getMesh()->getNodesOfCell(iK, F);
    getFemManager()->initCellAndSide( F, iil, getMesh()->cellIsCurved(iK) );
    fem->reInitCellAndSide( F, iil);
    Alat::Node vhat;
    vhat.x() = 0.0;
    fem->reInitReferencePointBoundary(vhat);
    Alat::Node normal = fem->getNormal();
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);

    for(int ii = 0; ii < ids.size(); ii++)
    {
      udir.zeros();
      if(dirichlet)
      {
        Alat::Node coords = getMesh()->getNode(ids[ii]);
        dirichlet->getValue( udir, color, normal, coords.x(), coords.y(), coords.z(), getTime() );
        // std::cerr << "DomainSolver::_strongDirichletVector() coords=" << coords << " udir="<<udir<<"\n";
      }
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        ( *vvector )(icomp, ids[ii]) = udir[icomp];
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletVectorZero(Alat::GhostVector& f) const
{
  Fada::MultiLevelVector* fmg = dynamic_cast<Fada::MultiLevelVector*>( getVector(f) );
  if(fmg)
  {
    for(int level = 0; level < fmg->nlevels(); level++)
    {
      strongDirichletVectorZero( level, fmg->getVector(level) );
    }
  }
  else
  {
    Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( getVector(f) );
    assert(fs);
    strongDirichletVectorZero( f.getLevel(), fs );
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::strongDirichletVectorZero(int level, Alat::SystemVector* f) const
{
  const Fada::VariablesMap& unknowns = getVariableManager()->getUnknowns();
  const FadaMesh::BoundaryInfo* boundaryinfo = getMesh()->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  const Fada::DofManagerAllVariables* dofmanager = getDofManagerAllVariables();
  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    const Fada::VariableInterface* var = p->second;
    std::string varname = var->getVarName();
    int ncomp = var->getNComponents();
    for(int ic = 0; ic < colors.size(); ic++)
    {
      int color = colors[ic];
      bool strongdirichlet = getBoundaryManager()->boundaryConditionIsStrong(varname, color);
      bool strongnormaldirichlet = getBoundaryManager()->boundaryConditionIsStrongNormal(varname, color);

      if( ( not strongnormaldirichlet ) and ( not strongdirichlet ) )
      {
        continue;
      }
      // std::cerr << "DomainSolver::strongDirichletVectorZero() varname="<<varname <<"  color="<<color<<"strongdirichlet="<<strongdirichlet<<"strongnormaldirichlet="<<strongnormaldirichlet<<"\n";
      // int ivar = getVariableManager()->getIndexOfUnknownVariable( var->getVarName() );
      Alat::VariableVector* vvector = dynamic_cast<Alat::VariableVector*>( f->getVector(varname) );
      assert(vvector);
      const Fada::DofInformationInterface* dofinfo = dofmanager->getDofInformationOfVariable( varname );
      getMesh()->setResolution(level);
      dofmanager->setMeshLevel(level);
      const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
      const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
      const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
      if(strongnormaldirichlet)
      {
        _strongNormalDirichletVectorZero(varname, ncomp, vvector, dofinfo, sides, cells, sideids);
      }
      else if(strongdirichlet)
      {
        _strongDirichletVectorZero(varname, ncomp, vvector, dofinfo, sides, cells, sideids);
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::_strongNormalDirichletVectorZero(const std::string& varname, int ncomp, Alat::VariableVector* vvector, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const
{
  Fada::FemInterface* fem = _femmanager->getFem(varname );
  Alat::Vector<Alat::Node> F( getMesh()->getNNodesPerCell(0) );
  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    int iK = cells[i];
    int iil = sideids[i];
    getMesh()->getNodesOfCell(iK, F);
    getFemManager()->initCellAndSide( F, iil, getMesh()->cellIsCurved(iK) );
    fem->reInitCellAndSide( F, iil);
    Alat::Node vhat;
    vhat.x() = 0.0;
    fem->reInitReferencePointBoundary(vhat);
    Alat::Node normal = fem->getNormal();
    // std::cerr << "normal=" << normal<<"\n";
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);
    int vcomp = 0;
    if( fabs( normal.x() ) < fabs( normal.y() ) )
    {
      vcomp = 1;
    }
    for(int ii = 0; ii < ids.size(); ii++)
    {
      ( *vvector )(vcomp, ids[ii]) = 0.0;
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::_strongDirichletVectorZero(const std::string& varname, int ncomp, Alat::VariableVector* vvector, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const
{
  // // std::cerr << "DomainSolver::_strongDirichletVectorZero() " << "\n";
  for(int i = 0; i < sides.size(); i++)
  {
    int iS = sides[i];
    int iK = cells[i];
    int iil = sideids[i];
    Alat::IntVector ids;
    dofinfo->setDofIdsOnSide(iS, ids);
    for(int ii = 0; ii < ids.size(); ii++)
    {
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        ( *vvector )(icomp, ids[ii]) = 0.0;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DomainSolver::_interpolateP1(Alat::VariableVectorInterface* unew, const Alat::VariableVectorInterface* uold) const
{
  int ncomp = unew->getNComponents();
  const FadaMesh::MultiLevelMesh* multimeshfine = dynamic_cast<const FadaMesh::MultiLevelMesh*>( getRefinedMesh() );
  assert(multimeshfine);
  const FadaMesh::MultiLevelMesh* multimeshcoarse = dynamic_cast<const FadaMesh::MultiLevelMesh*>( getMesh() );
  assert(multimeshcoarse);
  const FadaMesh::QuadToTri* quadtotrifine = dynamic_cast<const FadaMesh::QuadToTri*>( multimeshfine->getMesh(0) );
  assert(quadtotrifine);
  const FadaMesh::QuadToTri* quadtotricoarse =  dynamic_cast<const FadaMesh::QuadToTri*>( multimeshcoarse->getMesh(0) );
  assert(quadtotricoarse);

  const FadaMesh::QuadrilateralMesh& quadmeshcoarse = quadtotricoarse->getQuadrilateralMesh();
  const FadaMesh::QuadrilateralMesh& quadmeshfine = quadtotrifine->getQuadrilateralMesh();

  const FadaMesh::GeometryObject* geo = quadmeshfine.getGeometryObject("RefineInfo");
  // const FadaMesh::GeometryObject* geo = quadmeshcoarse.getGeometryObject("RefineInfo");
  const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  assert(refinfo);
  Alat::VariableVector* unewv = dynamic_cast<Alat::VariableVector*>( unew );
  assert(unewv);
  const Alat::VariableVector* uoldv = dynamic_cast<const Alat::VariableVector*>( uold );
  assert(uoldv);


  const Alat::FixArray<4, int>& refinfoinfonode = refinfo->refinfoinfonode;
  const Alat::FixArray<4, int>& refinfoinfoside = refinfo->refinfoinfoside;
  const Alat::FixArray<4, int>& refinfoinfocell = refinfo->refinfoinfocell;
  unew->zeros();
  const Alat::SparsityPattern& nodesrefinelist = refinfo->getCoarseNodes();
  for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
  {
    // std::cerr << "refinfo->getNCoarseNodes(iK) " << refinfo->getNCoarseNodes(iK) << "\n";
    // pas raffine
    if(refinfo->getNCoarseNodes(iK) == 4)
    {
      if(refinfo->getNCoarseCells(iK) != 1)
      {
        std::cerr << "unrefined Refinfo nodes: ";
        for(int ii = 0; ii < 4; ii++)
        {
          std::cerr <<   refinfo->getCoarseNodesNumber(iK, ii) << " ";
        }
        for(int ii = 0; ii < refinfo->getNCoarseCells(iK); ii++)
        {
          std::cerr <<   refinfo->getCoarseCellNumber(iK, ii) << " ";
        }
        assert(0);
      }
      int iKf = refinfo->getCoarseCellNumber(iK, 0);
      for(int ii = 0; ii < 4; ii++)
      {
        int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK, ii);
        int nodeidf = quadmeshfine.getNodeIdOfCell(iKf, ii);
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          ( *unewv )(icomp, nodeidf) = ( *uoldv )(icomp, nodeidc);
        }
      }
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        ( *unewv )(icomp, centernodef) = ( *uoldv )(icomp, centernodec);
      }
      continue;
    }
    //  raffine
    // std::cerr << "refined Refinfo nodes: ";
    // for(int ii=0;ii<9;ii++)
    // {
    //   std::cerr <<   refinfo->getCoarseNodesNumber(iK,ii) << " ";
    // }
    // std::cerr << "\n";
    for(int ii = 0; ii < 4; ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK, ii);
      int nodeidf = refinfo->getCoarseNodesNumber(iK, refinfoinfonode[ii]);
      // std::cerr << "nodeidc nodeidf " << nodeidc << " ---> " << nodeidf << "\n";
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        ( *unewv )(icomp, nodeidf) = ( *uoldv )(icomp, nodeidc);
      }
    }
    int centernodec = quadmeshcoarse.getNNodes()+iK;
    int centernodef = refinfo->getCoarseNodesNumber(iK, 3);
    // std::cerr << "centernodec " << centernodec << " ---> " << centernodef << "\n";
    for(int icomp = 0; icomp < ncomp; icomp++)
    {
      ( *unewv )(icomp, centernodef) = ( *uoldv )(icomp, centernodec);
    }
    for(int ii = 0; ii < 4; ii++)
    {
      int nodec1 = quadmeshcoarse.getNodeIdOfCell(iK, ii);
      int nodec2 = quadmeshcoarse.getNodeIdOfCell(iK, ( ii+1 )%4);
      int nodeidf = refinfo->getCoarseNodesNumber(iK, refinfoinfoside[ii]);
      // std::cerr << "nodec1 nodec2  " << nodec1 << " : " << nodec2 << " ---> " << nodeidf << "\n";
      for(int icomp = 0; icomp < ncomp; icomp++ )
      {
        ( *unewv )(icomp, nodeidf) = 0.5*( ( *uoldv )(icomp, nodec1) + ( *uoldv )(icomp, nodec2) );
      }
    }
    for(int ii = 0; ii < 4; ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK, refinfoinfocell[ii]);
      int nodeidf = refinfo->getCoarseNodesNumber(iK, refinfoinfonode[ii]);
      int iKf = refinfo->getCoarseCellNumber(iK, ii);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      // std::cerr << "nodeidc centernodec  " << nodeidc << " : " << centernodec << " ---> " << centernodef << "\n";
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        ( *unewv )(icomp, centernodef) = 0.5*( ( *uoldv )(icomp, centernodec) + ( *uoldv )(icomp, nodeidc) );
      }
    }
  }
  // std::cerr << "uold " << uoldv->norm() << "\n";
  // std::cerr << "unewv " << unewv->norm() << "\n";
}
