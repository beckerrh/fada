#include  "Fada/dirichletinterface.h"
#include  "Fada/integratorsinglevariable.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/varinfointegrator.h"
#include  "Alat/systemassemblevector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegratorSingleVariable::~IntegratorSingleVariable()
{}

IntegratorSingleVariable::IntegratorSingleVariable() : Fada::Integrator(), _dirichlet(NULL), _initdatacalled(false)
{}

IntegratorSingleVariable::IntegratorSingleVariable( const IntegratorSingleVariable& integratorsinglevariable) : Fada::Integrator(integratorsinglevariable)
{
  ( *this ).operator=(integratorsinglevariable);
}

IntegratorSingleVariable& IntegratorSingleVariable::operator=( const IntegratorSingleVariable& integratorsinglevariable)
{
  Fada::Integrator::operator=(integratorsinglevariable);
  assert(0);
  return *this;
}

std::string IntegratorSingleVariable::getName() const
{
  return "IntegratorSingleVariable";
}

/*--------------------------------------------------------------------------*/
void IntegratorSingleVariable::basicInit(const Fada::DomainSolverInterface* solver)
{
  Fada::Integrator::basicInit(solver);
  _mesh = solver->getMesh();
  _meshtype = solver->getMesh()->getType();
}

/*--------------------------------------------------------------------------*/
void IntegratorSingleVariable::initData(std::string bdrycond)
{
  _initdatacalled = true;
  assert(getVarInfo()->getNVarInput() == 1);
  assert(getVarInfo()->getNVarOutput() == 1);
  _varnamein = *getVarInfo()->getVarNamesInput().begin();
  _varnameout = *getVarInfo()->getVarNamesOutput().begin();

  // std::cerr << getName() << " _varnamein = "<<_varnamein << "\n";
  assert(_varnamein!="");

  _ncompin = getVarInfo()->getNCompIn(_varnamein);
  _ncompout = getVarInfo()->getNCompOut(_varnameout);
  _nlocal = getVarInfo()->getNDofLocalOut(_varnameout);

  _udir.set_size( _ncompin );
  if(bdrycond != "")
  {
    const Fada::DataInterface* data = getProblemData(_varnamein, bdrycond);
    _dirichlet = dynamic_cast<const Fada::DirichletInterface*>( data );
  }
  _femL = femL(_varnameout);
  _femR = femR(_varnameout);
}
/*--------------------------------------------------------------------------*/
void IntegratorSingleVariable::setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const
{
  assert(_initdatacalled);
  if(femfctmap.hasKey("U"))
  {
    _UL = &femfctmap["U"][_varnamein];
  }
  if(femfctmap.hasKey("UOLD"))
  {
    _UOLDL = &femfctmap["UOLD"][_varnamein];
  }
  if(femfctmap.hasKey("DU"))
  {
    _DUL = &femfctmap["DU"][_varnamein];
  }
  if(femfctmap.hasKey("BDF"))
  {
    _BDFL = &femfctmap["BDF"][_varnamein];
  }
}
void IntegratorSingleVariable::setFemFunctions(const Fada::FemFunctionsMap& femfctmapL, const Fada::FemFunctionsMap& femfctmapR) const
{
  assert(_initdatacalled);
  if(femfctmapL.hasKey("U"))
  {
    _UL = &femfctmapL["U"][_varnamein];
    _UR = &femfctmapR["U"][_varnamein];
  }
  if(femfctmapL.hasKey("UOLD"))
  {
    _UOLDL = &femfctmapL["UOLD"][_varnamein];
    _UOLDR = &femfctmapR["UOLD"][_varnamein];
  }
  if(femfctmapL.hasKey("BDF"))
  {
    _BDFL = &femfctmapL["BDF"][_varnamein];
    _BDFR = &femfctmapR["BDF"][_varnamein];
  }
  if(femfctmapL.hasKey("DU"))
  {
    _DUL = &femfctmapL["DU"][_varnamein];
    _DUR = &femfctmapR["DU"][_varnamein];
  }
}
void IntegratorSingleVariable::setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const
{
  _fL = &flocL[_varnameout];
  _fR = &flocR[_varnameout];
}
void IntegratorSingleVariable::setAssembleData(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const
{
  _ALL = &AlocLL(_varnameout,_varnameout);
  _ALR = &AlocLR(_varnameout,_varnameout);
  _ARL = &AlocRL(_varnameout,_varnameout);
  _ARR = &AlocRR(_varnameout,_varnameout);
}
