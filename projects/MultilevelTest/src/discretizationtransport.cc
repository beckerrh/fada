#include  "discretizationtransport.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/quadtotri.h"
#include  <cassert>
#include  "Fada/femcgp12d.h"
#include  "Fada/integrationformulaline.h"
#include  "Fada/datainterface.h"
#include  "Fada/modelinterface.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/integrationformulatriangle.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Fada/visitorintegrationloop.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/righthandsideinterface.h"
#include  "Fada/variablemanager.h"
#include  "Fada/boundarymanager.h"
#include  "Fada/dirichletinterface.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "Fada/integratormanager.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/systemvector.h"
#include  "Alat/variablevectorinterface.h"

/*--------------------------------------------------------------------------*/
DiscretizationTransport::~DiscretizationTransport() {}
DiscretizationTransport::DiscretizationTransport(): Fada::UnStructuredIntegrationLoop(){}
DiscretizationTransport::DiscretizationTransport( const DiscretizationTransport& discretizationtransport): Fada::UnStructuredIntegrationLoop(discretizationtransport)
{
(*this).operator=(discretizationtransport);
}
DiscretizationTransport& DiscretizationTransport::operator=( const DiscretizationTransport& discretizationtransport)
{
Fada::UnStructuredIntegrationLoop::operator=(discretizationtransport);
assert(0);
return *this;
}
std::string DiscretizationTransport::getName() const
{
return "DiscretizationTransport";
}
DiscretizationTransport* DiscretizationTransport::clone() const
{
  assert(0);
// return new DiscretizationTransport(*this);
}

/*--------------------------------------------------------------------------*/
void DiscretizationTransport::basicInit(const Fada::DomainSolverInterface* solver)
{
  Fada::UnStructuredIntegrationLoop::basicInit(solver);

  _model = solver->getModel();
  _mmesh =dynamic_cast<const FadaMesh::MultiLevelMesh*>(solver->getMesh());
  assert(_mmesh);

  _IFside = new Fada::LineGauss2();
  // _IFside = new Fada::LineMidPoint();
  _IFcell = new Fada::TriangleRotatedTrapez();

  int nlevels = _mmesh->getNLevels();
  _discdata.set_size(nlevels);
  for(int level=0;level<nlevels;level++)
  {
    const FadaMesh::QuadToTri* quadtotri = dynamic_cast<const FadaMesh::QuadToTri*>(_mmesh->getMesh(level));
    assert(quadtotri);
    quadtotri->constructPatchInfo(_discdata[level].patchinfo);
    _discdata[level].fem = new  Fada::FemCgP12d();
    _discdata[level].fem->basicInit(quadtotri);
    computeArea(quadtotri, _discdata[level]);
    arma::field<arma::vec>& patchdiff = _discdata[level].patchdiff;
    int nnodes = quadtotri->getNNodes();
    patchdiff.set_size(nnodes);
    int npmax=0;
    for(int iN = 0; iN < nnodes; iN++)
    {
      int npatch = _discdata[level].patchinfo.nodes[iN].size();
      patchdiff[iN].set_size(npatch);
      if (npatch> npmax)
      {
        npmax = npatch;
      }
    }
    _discdata[level].npatchmax = npmax;
  }
  // Problem Data
  const Fada::VariablesMap& unknowns = solver->getVariableManager()->getUnknowns();
  int nvars = unknowns.size();
  assert(nvars == 1);
  _variablenames.set_size(nvars);
  std::string varname = unknowns.begin()->first;
  _variablenames[0] = varname;
  int ncomp = unknowns.begin()->second->getNComponents();
  _help1.set_size(nvars);
  _help1[0].set_size(ncomp);
  _help2.set_size(nvars);
  _help2[0].set_size(ncomp);
  _righthandside.set_size(nvars);
  _dirichlet.set_size(nvars);
  _betafct = _model->getData("beta");
  assert(_betafct);
  const Fada::DataInterface* data = _model->getProblemData(varname, "RightHandSide");
  const Fada::RightHandSideInterface* righthandside = dynamic_cast<const Fada::RightHandSideInterface*>( data );
  _righthandside[0] = righthandside;

  const FadaMesh::BoundaryInfo* boundaryinfo = _mmesh->getMesh(0)->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  for(int level=1;level<nlevels;level++)
  {
    const Alat::IntVector colors2 = _mmesh->getMesh(level)->getBoundaryInfo()->getColors();
    assert(colors2==colors);
  }
  int ncolors = colors.size();
  _dirichlet[0].set_size(ncolors);
  for(int ic = 0; ic < ncolors; ic++)
  {
    int color = colors[ic];
    std::string bdrycond = solver->getBoundaryManager()->boundaryCondition(varname, color);
    const Fada::DataInterface* data = _model->getProblemData(varname, bdrycond);
    _dirichlet[0][ic] = dynamic_cast<const Fada::DirichletInterface*>( data );
    assert(_dirichlet[0][ic]);
  }
  for(int level=0;level<nlevels;level++)
  {
    _mmesh->setResolution(level);
    integrateBetan(_mmesh->getMesh(level), _discdata[level]);
    computeDownwind(_mmesh->getMesh(level), _discdata[level]);
    computePatchDiff(_mmesh->getMesh(level), _discdata[level]);
  }
}
/*--------------------------------------------------------------------------*/
void DiscretizationTransport::computeArea(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const
{
  Alat::DoubleVector& area = discdata.area;
  Fada::FemInterface* fem = discdata.fem;

  int ncells = mesh->getNCells();
  area.set_size(ncells);
  Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  Alat::Node vhat;
  for(int iC = 0; iC < ncells; iC++)
  {
    mesh->getNodesOfCell(iC, Fl);
    fem->reInitCell( Fl);
    double w = 0.0;
    for(int k = 0; k < _IFcell->n(); k++)
    {
      _IFcell->point(vhat, k);
      fem->reInitReferencePoint( vhat);
      double weight = _IFcell->weight(k)*fem->J();
      w += weight;
    }
    area[iC]=w;
  }
}

/*--------------------------------------------------------------------------*/
void DiscretizationTransport::computeDownwind(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const
{
  const Alat::DoubleVector& area = discdata.area;
  arma::cube& downwindcoef = discdata.downwindcoef;
  const Alat::Vector<Alat::Node>& beta = discdata.betacell;

  int ncells = mesh->getNCells();
  downwindcoef.zeros(ncells, 2, 3);
  // coef.set_size(ncells, 2);
  // ind.set_size(ncells, 2);
  arma::mat A(2, 2);
  arma::vec x(2), b(2);
  for(int iC = 0; iC < ncells; iC++)
  {
    Alat::Node NC = mesh->getNodeOfCell(iC);
    bool found = false;
    int i0, i1;
    for(int ii=0;ii<mesh->getNSidesPerCell(iC);ii++)
    {
      int iS = mesh->getSideIdOfCell(iC, ii);
      int nnodesside = mesh->getNNodesPerSide(iS);
      assert(nnodesside==2);
      i0 = mesh->getNodeIdOfSide(iS,0);
      i1 = mesh->getNodeIdOfSide(iS,1);
      Alat::Node N0 = mesh->getNode(i0);
      Alat::Node N1 = mesh->getNode(i1);
      A(0, 0) = beta[iC].x();
      A(1, 0) = beta[iC].y();
      A(0, 1) = N1.x() - N0.x();
      A(1, 1) = N1.y() - N0.y();
      b[0] = N1.x() - NC.x();
      b[1] = N1.y() - NC.y();
      x = arma::solve(A, b);
      // std::cerr << "NC="<<NC << " beta="<<beta[iC] << " x="<< x;
      // x[0] = delta !!
      // x[1] est le coefficient de N0 pour x^*
      if (x[0]>=0 and x[1] >= 0 and x[1] <= 1)
      {
        found = true;
        // coef(iC, 0) = x[0];
        // coef(iC, 1) = x[1];
        // ind(iC, 0) = i0;
        // ind(iC, 1) = i1;
        break;
      }
    }
    assert(found);
    assert(mesh->getNNodesPerCell(iC)==3);
    for(int ii=0;ii<3;ii++)
    {
      int iN = mesh->getNodeIdOfCell(iC, ii);
      if(iN==i0)
      {
        downwindcoef(iC, 0, ii) = (x[1] - 1.0/3.0)/x[0];
        downwindcoef(iC, 1, ii) = x[1]*area[iC];
      }
      else if(iN==i1)
      {
        downwindcoef(iC, 0, ii) = (2.0/3.0 - x[1])/x[0];
        downwindcoef(iC, 1, ii) = (1.0-x[1])*area[iC];
      }
      else
      {
        downwindcoef(iC, 0, ii) = -1.0/3.0/x[0];
        downwindcoef(iC, 1, ii) = 0.0;
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DiscretizationTransport::integrateBetan(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const
{
  Fada::FemInterface* fem =discdata.fem;
  arma::vec& betan = discdata.betaside;
  Alat::Vector<Alat::Node>& betac = discdata.betacell;

  int nsides = mesh->getNSides();
  betan.zeros(nsides);
  Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  Alat::Node vhat;
  Alat::Node beta;
  for(int iS = 0; iS < nsides; iS++)
  {
    int iKl = mesh->getCellIdOfSide(iS, 0);
    int iil = mesh->getLocalIndexOfSideInCell(iKl, iS);
    mesh->getNodesOfCell(iKl, Fl);
    fem->reInitCellAndSide( Fl, iil);
    for(int k = 0; k < _IFside->n(); k++)
    {
      _IFside->point(vhat, k);
      fem->reInitReferencePointBoundary( vhat);
      double weight = _IFside->weight(k)*fem->G();
      _betafct->getValue(beta, fem->x());
      betan[iS] += weight* arma::dot(beta,fem->getNormal());
      // std::cerr << "x = " << fem->x()  << " n = " << fem->getNormal() << " beta " << beta << " bn " <<beta*fem->getNormal() <<"\n";
    }
  }
  int ncells = mesh->getNCells();
  betac.set_size(ncells);
  for(int iC = 0; iC < ncells; iC++)
  {
    mesh->getNodesOfCell(iC, Fl);
    fem->reInitCell( Fl);
    double w = 0.0;
    for(int k = 0; k < _IFcell->n(); k++)
    {
      _IFcell->point(vhat, k);
      fem->reInitReferencePoint( vhat);
      double weight = _IFcell->weight(k)*fem->J();
      w += weight;
      _betafct->getValue(beta, fem->x());
      betac[iC] += weight * beta;
    }
    betac[iC] *= 1./w;
  }
  // std::cerr << "betac " << betac << "\n";
}


/*--------------------------------------------------------------------------*/
void DiscretizationTransport::matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* sparsematrix) const
{
  Alat::SparsityPatternSoft sparsitypatternsoft(n);
  assert(_mmesh);
  // std::cerr << "level="<<level << " _mmesh "<< _mmesh->getNLevels() << "\n";
  assert(_mmesh->getMesh(level));
  matrixSparsityPattern(varnamei, varnamej, sparsitypatternsoft, _mmesh->getMesh(level), _discdata[level]);
  sparsematrix->set_size(sparsitypatternsoft);
  sparsematrix->zeros();
}


/*--------------------------------------------------------------------------*/
void DiscretizationTransport::constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface* f) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  int level = 0;
  Fada::LocalGlobal::VectorMap vectormap;
  Fada::VisitorIntegrationLoopRightHandSide visitor(_localglobal, _variablemanager, fs);
  _dofmanagerallvariables->setMeshLevel(level);

  rightHandSideCells(status, fs, _mmesh->getMesh(level), _discdata[level]);
  rightHandSideBoundarySides(status, fs, _mmesh->getMesh(level), _discdata[level]);
}
/*--------------------------------------------------------------------------*/
void DiscretizationTransport::constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold) const
{
  Alat::SystemVector* fs = dynamic_cast<Alat::SystemVector*>( f );
  assert(fs);
  const Alat::SystemVector* us = dynamic_cast<const Alat::SystemVector*>( u );
  assert(us);
  const Alat::SystemVector* uolds = dynamic_cast<const Alat::SystemVector*>( uold );
  const Alat::SystemVector* uveryolds = dynamic_cast<const Alat::SystemVector*>( uveryold );
  int level = 0;
  Fada::VisitorIntegrationLoopForm visitor(_localglobal, _variablemanager, fs);
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  _fillVectorMap(vectormap, visitor.getTerm(), us, uolds, uveryolds);

  formCells(status, fs, us, _mmesh->getMesh(level), _discdata[level]);
  formBoundarySides(status, fs, us, _mmesh->getMesh(level), _discdata[level]);
}
/*--------------------------------------------------------------------------*/
void DiscretizationTransport::constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::MatrixInterface* A, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold) const
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
    // assert(0);
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
void DiscretizationTransport::constructJacobianMatrix(int level, AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVector* us, const Alat::SystemVector* uold, const Alat::SystemVector* uveryold) const
{
  Fada::VisitorIntegrationLoopMatrix visitor(_localglobal, _variablemanager, As, _couplingtype);
  _dofmanagerallvariables->setMeshLevel(level);
  Fada::LocalGlobal::VectorMap vectormap;
  _fillVectorMap( vectormap, FadaEnums::Matrix, us, uold, uveryold );

  matrixCells(status, As, us, _mmesh->getMesh(level), _discdata[level]);
  matrixBoundarySides(status, As, us, _mmesh->getMesh(level), _discdata[level]);
  // As->write(std::cerr);
  // std::cerr  << "\n";
  // As->zeros();
  // integratePerLevelBoundarySides(status, visitor, level, vectormap );
  // As->write(std::cerr);
  // std::cerr  << "\n";

    // std::cerr << "DomainIntegrationLoop::constructJacobianMatrix(AlatEnums::residualstatus& status, )  A=";
    // As->write(std::cerr) << "\n";
}
