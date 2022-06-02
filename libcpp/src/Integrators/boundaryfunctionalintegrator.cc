#include  "Fada/boundarymanager.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/variable.h"
#include  "Fada/varinfointegrator.h"
#include  "Integrators/boundaryfunctionalintegrator.h"
#include  "Alat/tokenize.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
BoundaryFunctionalIntegrator::~BoundaryFunctionalIntegrator(){}
BoundaryFunctionalIntegrator::BoundaryFunctionalIntegrator(const Fada::IntegratorInterface* integrator, int color, int ncomp, double scale, std::string bdrycond) : Fada::Integrator(), _color(color), _ncomp(ncomp), _scale(scale), _bdrycond(bdrycond)
{
  _integrator = dynamic_cast<const Fada::IntegratorInterface*>( integrator );
  assert(integrator);
}

BoundaryFunctionalIntegrator::BoundaryFunctionalIntegrator( const BoundaryFunctionalIntegrator& dragandliftintegrator) : Fada::Integrator(dragandliftintegrator)
{
  assert(0);
}

BoundaryFunctionalIntegrator& BoundaryFunctionalIntegrator::operator=( const BoundaryFunctionalIntegrator& dragandliftintegrator)
{
  Fada::Integrator::operator=(dragandliftintegrator);
  assert(0);
  return *this;
}

std::string BoundaryFunctionalIntegrator::getName() const
{
  return "BoundaryFunctionalIntegrator";
}

BoundaryFunctionalIntegrator* BoundaryFunctionalIntegrator::clone() const
{
  return new BoundaryFunctionalIntegrator(*this);
}

/*--------------------------------------------------------------------------*/
void BoundaryFunctionalIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::BoundarySides);
}

/*--------------------------------------------------------------------------*/
Fada::VariableInterface* BoundaryFunctionalIntegrator::constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const
{
  Fada::Variable* var = new Fada::Variable(name, _ncomp, "sum");
  _varname = name;
  std::string name2 = name.substr(0, name.size()-1);
  Alat::StringVector names = Alat::Tokenize(name, "|");
  if(names.size() == _ncomp)
  {
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      var->setVisualizationName(icomp, names[icomp]);
    }
  }
  return var;
}

/*--------------------------------------------------------------------------*/
void BoundaryFunctionalIntegrator::initData()
{
  Fada::Integrator::initData();
  assert( getVarInfo()->getNVarInput() == _integrator->getVarInfo()->getNVarInput() );
  assert(getVarInfo()->getNVarOutput() == 1);
  assert(_varname == *getVarInfo()->getVarNamesOutput().begin());
  Alat::StringIntMap ncomp, nout;
  ncomp[_varname] = _ncomp;
  nout[_varname] = 1;
  _flocLother.set_size(ncomp, nout);
  _flocRother.set_size(ncomp, nout);
}
/*--------------------------------------------------------------------------*/
void BoundaryFunctionalIntegrator::setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const
{
  Fada::Integrator::setFemFunctions(femfctmap);
  _integrator->setFemFunctions(femfctmap);
}
void BoundaryFunctionalIntegrator::setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const
{
  Fada::Integrator::setAssembleData(flocL, flocR);
  _integrator->setAssembleData(_flocLother, _flocRother);  
}

/*--------------------------------------------------------------------------*/
void BoundaryFunctionalIntegrator::postProcessBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  if(color != _color)
  {
    return;
  }
  if( not _boundarymanager->boundaryConditionIs(_bdrycond, color) )
  {
    _error_string("postProcessBoundarySide", "wrong boundary condition", _bdrycond);
  }
  _flocLother.zeros();
  // _integrator->postProcessBoundarySide(status, iS, iK, _flocLother, weight, color, femfcts);
  _integrator->postProcessBoundarySide(status, iS, iK, weight, color);
  (*_flocL)[_varname].add(_scale, _flocLother[_varname]);
}
