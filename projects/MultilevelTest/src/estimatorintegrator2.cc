#include  "Alat/assemblevector.h"
#include  "Alat/node.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemassemblevector.h"
#include  "Fada/applicationwithexactsolution.h"
#include  "Fada/datafunctionvector.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Fada/modelinterface.h"
#include  "Fada/variable.h"
#include  "Fada/variablemanager.h"
#include  "Fada/varinfointegrator.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/problemdata.h"
#include  "Alat/tokenize.h"
#include  "Alat/vector.h"
#include  "estimatorintegrator2.h"
#include  <cassert>
#include  <math.h>

// using namespace Integrators;

/*--------------------------------------------------------------------------*/
EstimatorIntegrator2::~EstimatorIntegrator2(){}
EstimatorIntegrator2::EstimatorIntegrator2() : Fada::Integrator(){}
EstimatorIntegrator2::EstimatorIntegrator2( const EstimatorIntegrator2& errorintegrator) : Fada::Integrator(errorintegrator){}
EstimatorIntegrator2& EstimatorIntegrator2::operator=( const EstimatorIntegrator2& errorintegrator)
{
  Fada::Integrator::operator=(errorintegrator);
  assert(0);
  return *this;
}
EstimatorIntegrator2* EstimatorIntegrator2::clone() const
{
  return new EstimatorIntegrator2(*this);
}
std::string EstimatorIntegrator2::getName() const
{
  return "EstimatorIntegrator2";
}

/*--------------------------------------------------------------------------*/
int EstimatorIntegrator2::getOrderForIntegrationFormula(int orderin, int orderout) const
{
  return 2*orderin+3;
}

/*--------------------------------------------------------------------------*/
void EstimatorIntegrator2::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::Cells);
  // addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::CouplingSides);
  // addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::BoundarySides);
}

/*--------------------------------------------------------------------------*/
Fada::VariableInterface* EstimatorIntegrator2::constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const
{
  Alat::StringVector bouts = Alat::Tokenize(name, "_");
  // assert(bouts.size() == 3);
  // assert(bouts[0] == "Err");
  int ncomp = variablemanager.getInputVariable(bouts[1])->getNComponents();
  // std::string type = bouts[2];
  _unknown_of_postproc[name] = bouts[1];
  return new Fada::Variable(name, ncomp, "H1");
}

/*--------------------------------------------------------------------------*/
void EstimatorIntegrator2::initData()
{
  const Fada::ProblemData* problemdata = getModel()->getProblemData();
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  _solutions.clear();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    std::string var = _unknown_of_postproc[*p];
    std::string varex = var;
    varex += "_exact";
    _solutions[var] = dynamic_cast<const Fada::DataFunctionVector*>( problemdata->getData(varex, "ExactSolution") );
    assert(_solutions[var]);
    assert(getVarInfo()->getNCompIn(var)==_solutions[var]->size());
    _ncomp = getVarInfo()->getNCompIn(var);
    _error[var].set_size( _ncomp );
    _derror[var].set_size( _ncomp );
  }
}

/*--------------------------------------------------------------------------*/
void EstimatorIntegrator2::postProcessCell(AlatEnums::residualstatus& status, double weight) const
{
  double time = getTime();
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    Alat::AssembleVector& floc = (*_flocL)[*p];
    std::string var = _unknown_of_postproc[*p];
    const Fada::FemFunction& u = (*_femfctmapL)["U"][var];
    Alat::DoubleVector& error= _error[var];
    Alat::Vector<Alat::Node>& derror= _derror[var];
    double x = fem(var)->x().x();
    double y = fem(var)->x().y();
    double z = fem(var)->x().z();
    for(int icomp = 0; icomp < _ncomp; icomp++)
    {
      error[icomp] = u(icomp) - ( *_solutions[var] )(icomp) ( x, y, z, time );
      derror[icomp].x() = u.d(icomp).x() - ( *_solutions[var] )(icomp).x( x, y, z, time );
      derror[icomp].y() = u.d(icomp).y() - ( *_solutions[var] )(icomp).y( x, y, z, time );
      derror[icomp].z() = u.d(icomp).z() - ( *_solutions[var] )(icomp).z( x, y, z, time );
    }
    for(int icomp = 0; icomp < getVarInfo()->getNCompOut(*p); icomp++)
    {
      std::string type = getVarInfo()->getPostProcType(*p, icomp);
      if(type == "L2")
      {
        floc(icomp,0) += weight*error[icomp]*error[icomp];
      }
      else if(type == "H1")
      {
        // floc(icomp,0) += weight* arma::dot(derror[icomp],derror[icomp]);
        floc(icomp,0) += weight;
      }
      else if(type == "C0")
      {
        floc(icomp,0) = fmax( floc(icomp,0), fabs(error[icomp]) );
      }
      else if(type == "OS")
      {
        floc(icomp,0) = fmax( floc(icomp,0), ( *_solutions[var] )(icomp) ( x, y, z, time ) );
      }
      else if(type == "US")
      {
        floc(icomp,0) = fmin( floc(icomp,0), ( *_solutions[var] )(icomp) ( x, y, z, time ) );
      }
      else if(type == "L1")
      {
        floc(icomp,0) += weight*fabs(error[icomp]);
      }
      else if(type == "W1")
      {
        for(int idim = 0; idim < getDimension(); idim++)
        {
          floc(icomp,0) += weight* fabs(derror[icomp][idim]);
        }
      }
      else
      {
        std::cerr << "*** ERROR in EstimatorIntegrator2::postProcessCell(): unknown type " << type << "\n";
        assert(0);
        exit(1);
      }
    }
  }
}
