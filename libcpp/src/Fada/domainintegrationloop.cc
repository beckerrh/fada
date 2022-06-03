#include  "FadaMesh/boundaryinfo.h"
#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/initialconditioninterface.h"
#include  "Fada/integrationformulainterface.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratormanager.h"
#include  "Alat/matrixallvariables.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Fada/varinfointegrator.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitorintegrationloop.h"
#include  "Fada/changeintegrationpoint.h"
#include  "Fada/domainintegrationloop.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainIntegrationLoop::~DomainIntegrationLoop(){}
DomainIntegrationLoop::DomainIntegrationLoop() : Fada::DomainIntegrationLoopInterface(), IntegrationLoop(){}
DomainIntegrationLoop::DomainIntegrationLoop( const DomainIntegrationLoop& domainintegrationloop) : Fada::DomainIntegrationLoopInterface(domainintegrationloop), IntegrationLoop(domainintegrationloop)
{
  assert(0);
}
DomainIntegrationLoop& DomainIntegrationLoop::operator=( const DomainIntegrationLoop& domainintegrationloop)
{
  assert(0);
  return *this;
}
std::string DomainIntegrationLoop::getName() const
{
  return "DomainIntegrationLoop";
}
DomainIntegrationLoop* DomainIntegrationLoop::clone() const
{
  assert(0);
  // return new DomainIntegrationLoop(*this);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::basicInit(const Fada::DomainSolverInterface* solver)
{
  _solver = solver;

  IntegrationLoop::basicInit( _solver->getMesh(), _solver->getIntegratorManager(), _solver->getFemManager(), _solver->getFemManagerRight(), _solver->getParameterFile(), _solver->getCouplingType() );

  _variablemanager = _solver->getVariableManager();
  _dofmanagerallvariables = _solver->getDofManagerAllVariables();
  assert(_variablemanager);
  assert(_dofmanagerallvariables);
  assert(_femmanager);
  _localglobal.basicInit( _dofmanagerallvariables, _variablemanager, _solver->getVariablesScales() );
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::setVariablesScales(const Alat::StringDoubleVectorMap& variablescales)
{
  _localglobal.setVariablesScales(variablescales);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::setDataVector(Alat::VectorInterface* data)
{
  _data =  data;
}

/*--------------------------------------------------------------------------*/
const Fada::DomainSolverInterface* DomainIntegrationLoop::getSolver() const
{
  return _solver;
}

const Fada::VariableManager* DomainIntegrationLoop::getVariableManager() const
{
  return _variablemanager;
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::_fillVectorMap(Fada::LocalGlobal::VectorMap& vectormap, FadaEnums::term term, const Alat::SystemVector* u, const Alat::SystemVector* uold, const Alat::SystemVector* uveryold) const
{
  const Alat::SystemVector* uav = dynamic_cast<const Alat::SystemVector*>( u );
  assert(uav);
  vectormap["U"] = uav;
  bool needsveryold = false;
  for(Fada::IntegratorManager::const_iterator p = _integratormanager->begin(term); p != _integratormanager->end(term); p++)
  {
    // std::cerr << (*p)->getName() << " : " << (*p)->needsVeryOld() << "\n";
    if( ( *p )->needsVeryOld() )
    {
      needsveryold = true;
      break;
    }
  }
  const Alat::SystemVector* uoldav = dynamic_cast<const Alat::SystemVector*>( uold );
  if(uoldav)
  {
    vectormap["UOLD"] = uoldav;
  }
  if(needsveryold)
  {
    assert(uoldav);
    const Alat::SystemVector* uveryoldav = dynamic_cast<const Alat::SystemVector*>( uveryold );
    assert(uveryoldav);
    vectormap["UVERYOLD"] = uveryoldav;
  }
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::initSolution(Alat::VectorInterface* u, int level) const
{
  if( _integratormanager->noIntegrator(FadaEnums::Initialize, FadaEnums::Cells) )
  {
    std::cerr << "### DomainIntegrationLoop::initSolution() no integrator !\n";
    return;
  }
  Alat::SystemVector* uav = dynamic_cast<Alat::SystemVector*>( u );
  if(not uav)
  {
    Fada::MultiLevelVector* umg = dynamic_cast< Fada::MultiLevelVector*>( u );
    assert(umg);
    uav = umg->getVector(level);
  }
  else
  {
    assert(level==0);
  }
  assert(uav);
  uav->zeros();
  _mesh->setResolution(level);
  _dofmanagerallvariables->setMeshLevel(level);
  Alat::StringSet formulanames = getIntegrationFormulaeForTermAndDomain(FadaEnums::Initialize, FadaEnums::Cells);
  // std::cerr << "formulanames="<<formulanames<<"\n";
  const Alat::StringDoubleVectorMap& variablesscales = _solver->getVariablesScales();
  for(Alat::StringSet::const_iterator p = formulanames.begin(); p != formulanames.end(); p++)
  {
    const Fada::IntegratorManager::IntegratorSet& integratorset = getIntegratorsForTermAndDomain(FadaEnums::Initialize, FadaEnums::Cells, *p);
    const Fada::IntegrationFormulaInterface* IF = _formulae[*p];
    // std::cerr << "IF="<< IF->getName()<<"\n";
    // assert(0);
    Alat::Node vhat;
    for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integratorset.begin(); it != integratorset.end(); it++)
    {
      const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
      assert(integrator);
      // Alat::IntVector indexvarout( integrator->getVarInfo()->getIndexUnknownVarOut() );
      const Alat::StringSet& varnamesout = integrator->getVarInfo()->getVarNamesOutput();
      Alat::Vector<Alat::Node> F( _mesh->getNNodesPerCell(0) );
      // for(int ivar = 0; ivar < indexvarout.size(); ivar++)
      for(Alat::StringSet::const_iterator p = varnamesout.begin(); p!=varnamesout.end(); p++)
      {
        // int indexvar = indexvarout[ivar];
        // std::string varname = integrator->getVarInfo()->getUnknownOutputName(ivar);
        std::string varname = *p;

        const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(varname);
        Alat::IntVector indices( dofinfo->ndofpercell() );
        Alat::DoubleMatrix Mloc;
        // int ncomp = integrator->getVarInfo()->getNCompOut(ivar);
        int ncomp = integrator->getVarInfo()->getNCompOut(*p);
        Alat::AssembleVector bloc, uloc;
        int ndof = dofinfo->ndof();
        // const Fada::DataInterface* data = integrator->getProblemData(integrator->getVarInfo()->getVarNameOutput(ivar), "InitialCondition");
        const Fada::DataInterface* data = integrator->getProblemData(*p, "InitialCondition");
        if(not data)
        {
          _error_string("initSolution","please define \'InitialCondition\' in ProblemData (in class Application)");
        }
        const Fada::InitialConditionInterface* initial = dynamic_cast<const Fada::InitialConditionInterface*>( data );
        assert(initial);
        // std::cerr << "initial= " << initial->getName() << "\n";
        // assert(0);
        Alat::IntVector count(ndof);
        Alat::DoubleVector uin(ncomp);
        count.zeros();
        for(int iK = 0; iK < _mesh->getNCells(); iK++)
        {
          _mesh->getNodesOfCell(iK, F);
          _femmanager->initCell( F, _mesh->cellIsCurved(iK) );
          dofinfo->getCellIndices(iK, indices);
          int n = indices.size();
          Mloc.set_size(n,n);
          Mloc.zeros();
          bloc.set_size(ncomp, n);

          for(int k = 0; k < IF->n(); k++)
          {
            IF->point(vhat, k);
            double J = _femmanager->reInitReferencePoint(vhat);
            double weight = IF->weight(k)*J;
            const Fada::FemInterface* fem = _femmanager->getFem(*p);
            Alat::Node vphys = fem->x();
            uin.zeros();
            initial->getValue( uin, vphys.x(), vphys.y(), vphys.z() );
            // std::cerr << "uin="<<uin<<" uin*uin="<<uin*uin<< "\n";
            for(int ii = 0; ii < n; ii++)
            {
              for(int jj = 0; jj < n; jj++)
              {
                Mloc (ii, jj) += weight*fem->phi(jj)*fem->phi(ii);
              }
              for(int icomp = 0; icomp < ncomp; icomp++)
              {
                // bloc[icomp][ii] += weight*uin[icomp]*fem->phi(ii);
                bloc(icomp,ii) += weight*uin[icomp]*fem->phi(ii);
              }
            }
          }
          Mloc.gaussJordan();
          uloc.set_size(ncomp, n);
          uloc.zeros();
          for(int icomp = 0; icomp < ncomp; icomp++)
          {
            // Mloc.productVector(uloc[icomp], bloc[icomp]);
            for(int ii = 0; ii < n; ii++)
            {
              for(int jj = 0; jj < n; jj++)
              {
                uloc(icomp,ii) += Mloc(ii,jj) * bloc(icomp,jj);
              }
            }
          }
          for(int ii = 0; ii < n; ii++)
          {
            count[indices[ii]]++;
          }
          // Alat::DoubleVector scaleinv(ncomp);
          // for(int icomp = 0; icomp < ncomp; icomp++)
          // {
          //   scaleinv[icomp] = 1.0/variablesscales[varname][icomp];
          // }
          // uav->getVector(varname)->assemble( uloc, indices, 1.0/variablesscales[varname] );
          // uav->getVector(varname)->assemble( uloc, indices, scaleinv );
          uav->getVector(varname)->assemble( uloc, indices, variablesscales[varname] );
        }
        uav->getVector(varname)->scaleIntVector(count);
      }
    }
  }
  // std::cerr << "DomainIntegrationLoop::initSolution() u="<< umg->getVector(0)->writeAscii(std::cerr)<<"\n";
  // assert(0);
  // std::cerr << "@@\n";
  // umg->getVector(0)->writeAscii(std::cerr);
  // std::cerr << "@@\n";
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface* f, int level) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  Fada::LocalGlobal::VectorMap vectormap;
  Fada::VisitorIntegrationLoopRightHandSide visitor(_localglobal, _variablemanager, fs);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  // assert(_datavector);
  _mesh->setResolution(level);
  _dofmanagerallvariables->setMeshLevel(level);

  Fada::FemFunctionsMap femfctmap, femfctmapR;

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap);
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap);
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR);
  // std::cerr << "DomainIntegrationLoop::constructRightHandSide() ";
  // f->writeAscii(std::cerr);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* bdf, double d, int level) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  Fada::LocalGlobal::VectorMap vectormap;
  const Alat::SystemVector* uav = dynamic_cast<const Alat::SystemVector*>( u );
  const Alat::SystemVector* bdfav = dynamic_cast<const Alat::SystemVector*>( bdf );
  assert(uav);
  vectormap["U"] = uav;
  assert(bdfav);
  vectormap["BDF"] = bdfav;
  Fada::VisitorIntegrationLoopTimeRhs visitor(_localglobal, _variablemanager, fs, d);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  // assert(_datavector);
  _dofmanagerallvariables->setMeshLevel(level);

  assert( _integratormanager->hasIntegrator(FadaEnums::TimeRHS, FadaEnums::Cells) );
  Fada::FemFunctionsMap femfctmap(vectormap), femfctmapR(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap);
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap);
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR);

  // std::cerr << "2DomainIntegrationLoop::integrateTimeRhs() f=";
  // fs->writeAscii(std::cerr);
  // std::cerr << "\n";
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
  assert(us);
  const Alat::SystemVector* uolds = dynamic_cast<const Alat::SystemVector*>( uold );
  const Alat::SystemVector* uveryolds = dynamic_cast<const Alat::SystemVector*>( uveryold );
  Fada::VisitorIntegrationLoopForm visitor(_localglobal, _variablemanager, fs);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  _fillVectorMap(vectormap, visitor.getTerm(), us, uolds, uveryolds);

  Fada::FemFunctionsMap femfctmap(vectormap), femfctmapR(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap);
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap);
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* du, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
  assert(us);
  const Alat::SystemVector* dus = dynamic_cast<const Alat::SystemVector*>( du );
  assert(dus);
  const Alat::SystemVector* uolds = dynamic_cast<const Alat::SystemVector*>( uold );
  const Alat::SystemVector* uveryolds = dynamic_cast<const Alat::SystemVector*>( uveryold );
  Fada::LocalGlobal::VectorMap vectormap;
  Fada::VisitorIntegrationLoopLinearization visitor(_localglobal, _variablemanager, fs);
  _fillVectorMap(vectormap, visitor.getTerm(), us, uolds, uveryolds);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  _dofmanagerallvariables->setMeshLevel(level);
  vectormap["DU"] = dus;

  Fada::FemFunctionsMap femfctmap(vectormap), femfctmapR(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap);
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap);
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface* u, const Alat::VectorInterface* du, int level) const
{
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
  assert(us);
  Fada::VisitorIntegrationLoopComputeNormSquared visitor(_localglobal, _variablemanager, norms);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  const Alat::SystemVector* uav = dynamic_cast<const Alat::SystemVector*>( u );
  assert(uav);
  vectormap["U"] = uav;
  const Alat::SystemVector* duav = dynamic_cast<const Alat::SystemVector*>( du );
  assert(duav);
  vectormap["DU"] = duav;
  assert( _integratormanager->hasIntegrator(FadaEnums::ComputeNormSquared, FadaEnums::Cells) );

  Fada::FemFunctionsMap femfctmap(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, int level) const
{
  if( not _integratormanager->hasIntegrator(FadaEnums::PostProcess) )
  {
    return;
  }
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
  const Alat::SystemVector* uolds;
  Alat::SystemVector* fs;
  if(not us)
  {
    assert(level>=0);
    const Fada::MultiLevelVector* umg = dynamic_cast<const Fada::MultiLevelVector*>( u );
    assert(umg);
    us = umg->getVector(level);
    const Fada::MultiLevelVector* uoldmg = dynamic_cast<const Fada::MultiLevelVector*>( uold );
    assert(uoldmg);
    uolds = uoldmg->getVector(level);
    Fada::MultiLevelVector* fmg = dynamic_cast< Fada::MultiLevelVector*>( f );
    assert(fmg);
    assert(umg->nlevels()==fmg->nlevels());
    fs = fmg->getVector(level);
    std::cerr << "DomainIntegrationLoop::postProcess() nlevels = " << umg->nlevels() << " level="<<level<<"\n";
  }
  else
  {
    assert(level==0);
    uolds = dynamic_cast<const Alat::SystemVector*>( uold );
    fs = dynamic_cast<Alat::SystemVector*>( f );
  }
  assert(us);
  assert(fs);
  // std::cerr << "f\n";
  // fs->writeAscii(std::cerr);
  // std::cerr << "u\n";
  // us->writeAscii(std::cerr);

  const Alat::SystemVector* uveryolds = NULL;
  Fada::VisitorIntegrationLoopPostProcess visitor(_localglobal, _variablemanager, fs);
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  _fillVectorMap(vectormap, FadaEnums::PostProcess, us, uolds, uveryolds);

  Fada::FemFunctionsMap femfctmap(vectormap), femfctmapR(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap );
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap );
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR );

  // std::cerr << "AFTER f\n";
  // fs->writeAscii(std::cerr);
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::MatrixInterface* A, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level) const
{
  Fada::MultiLevelMatrix* Amg = dynamic_cast<Fada::MultiLevelMatrix*>( A );
  if(Amg)
  {
    const Fada::MultiLevelVector* umg = dynamic_cast<const Fada::MultiLevelVector*>( u );
    assert(umg);
    const Fada::MultiLevelVector* uoldmg = dynamic_cast<const Fada::MultiLevelVector*>( uold );
    const Fada::MultiLevelVector* uveryoldmg = dynamic_cast<const Fada::MultiLevelVector*>( uveryold );
    const Alat::SystemVector* uolds = NULL, * uveryolds = NULL;

    int nlevels = Amg->nlevels();
    for(int level = 0; level < nlevels; level++)
    {
      if(uoldmg)
      {
        uolds = uoldmg->getVector(level);
      }
      if(uveryoldmg)
      {
        uveryolds = uveryoldmg->getVector(level);
      }
      constructJacobianMatrix(level, status, Amg->getMatrix(level), umg->getVector(level), uolds, uveryolds);
    }
  }
  else
  {
    Alat::SystemMatrixInterface* As = dynamic_cast<Alat::SystemMatrixInterface*>( A );
    int level = 0;
    const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
    assert(us);
    const Alat::SystemVector* uolds = dynamic_cast<const Alat::SystemVector*>( uold );
    const Alat::SystemVector* uveryolds = dynamic_cast<const Alat::SystemVector*>( uveryold );
    constructJacobianMatrix(level, status, As, us, uolds, uveryolds);
    // std::cerr << "DomainIntegrationLoop::constructJacobianMatrix(AlatEnums::residualstatus& status, )  A=";
    // As->write(std::cerr) << "\n";
  }
}

/*--------------------------------------------------------------------------*/
void DomainIntegrationLoop::constructJacobianMatrix(int level, AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* A, const Alat::SystemVector* u, const Alat::SystemVector* uold, const Alat::SystemVector* uveryold) const
{
  Fada::VisitorIntegrationLoopMatrix visitor(_localglobal, _variablemanager, A, _couplingtype);
  // _datavector = dynamic_cast<const Alat::SystemVector*>( _datamg->getVector(level) );
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  _fillVectorMap( vectormap, FadaEnums::Matrix, u, uold, uveryold );

  Fada::FemFunctionsMap femfctmap(vectormap), femfctmapR(vectormap);

  integratePerLevelCells(status, visitor, level, vectormap, femfctmap );
  integratePerLevelBoundarySides(status, visitor, level, vectormap, femfctmap );
  integratePerLevelInteriorSides(status, visitor, level, vectormap, femfctmap, femfctmapR );
}

/*--------------------------------------------------------------------------*/
template<class VISITOR>
void DomainIntegrationLoop::integratePerLevelCells(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const
{
  FadaEnums::term term = visitor.getTerm();
  FadaEnums::domain domain = FadaEnums::Cells;
  if( not _integratormanager->hasIntegrator(term, domain) )
  {
    return;
  }
  Alat::Vector<Alat::Node> F( _mesh->getNNodesPerCell(0) );
  _mesh->setResolution(level);
  _localglobal.initLocalVectors(_localvectors, vectormap);
  Alat::StringSet formulanames = getIntegrationFormulaeForTermAndDomain(term, domain);
  const Fada::IntegratorManager::IntegratorSet& integrators = _integratormanager->getIntegratorsForTermAndDomain(term, domain);
  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = *it;
    // std::cerr << "DomainIntegrationLoop::integratePerLevelCells() femfctmap = " << femfctmap << "\n";
    integrator->setFemFunctions(femfctmap);
    visitor.setAssembleData(integrator);
    integrator->beforeIntegrationLoop(term, domain, _localglobal, _localvectors, _localvectorsR, vectormap, _datavector);
  }
  Alat::Node vhat;
  for(int iK = 0; iK < _mesh->getNCells(); iK++)
  {
    _mesh->getNodesOfCell(iK, F);
    _femmanager->initCell( F, _mesh->cellIsCurved(iK) );
    _localglobal.extractAll(_localvectors, iK, vectormap, _datavector);
    visitor.setOuputZero();
    for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
    {
      const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
      assert(integrator);
      integrator->beforeIntegration(_localvectors);
      visitor.perCellBeforeIntegration(status, iK, integrator, _localvectors );
    }
    for(Alat::StringSet::const_iterator p = formulanames.begin(); p != formulanames.end(); p++)
    {
      const Fada::IntegratorManager::IntegratorSet& integratorsforformula = getIntegratorsForTermAndDomain(term, domain, *p);
      const Fada::IntegrationFormulaInterface* IF = _formulae[*p];
      for(int k = 0; k < IF->n(); k++)
      {
        IF->point(vhat, k);
        double J = _femmanager->reInitReferencePoint(femfctmap, vhat, _localvectors);
        double weight = IF->weight(k)*J;
        for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integratorsforformula.begin(); it != integratorsforformula.end(); it++)
        {
          const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
          assert(integrator);
          visitor.perCell(status, iK, integrator, weight, femfctmap );
        }
      }
    }
    Alat::StringSet variables;
    for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
    {
      const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
      assert(integrator);
      const Fada::VarInfoIntegrator* varInfo = integrator->getVarInfo();
      variables.insert( varInfo->getVarNamesOutput().begin(), varInfo->getVarNamesOutput().end() );
    }
    visitor.assembleOutput(iK, variables);
  }
  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = *it;
    integrator->afterIntegrationLoop( term, domain, visitor.getOutputVector() );
  }
}

/*--------------------------------------------------------------------------*/
template<class VISITOR>
void DomainIntegrationLoop::integratePerLevelBoundarySides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const
{
  FadaEnums::term term = visitor.getTerm();
  FadaEnums::domain domain = FadaEnums::BoundarySides;
  if( not _integratormanager->hasIntegrator(term, domain ) )
  {
    return;
  }
  Alat::Vector<Alat::Node> F( _mesh->getNNodesPerCell(0) );
  _mesh->setResolution(level);
  _localglobal.initLocalVectors(_localvectors, vectormap);

  const FadaMesh::BoundaryInfo* boundaryinfo = _mesh->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();

  Alat::StringSet formulanames = getIntegrationFormulaeForTermAndDomain(term, domain);
  const Fada::IntegratorManager::IntegratorSet& integrators = _integratormanager->getIntegratorsForTermAndDomain(term, domain);

  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = *it;
    integrator->beforeIntegrationLoop(term, domain, _localglobal, _localvectors, _localvectorsR, vectormap, _datavector);
    integrator->setFemFunctions(femfctmap);
    visitor.setAssembleData(integrator);
  }
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
    const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
    const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
    for(int i = 0; i < sides.size(); i++)
    {
      int iS = sides[i];
      int iK = cells[i];
      int iil = sideids[i];

      _mesh->getNodesOfCell(iK, F);
      _femmanager->initCellAndSide( F, iil, _mesh->cellIsCurved(iK) );
      _localglobal.extractAll(_localvectors, iK, vectormap, _datavector);
      visitor.setOuputZero();
      for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
      {
        const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
        assert(integrator);
        integrator->beforeIntegration(_localvectors);
        visitor.perBoundarySideBeforeIntegration(status, iS, integrator, _localvectors, color );
      }
      for(Alat::StringSet::const_iterator p = formulanames.begin(); p != formulanames.end(); p++)
      {
        const Fada::IntegratorManager::IntegratorSet& integratorset = getIntegratorsForTermAndDomain(term, domain, *p);
        const Fada::IntegrationFormulaInterface* IF = _formulae[*p];
        Alat::Node vhat;
        for(int k = 0; k < IF->n(); k++)
        {
          IF->point(vhat, k);
          double G = _femmanager->reInitReferencePointBoundary( femfctmap, vhat, _localvectors);
          double weight = IF->weight(k)*G;
          for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integratorset.begin(); it != integratorset.end(); it++)
          {
            const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
            assert(integrator);
            visitor.perBoundarySide(status, iS, iK, integrator, weight, femfctmap, color );
          }
        }
      }
      Alat::StringSet variables;
      for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
      {
        const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
        assert(integrator);
        const Fada::VarInfoIntegrator* varInfo = integrator->getVarInfo();
        variables.insert( varInfo->getVarNamesOutput().begin(), varInfo->getVarNamesOutput().end() );
      }
      visitor.assembleOutput(iK, variables);
    }
  }
  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = *it;
    integrator->afterIntegrationLoop( term, domain, visitor.getOutputVector() );
  }
}

/*--------------------------------------------------------------------------*/
template<class VISITOR>
void DomainIntegrationLoop::integratePerLevelInteriorSides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmapL, Fada::FemFunctionsMap& femfctmapR) const
{
  FadaEnums::term term = visitor.getTerm();
  FadaEnums::domain domain = FadaEnums::InteriorSides;
  if( not _integratormanager->hasIntegrator(term, domain) )
  {
    return;
  }
  assert(_femmanagerR);

  Alat::Vector<Alat::Node> Fl( _mesh->getNNodesPerCell(0) ), Fr( _mesh->getNNodesPerCell(0) );
  _mesh->setResolution(level);

  _localglobal.initLocalVectors(_localvectors, vectormap);
  _localglobal.initLocalVectors(_localvectorsR, vectormap);

  Alat::StringSet formulanames = getIntegrationFormulaeForTermAndDomain(term, domain);
  const Fada::IntegratorManager::IntegratorSet& integrators = _integratormanager->getIntegratorsForTermAndDomain(term, domain);

  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
    assert(integrator);
    integrator->beforeIntegrationLoop(term, domain, _localglobal, _localvectors, _localvectorsR, vectormap, _datavector);
    integrator->setFemFunctions(femfctmapL, femfctmapR);
    visitor.setAssembleData(integrator);
  }
  for(int iS = 0; iS < _mesh->getNSides(); iS++)
  {
    int iKl = _mesh->getCellIdOfSide(iS, 0);
    int iKr = _mesh->getCellIdOfSide(iS, 1);
    if(iKr == -1)
    {
      continue;
    }
    int iil = _mesh->getLocalIndexOfSideInCell(iKl, iS);
    int iir = _mesh->getLocalIndexOfSideInCell(iKr, iS);
    _mesh->getNodesOfCell(iKl, Fl);
    _mesh->getNodesOfCell(iKr, Fr);
    int offset = _mesh->getCouplingOffset(iS);
    _femmanager->initCellAndSide( Fl, iil, _mesh->cellIsCurved(iKl) );
    _femmanagerR->initCellAndSide( Fr, iir, _mesh->cellIsCurved(iKr) );
    _localglobal.extractAll(_localvectors, iKl, vectormap, _datavector);
    _localglobal.extractAll(_localvectorsR, iKr, vectormap, _datavector);
    visitor.setOuputZeroAll();
    for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
    {
      const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
      assert(integrator);
      integrator->beforeIntegration(_localvectors, _localvectorsR);
      visitor.perCouplingSideBeforeIntegration(status, iS, integrator, _localvectors, _localvectorsR);
    }

    for(Alat::StringSet::const_iterator p = formulanames.begin(); p != formulanames.end(); p++)
    {
      const Fada::IntegratorManager::IntegratorSet& integratorset = getIntegratorsForTermAndDomain(term, domain, *p);
      const Fada::IntegrationFormulaInterface* IF = _formulae[*p];
      Alat::Node vhat, vhatr;
      for(int k = 0; k < IF->n(); k++)
      {
        IF->point(vhat, k);

        Fada::changeIntegrationPoint(vhatr, vhat, offset);
        double G = _femmanager->reInitReferencePointBoundary(femfctmapL, vhat, _localvectors);
        _femmanagerR->reInitReferencePointBoundary(femfctmapR, vhatr, _localvectorsR);
        double weight = IF->weight(k)*G;
        for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integratorset.begin(); it != integratorset.end(); it++)
        {
          const Fada::IntegratorInterface* integrator = *it;
          visitor.perCouplingSide( status, iKl, iKr, integrator, weight, G, femfctmapL, femfctmapR );
        }
      }
    }
    Alat::StringSet variables;
    for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
    {
      const Fada::IntegratorInterface* integrator = dynamic_cast<const Fada::IntegratorInterface*>( *it );
      assert(integrator);
      const Fada::VarInfoIntegrator* varInfo = integrator->getVarInfo();
      variables.insert( varInfo->getVarNamesOutput().begin(), varInfo->getVarNamesOutput().end() );
    }
    visitor.assembleOutput(iKl, iKr, variables, variables);
  }
  for(Fada::IntegratorManager::IntegratorSet::const_iterator it = integrators.begin(); it != integrators.end(); it++)
  {
    const Fada::IntegratorInterface* integrator = *it;
    integrator->afterIntegrationLoop( term, domain, visitor.getOutputVector() );
  }
}
