#include  "Alat/filescanner.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Alat/tokenize.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/systemvector.h"
#include  "Fada/domainsolverinterface.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/preconditionerwithvariablematrix.h"
#include  "Fada/variablemanager.h"
#include  "Fada/visitordomainsolveronelevel.h"
#include  "Fada/preconditionerdomainsolver.h"
#include  "Fada/preconditionerselector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
PreconditionerDomainSolver::~PreconditionerDomainSolver(){}
PreconditionerDomainSolver::PreconditionerDomainSolver(const Alat::SystemMatrixInterface* systemmatrix, const Alat::StringVector& variables) : Alat::PreconditionerWithSystemMatrix(systemmatrix), _variables(variables)
{
  _systemmatrix = systemmatrix;
}
PreconditionerDomainSolver::PreconditionerDomainSolver( const PreconditionerDomainSolver& preconditionerdirectpervariables) : Alat::PreconditionerWithSystemMatrix(preconditionerdirectpervariables), _variables(preconditionerdirectpervariables._variables)
{
  ( *this ).operator=(preconditionerdirectpervariables);
}

PreconditionerDomainSolver& PreconditionerDomainSolver::operator=( const PreconditionerDomainSolver& preconditionerdirectpervariables)
{
  Alat::PreconditionerWithSystemMatrix::operator=(preconditionerdirectpervariables);
  assert(0);
  return *this;
}

std::string PreconditionerDomainSolver::getName() const
{
  std::string name = "PreconditionerDomainSolver_";
  Alat::StringSet variableset(_variables);
  for(Alat::StringSet::const_iterator p = variableset.begin();p!=variableset.end();p++)
  {
    name += *p;
  }
  return name;
  // return "PreconditionerDomainSolver";
}

PreconditionerDomainSolver* PreconditionerDomainSolver::clone() const
{
  return new PreconditionerDomainSolver(*this);
}

/*--------------------------------------------------------------------------*/
int PreconditionerDomainSolver::getNVectors() const
{
  return 1;
}

/*--------------------------------------------------------------------------*/
void PreconditionerDomainSolver::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  PreconditionerWithSystemMatrix::basicInit(parameterfile, blockname, visitor);
  _visitordsol = dynamic_cast<const Fada::VisitorDomainSolverOneLevel*>( getVisitor() );
  assert(_visitordsol);
  const Fada::DomainSolverInterface* solver = _visitordsol->getDomainSolver();
  const Fada::VariableManager* variablemanager = solver->getVariableManager();
  int level = _visitordsol->level();
  int nlevels = _visitordsol->nlevels();
  int dimension = solver->getMesh()->getDimension();

  int nvars = _variables.size();
  Alat::StringVector preconditionernames(nvars), sorternames(nvars), schur(nvars);
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("type", &_type, "gs");
  for(int i = 0; i < nvars; i++)
  {
    std::string varname = _variables[i];
    dataformathandler.insert(varname+"_prec", &preconditionernames[i], "none");
    dataformathandler.insert(varname+"_sorter", &sorternames[i], "none");
    dataformathandler.insert(varname+"_schur", &schur[i], "none");
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, getName(), 0);
  // std::cerr << "PreconditionerDomainSolver::basicInit() name="<<getName()<<"\n";
  // std::cerr << "PreconditionerDomainSolver::basicInit() getName()="<<getName()<<"\n";
  // std::cerr << "PreconditionerDomainSolver::basicInit() preconditionernames="<<preconditionernames<<"\n";
  // std::cerr << "PreconditionerDomainSolver::basicInit() schur="<<schur<<"\n";
  // std::cerr << "PreconditionerDomainSolver::basicInit() sorternames="<<sorternames<<"\n";

  for(int i = 0; i < nvars; i++)
  {
    std::string varname = _variables[i];
		if(preconditionernames[i]=="none")
		{
			std::cerr << "need \'" << varname+"_prec" << "\' in Block \'" << getName() << "\'\n";
			assert(0);
		}
	}

  _preconditioners.set_size( nvars );
  int countschur = 0;
  for(int i = 0; i < nvars; i++)
  {
    if(schur[i] != "none")
    {
      _ischur[i] = countschur++;
    }
  }
  _schurmatrices.set_size(countschur);
  for(int i = 0; i < nvars; i++)
  {
    if(schur[i] == "none")
    {
      continue;
    }
    std::string ivar = _variables[i];
    std::string kvar = schur[i];
    assert(ivar != kvar);
    int ischur = _ischur[i];
    _ivarschur[ischur] = ivar;
    _kvarschur[ischur] = kvar;
    std::string type = "none", storage = "full";
//
//      A  C
//      B  D
//
    const Alat::VariableMatrixInterface* A = _systemmatrix->getMatrix(kvar, kvar);
    const Alat::VariableMatrixInterface* B = _systemmatrix->getMatrix(ivar, kvar);
    const Alat::VariableMatrixInterface* C = _systemmatrix->getMatrix(kvar, ivar);
    const Alat::VariableMatrixInterface* D = _systemmatrix->getMatrix(ivar, ivar);

    _schurmatrices[ischur] =  D->newSchurComplementMatrix(B, C, type, storage);
  }
  // std::cerr << "PreconditionerDomainSolver::basicInit() _ivarschur="<<_ivarschur<<"\n";
  // std::cerr << "PreconditionerDomainSolver::basicInit() _kvarschur="<<_kvarschur<<"\n";
  // assert(0);
  // creation prec
  Fada::PreconditionerSelector preconditionerselector;
  for(int i = 0; i < nvars; i++)
  {
    const Alat::VariableMatrixInterface* matrix;
    if(schur[i] == "none")
    {
      matrix = _systemmatrix->getMatrix(_variables[i], _variables[i]);
    }
    else
    {
      // std::cerr << "PreconditionerDomainSolver::basicInit() _ischur[i]="<<_ischur[i]<<" ivar="<<ivar<<"\n";
      matrix = _schurmatrices[_ischur[i]];
    }
    _preconditioners[i] = preconditionerselector.newPreconditioner( preconditionernames[i], sorternames[i], matrix, level, nlevels, dimension);
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerDomainSolver::reInit()
{
  for(int ischur = 0; ischur < _schurmatrices.size(); ischur++)
  {
    std::string ivar = _ivarschur[ischur];
    std::string kvar = _kvarschur[ischur];
    const Alat::VariableMatrixInterface* A = _systemmatrix->getMatrix(kvar, kvar);
    const Alat::VariableMatrixInterface* B = _systemmatrix->getMatrix(ivar, kvar);
    const Alat::VariableMatrixInterface* C = _systemmatrix->getMatrix(kvar, ivar);
    const Alat::VariableMatrixInterface* D = _systemmatrix->getMatrix(ivar, ivar);
    // std::cerr << "PreconditionerDomainSolver::computePreconditioner() schur="<<i<< "\n";
    bool fullStencil = false;
    _schurmatrices[ischur]->reInitSchurMatrix(A, B, C, D, fullStencil);
  }
  for(int i = 0; i < _preconditioners.size(); i++)
  {
    _preconditioners[i]->reInit();
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerDomainSolver::computePreconditioner()
{
  for(int ischur = 0; ischur < _schurmatrices.size(); ischur++)
  {
    std::string ivar = _ivarschur[ischur];
    std::string kvar = _kvarschur[ischur];
    // std::cerr << "PreconditionerDomainSolver::computePreconditioner() ischur="<<ischur<<" ivar="<<ivar<<" kvar="<<kvar<<"\n";
    const Alat::VariableMatrixInterface* A = _systemmatrix->getMatrix(kvar, kvar);
    const Alat::VariableMatrixInterface* B = _systemmatrix->getMatrix(ivar, kvar);
    const Alat::VariableMatrixInterface* C = _systemmatrix->getMatrix(kvar, ivar);
    const Alat::VariableMatrixInterface* D = _systemmatrix->getMatrix(ivar, ivar);
    // std::cerr << "PreconditionerDomainSolver::computePreconditioner() schur="<<i<< "\n";
    _schurmatrices[ischur]->computeSchurComplement(A, B, C, D);
  }
  for(int i = 0; i < _preconditioners.size(); i++)
  {
    // std::cerr << "PreconditionerDomainSolver::computePreconditioner() "<< _preconditioners[i]->getName()<<"\n";
    _preconditioners[i]->computePreconditioner();
  }
}

/*--------------------------------------------------------------------------*/
void PreconditionerDomainSolver::_solveApproximate(AlatEnums::iterationstatus& status, Alat::SystemVector* u, const Alat::SystemVector* f, int iteration) const
{
  Alat::SystemVector* hav = dynamic_cast<Alat::SystemVector*>( getMemory(0) );
  assert(hav);
  Alat::SystemVector* uav = dynamic_cast<Alat::SystemVector*>( u );
  assert(uav);
  const Alat::SystemVector* fav = dynamic_cast<const Alat::SystemVector*>( f );
  assert(fav);

  // std::cerr << "PreconditionerDomainSolver::_solveApproximate() iteration="  << iteration<<"\n";

  if(_type == "jacobi")
  {
    for(int i = 0; i < _variables.size(); i++)
    {
      std::string var = _variables[i];
      const Alat::PreconditionerWithVariableMatrix* preconditioner = dynamic_cast<const Alat::PreconditionerWithVariableMatrix*>( _preconditioners[i] );
      assert(preconditioner);
      preconditioner->_solveApproximate(status, uav->getVector(var), fav->getVector(var), iteration);
    }
  }
  else if(_type == "gs")
  {
    for(int i = 0; i < _variables.size(); i++)
    {
      std::string varI = _variables[i];
      hav->getVector(varI)->equal( fav->getVector(varI) );
      for(int j = 0; j < i; j++)
      {
        std::string varJ = _variables[j];
        _systemmatrix->getMatrix(varI, varJ)->matrixVectorProduct(hav->getVector(varI), uav->getVector(varJ), -1.0);
      }
      const Alat::PreconditionerWithVariableMatrix* preconditioner = dynamic_cast<const Alat::PreconditionerWithVariableMatrix*>( _preconditioners[i] );
      assert(preconditioner);
      preconditioner->_solveApproximate(status, uav->getVector(varI), hav->getVector(varI), iteration);
    }
  }
  else
  {
    assert(0);
  }
}