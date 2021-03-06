#include  "Alat/assemblevector.h"
#include  "Alat/systemassemblevector.h"
#include  "Alat/tokenize.h"
#include  "errorestimatorintegrator.h"
#include  "Fada/feminterface.h"
#include  "Fada/modelinterface.h"
#include  "Fada/problemdata.h"
#include  "Fada/variable.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
ErrorEstimatorIntegrator::~ErrorEstimatorIntegrator()
{}
ErrorEstimatorIntegrator::ErrorEstimatorIntegrator() : Fada::Integrator()
{}
ErrorEstimatorIntegrator::ErrorEstimatorIntegrator( const ErrorEstimatorIntegrator& errorestimatorintegrator) : Fada::Integrator(errorestimatorintegrator)
{
  assert(0);
}

ErrorEstimatorIntegrator& ErrorEstimatorIntegrator::operator=( const ErrorEstimatorIntegrator& errorestimatorintegrator)
{
  assert(0);
  Fada::Integrator::operator=(errorestimatorintegrator);
  return *this;
}
std::string ErrorEstimatorIntegrator::getName() const
{
  return "ErrorEstimatorIntegrator";
}

std::string ErrorEstimatorIntegrator::getClassName() const
{
  return "ErrorEstimatorIntegrator";
}

/*--------------------------------------------------------------------------*/
void ErrorEstimatorIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::InteriorSides);
  addDomainsOfTerm(FadaEnums::PostProcess, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/
void ErrorEstimatorIntegrator::initData()
{
  Fada::Integrator::initData();
  const Fada::ProblemData* problemdata = getModel()->getProblemData();
  _rhs = dynamic_cast<const Fada::RightHandSideInterface*>(problemdata->getData("U", "RightHandSide"));
  assert(_rhs);
  _frhs.set_size(1);
}

/*--------------------------------------------------------------------------*/
Fada::VariableInterface* ErrorEstimatorIntegrator::constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const
{
  int ncomp = 2;
  std::string fem = "dgp02d";
  std::string visutype = "cell";
  Fada::Variable* var = new Fada::Variable(name, fem, ncomp, visutype);
  _varnameout = name;
  Alat::StringVector names2 = Alat::Tokenize(name, "_");
  assert(names2.size()==2);
  _varnamein = names2[1];
  Alat::StringVector names = Alat::Tokenize(name, "|");
  if(names.size() == ncomp)
  {
    for(int icomp = 0; icomp < ncomp; icomp++)
    {
      var->setVisualizationName(icomp, names[icomp]);
    }
  }
  return var;
}

/*--------------------------------------------------------------------------*/
void ErrorEstimatorIntegrator::postProcessCell(AlatEnums::residualstatus& status, double weight) const
{
  Alat::AssembleVector& floc = ( *_flocL )[_varnameout];
  const Fada::FemFunction& u = ( *_femfctmapL )["U"][_varnamein];
  // std::cerr << "ErrorEstimatorIntegrator::postProcessCell() empty\n";
  double h = fem(_varnamein)->getHOfCell();
  double x = fem(_varnamein)->x().x();
  double y = fem(_varnamein)->x().y();
  double z = fem(_varnamein)->x().z();
  if(_rhs == NULL)
  {
    _frhs.fill(0.0);
  }
  else
  {
    _rhs->getValue( _frhs, x, y, z, getTime() );
  }
  // std::cerr << "_frhs="<<_frhs;
  floc(1, 0) += weight * h*h * _frhs[0]* _frhs[0];
  // floc(1, 0) = h*h;
  // assert(0);
}

/*--------------------------------------------------------------------------*/
void ErrorEstimatorIntegrator::postProcessCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  // const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  // assert(outvars.size()==1);
  // const std::string& outvar = *outvars.begin();
  // std::cerr << "outvars="<<outvars<<"\n";
  // std::cerr << "_varnamein="<<_varnamein<<"\n";
  // std::cerr << "_varnameout="<<_varnameout<<"\n";
  Alat::AssembleVector& flocL = ( *_flocL )[_varnameout];
  Alat::AssembleVector& flocR = ( *_flocR )[_varnameout];
  // std::string var = _unknown_of_postproc[_varname];
  const Fada::FemFunction& uL = ( *_femfctmapL )["U"][_varnamein];
  const Fada::FemFunction& uR = ( *_femfctmapR )["U"][_varnamein];
  double h = fem(_varnamein)->getHOfCell();
  const Alat::Node& normal = fem(_varnamein)->getNormal();
  double jumpn = 0.5*( arma::dot(uL.d(0),normal)-arma::dot(uR.d(0),normal) );
  // weight = omega*|S| !
  double eta = h*jumpn*jumpn*weight;
  flocL(0, 0) += eta;
  flocR(0, 0) += eta;
}
