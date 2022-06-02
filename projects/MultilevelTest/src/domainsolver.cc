#include  "domainsolver.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/domainmodelinterface.h"
#include  "Fada/unstructuredintegrationloop.h"
#include  "Fada/multilevelvector.h"
// #include  "Fada/multigridtransferinterface.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Fada/dofinformationinterface.h"
#include  "Alat/systemvectorinterface.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/quadtotri.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/refineinfo.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
DomainSolver::~DomainSolver()
{
}

DomainSolver::DomainSolver(): Fada::DomainSolver()
{
}

DomainSolver::DomainSolver( const DomainSolver& domainsolver): Fada::DomainSolver(domainsolver)
{
(*this).operator=(domainsolver);
}

DomainSolver& DomainSolver::operator=( const DomainSolver& domainsolver)
{
Fada::DomainSolver::operator=(domainsolver);
assert(0);
return *this;
}

std::string DomainSolver::getName() const
{
return "DomainSolver";
}

DomainSolver* DomainSolver::clone() const
{
return new DomainSolver(*this);
}

/*--------------------------------------------------------------------------*/
Fada::DomainIntegrationLoopInterface* DomainSolver::newDomainIntegrationLoop() const
{
  // assert(getModel());
  // return getModel()->newDiscretization();
  return new Fada::UnStructuredIntegrationLoop();
}

/*--------------------------------------------------------------------------*/
void DomainSolver::testProlongate(Alat::GhostVector& u) const
{
  Fada::MultiLevelVector* umg = dynamic_cast<Fada::MultiLevelVector*>( getVector(u) );
  assert(umg);
  int nlevels = getNLevels();
  getDomainIntegrationLoop()->initSolution( umg, nlevels-1);
  std::cerr << "DomainSolver::testProlongate() " << nlevels-1 << " : " << umg->getVector(nlevels-1)->norm()<<"\n";
  for(int level=nlevels-2;level>=0;level--)
  {
    prolongate(level, u, u);
    std::cerr << "### from " << level+1 << " to " << level << " : " << umg->getVector(level)->norm() << "\n";
  }
}
/*--------------------------------------------------------------------------*/
void DomainSolver::interpolateSolution( Alat::GhostVector& unew, const Alat::GhostVector& uold) const
{
  int level=0;
  getMesh()->setResolution(level);
  vectorZero(unew);
  Alat::SystemVectorInterface* unewall = NULL;
  const Alat::SystemVectorInterface* uoldall = NULL;
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
  const Fada::VariablesMap& unknowns = getVariableManager()->getUnknowns();
  for(Fada::VariablesMap::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    Fada::VariableInterface* var = p->second;
    const Fada::DofInformationInterface* dofinformation = getDofManagerAllVariables()->getDofInformationOfVariable( var->getVarName() );
    if(dofinformation->getFem() == NULL)
    {
      continue;
    }
    interpolateOneVariable(dofinformation, unewall->getVector(p->first), uoldall->getVector(p->first));
  }
  if(unewmg)
  {
    const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( _refinedmesh );
    assert(mlmesh);
    for(int level=0;level<getNLevels()-1;level++)
    {
      const FadaMesh::MeshInterface* coarsemesh = mlmesh->getMesh(level+1);
      const FadaMesh::MeshInterface* finemesh = mlmesh->getMesh(level);
      project(coarsemesh, finemesh, unewmg->getVector(level+1), unewmg->getVector(level));
    }
  }
}
/*--------------------------------------------------------------------------*/
void DomainSolver::project( const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine) const
{
  const FadaMesh::QuadToTri* quadtotrifine = dynamic_cast<const FadaMesh::QuadToTri*>(finemesh);
  assert(quadtotrifine);
  const FadaMesh::QuadToTri* quadtotricoarse =  dynamic_cast<const FadaMesh::QuadToTri*>(coarsemesh);
  assert(quadtotricoarse);
  const FadaMesh::QuadrilateralMesh& quadmeshcoarse = quadtotricoarse->getQuadrilateralMesh();
  const FadaMesh::QuadrilateralMesh& quadmeshfine = quadtotrifine->getQuadrilateralMesh();
  // const FadaMesh::GeometryObject* geo = quadmeshfine.getGeometryObject("RefineInfo");
  const FadaMesh::GeometryObject* geo = quadmeshcoarse.getGeometryObject("RefineInfo");
  const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  assert(refinfo);
  const Alat::FixArray<4,int>& refinfoinfonode = refinfo->refinfoinfonode;
  const Alat::FixArray<4,int>& refinfoinfoside = refinfo->refinfoinfoside;
  const Alat::FixArray<4,int>& refinfoinfocell = refinfo->refinfoinfocell;
  ucoarse->zeros();


  // std::cerr << "fine  " << quadtotrifine->getFileNameQuad() << " coarse " << quadtotricoarse->getFileNameQuad() << "\n";
  // std::cerr << "fine " << quadtotrifine->getNNodes() << " coarse " << quadtotricoarse->getNNodes() << "\n";
  // std::cerr << "fine quad " << quadmeshfine.getNNodes() << " coarse quad " << quadmeshcoarse.getNNodes() << "\n";
  if(quadmeshfine.getNNodes()==quadmeshcoarse.getNNodes())
  {
    std::cerr << "fine  " << quadtotrifine->getFileNameQuad() << " coarse " << quadtotricoarse->getFileNameQuad() << "\n";
    std::cerr << "fine " << quadtotrifine->getNNodes() << " coarse " << quadtotricoarse->getNNodes() << "\n";
    std::cerr << "fine quad " << quadmeshfine.getNNodes() << " coarse quad " << quadmeshcoarse.getNNodes() << "\n";
    exit(1);
  }


  const Fada::VariablesMap& unknowns = getVariableManager()->getUnknowns();
  for(Fada::VariablesMap::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    Alat::VariableVector* ucoarsev = dynamic_cast<Alat::VariableVector*>(ucoarse->getVector(p->first));
    const Alat::VariableVector* ufinev = dynamic_cast<const Alat::VariableVector*>(ufine->getVector(p->first));
    int ncomp = ucoarsev->getNComponents();

    for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
    {
      // pas raffine
      if(refinfo->getNCoarseNodes(iK)==4)
      {
        int iKf = refinfo->getCoarseCellNumber(iK, 0);
        for(int ii=0;ii<4;ii++)
        {
          int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
          int nodeidf = quadmeshfine.getNodeIdOfCell(iKf,ii);
          for(int icomp = 0; icomp < ncomp; icomp++)
          {
            (*ucoarsev)(icomp, nodeidc) = (*ufinev)(icomp, nodeidf);
          }
        }
        int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
        int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          (*ucoarsev)(icomp, centernodec) = (*ufinev)(icomp, centernodef);
        }
        continue;
      }
      //  raffine
      for(int ii=0;ii<4;ii++)
      {
        int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
        int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfonode[ii]);
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          (*ucoarsev)(icomp, nodeidc) = (*ufinev)(icomp, nodeidf);
        }
      }
      int centernodec = quadmeshcoarse.getNNodes()+iK;
      int centernodef = refinfo->getCoarseNodesNumber(iK,3);
      // std::cerr << "UR centernodec centernodef " << centernodec << " ---> " << centernodef << "\n";
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        (*ucoarsev)(icomp, centernodec) = (*ufinev)(icomp, centernodef);
      }
    }
  }
}
