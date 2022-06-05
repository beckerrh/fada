#include  "Fada/multileveltransfercgp12d.h"
#include  "Alat/variablevector.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/quadtotri.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/refineinfo.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferCgP12d::~MultiLevelTransferCgP12d()
{
}

MultiLevelTransferCgP12d::MultiLevelTransferCgP12d(): Fada::MultiLevelTransferSingleFemInterface()
{
}

MultiLevelTransferCgP12d::MultiLevelTransferCgP12d( const MultiLevelTransferCgP12d& multileveltransfercgp12d): Fada::MultiLevelTransferSingleFemInterface(multileveltransfercgp12d)
{
(*this).operator=(multileveltransfercgp12d);
}

MultiLevelTransferCgP12d& MultiLevelTransferCgP12d::operator=( const MultiLevelTransferCgP12d& multileveltransfercgp12d)
{
Fada::MultiLevelTransferSingleFemInterface::operator=(multileveltransfercgp12d);
assert(0);
return *this;
}

std::string MultiLevelTransferCgP12d::getName() const
{
return "MultiLevelTransferCgP12d";
}

MultiLevelTransferCgP12d* MultiLevelTransferCgP12d::clone() const
{
return new MultiLevelTransferCgP12d(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferCgP12d::basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( mesh );
  assert(mlmesh);
  int nlevels = mlmesh->getNLevels();
  _matrixtransfer.set_size(nlevels-1);
  _matrixproject.set_size(nlevels-1);
  _nodeids.set_size(nlevels-1);
  for(int level = 0; level < nlevels-1; level++)
  {
    // std::cerr << "MultiLevelTransferCgP12d ### from " << level << " to " << level+1 << "\n";
    mlmesh->setResolution(level+1);
    const FadaMesh::MeshInterface* coarsemesh = mlmesh->getMesh(level+1);
    const FadaMesh::MeshInterface* finemesh = mlmesh->getMesh(level);
    _constructP12d(_matrixtransfer[level], _matrixproject[level], _nodeids[level], coarsemesh, finemesh);
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferCgP12d::prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d) const
{
  _matrixtransfer[level].matrixVectorProduct(ufine, ucoarse, d);
}
void MultiLevelTransferCgP12d::restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const
{
  assert(level-1<_matrixtransfer.size());
  const Alat::VariableMatrix& matrixtransfer = _matrixtransfer[level-1];
  assert(ufine->n()==matrixtransfer.getSparsityPattern()->n());
  ucoarse->zeros();
  matrixtransfer.matrixVectorProductTransposed(ucoarse, ufine);
}
void MultiLevelTransferCgP12d::project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const
{
  Alat::VariableVector* uc =dynamic_cast<Alat::VariableVector*>(ucoarse);
  assert(uc);
  const Alat::VariableVector* uf =dynamic_cast<const Alat::VariableVector*>(ufine);
  assert(uf);
  const Alat::IntVector& nodeids = _nodeids[level-1];
  int ncomp = ufine->getNComponents();
  // std::cerr << "project " << level-1 << " n " << nodeids.size() << " " << uc->n()<< " " << uf->n() << "\n";
  // std::cerr << "nodeids" << nodeids << "\n";
  assert(nodeids.size()==uc->n());
  for(int i=0; i< nodeids.size(); i++)
  {
    int ifine = nodeids[i];
    (*uc)(i) = (*uf)(ifine);
    // for(int icomp=0;icomp<ncomp;icomp++)
    // {
    //   (*uc)(icomp, i) = (*uf)(icomp, ifine);
    // }
  }
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferCgP12d::_constructP12d(Alat::VariableMatrix& matrixtransfer, Alat::VariableMatrix& matrixproject, Alat::IntVector& nodeids, const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh)
{
  const FadaMesh::QuadToTri* quadtotrifine = dynamic_cast<const FadaMesh::QuadToTri*>(finemesh);
  assert(quadtotrifine);
  const FadaMesh::QuadToTri* quadtotricoarse =  dynamic_cast<const FadaMesh::QuadToTri*>(coarsemesh);
  assert(quadtotricoarse);
  const FadaMesh::QuadrilateralMesh& quadmeshcoarse = quadtotricoarse->getQuadrilateralMesh();
  const FadaMesh::QuadrilateralMesh& quadmeshfine = quadtotrifine->getQuadrilateralMesh();
  const FadaMesh::GeometryObject* geo = quadmeshcoarse.getGeometryObject("RefineInfo");
  const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  assert(refinfo);

  const FadaMesh::TriOfQuad& trifquad = quadtotricoarse->getTriOfQuad();
  // const FadaMesh::GeometryObject* geohn = quadmeshfine.getGeometryObject("HangingNodeInfo");
  // const FadaMesh::HangingNodeInfo* hninfo = dynamic_cast<const FadaMesh::HangingNodeInfo*>( geohn );
  // assert(hninfo);


  const Alat::FixArray<4,int>& refinfoinfonode = refinfo->refinfoinfonode;
  const Alat::FixArray<4,int>& refinfoinfoside = refinfo->refinfoinfoside;
  const Alat::FixArray<4,int>& refinfoinfocell = refinfo->refinfoinfocell;


  const Alat::IntVector& nodo2n = refinfo->getNodeIds();
  assert(quadtotricoarse->getNNodes()==nodo2n.size()+quadmeshcoarse.getNCells());
  nodeids.set_size(quadtotricoarse->getNNodes());
  for(int i=0;i<nodo2n.size();i++)
  {
    nodeids[i] = nodo2n[i];
  }
  for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
  {
    if(refinfo->getNCoarseNodes(iK)==4)
    {
      int iKf = refinfo->getCoarseCellNumber(iK, 0);
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      nodeids[centernodec] = centernodef;
    }
    else
    {
      int centernodec = quadmeshcoarse.getNNodes()+iK;
      int centernodef = refinfo->getCoarseNodesNumber(iK,3);
      nodeids[centernodec] = centernodef;
    }
  }
  // std::cerr << "nodeids" << nodeids << "\n";

  Alat::IntMap nodeinv;
  // std::cerr << "nodo2n" << nodo2n << "\n";
  for(int i=0;i<nodo2n.size();i++)
  {
    nodeinv[nodo2n[i]] = i;
  }
  // for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
  // {
  //   if(refinfo->getNCoarseNodes(iK)==4)
  //   {
  //     int iKf = refinfo->getCoarseCellNumber(iK, 0);
  //     int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
  //     int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
  //     nodeinv[centernodef]=centernodec;
  //   }
  //   else
  //   {
  //     int centernodec = quadmeshcoarse.getNNodes()+iK;
  //     int centernodef = refinfo->getCoarseNodesNumber(iK,3);
  //     nodeinv[centernodef]=centernodec;
  //   }
  // }
  // std::cerr << "nodeinv" << nodeinv << "\n";

  // for(Alat::IntMap::const_iterator p=nodeinv.begin(); p!=nodeinv.end(); p++)
  // {
  //   Alat::Node xnew = quadtotrifine->getNode(p->first);
  //   Alat::Node xold = quadtotricoarse->getNode(p->second);
  //   xold.add(-1.0, xnew);
  //   if(xold.x()*xold.x()+xold.y()*xold.y() > 1e-16)
  //   {
  //     std::cerr << "### wrong nodeid2id " << p->second << " ==> " << p->first << "\n";
  //     std::cerr << "### wrong nodeid2id " << p->second << " ==> " << p->first << "\n";
  //     std::cerr << "### wrong nodeid2id " << p->second << " ==> " << p->first << "\n";
  //     // assert(0);
  //   }
  // }

  // std::cerr << nodo2n.size() << " : coarse " << quadmeshcoarse.getNNodes() << " fine " << quadmeshfine.getNNodes() << "\n";
  // assert(nodo2n.size()==quadmeshfine.getNNodes());

  // std::cerr << "fine getFileNameQuad " << quadtotrifine->getFileNameQuad() << " coarse getFileNameQuad" << quadtotricoarse->getFileNameQuad() << "\n";
  // std::cerr << "fine " << quadtotrifine->getNNodes() << " coarse " << quadtotricoarse->getNNodes() << "\n";
  // std::cerr << "fine quad " << quadmeshfine.getNNodes() << " coarse quad " << quadmeshcoarse.getNNodes() << "\n";
  Alat::SparsityPatternSoft sparsitypatternsoft(quadtotrifine->getNNodes());

  for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
  {
    // pas raffine
    if(refinfo->getNCoarseNodes(iK)==4)
    {
      // std::cerr << "not refined ntris=" << iK << " --> " << trifquad.getNTrianglesOfQuad(iK) << "\n";
      assert(refinfo->getNCoarseCells(iK)==1);
      // if(trifquad.getNTrianglesOfQuad(iK)==4)
      {
        int iKf = refinfo->getCoarseCellNumber(iK, 0);
        for(int ii=0;ii<4;ii++)
        {
          int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
          int nodeidf = quadmeshfine.getNodeIdOfCell(iKf,ii);
          sparsitypatternsoft[nodeidf].insert(nodeidc);
          // nodeids[nodeidc] = nodeidf;
          // std::cerr << "not refined: nodeidf nodeidc= " << nodeidf << " --> " << nodeidc << "\n";
        }
        int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
        int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
        sparsitypatternsoft[centernodef].insert(centernodec);
        // nodeids[centernodec] = centernodef;
        // std::cerr << "not refined: centernodef centernodec= " << centernodef << " --> " << centernodec << "\n";
      }
      continue;
    }
    //  raffine
    for(int ii=0;ii<4;ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfonode[ii]);
      // std::cerr << "nodeidc nodeidf " << nodeidc << " ---> " << nodeidf << "\n";
      sparsitypatternsoft[nodeidf].insert(nodeidc);
      // nodeids[nodeidc] = nodeidf;
    }
    int centernodec = quadmeshcoarse.getNNodes()+iK;
    int centernodef = refinfo->getCoarseNodesNumber(iK,3);
    // std::cerr << "centernodec " << centernodec << " ---> " << centernodef << "\n";
    sparsitypatternsoft[centernodef].insert(centernodec);
    // nodeids[centernodec] = centernodef;
    for(int ii=0;ii<4;ii++)
    {
      int nodec1 = quadmeshcoarse.getNodeIdOfCell(iK,ii);
      int nodec2 = quadmeshcoarse.getNodeIdOfCell(iK,(ii+1)%4);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfoside[ii]);
      // std::cerr << "nodec1 nodec2  " << nodec1 << " : " << nodec2 << " ---> " << nodeidf << "\n";
      if(sparsitypatternsoft[nodeidf].size()==0)
      {
        sparsitypatternsoft[nodeidf].insert(nodec1);
        sparsitypatternsoft[nodeidf].insert(nodec2);
      }
    }
    // }
    for(int ii=0;ii<4;ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,refinfoinfocell[ii]);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfonode[ii]);
      int iKf = refinfo->getCoarseCellNumber(iK, ii);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      // std::cerr << "nodeidc centernodec  " << nodeidc << " : " << centernodec << " ---> " << centernodef << "\n";
      sparsitypatternsoft[centernodef].insert(centernodec);
      sparsitypatternsoft[centernodef].insert(nodeidc);
    }
  }
  for(Alat::IntMap::const_iterator p=nodeinv.begin(); p!=nodeinv.end(); p++)
  {
    sparsitypatternsoft[p->first].clear();
    sparsitypatternsoft[p->first].insert(p->second);
  }

  matrixtransfer.set_size(sparsitypatternsoft);
  matrixproject.set_size(sparsitypatternsoft);
  const Alat::SparsityPattern& SP = *matrixtransfer.getSparsityPattern();
  Alat::DoubleVector& mattransfer = matrixtransfer.getSimpleMatrix();
  mattransfer.zeros();


  for(int iK = 0; iK < quadmeshcoarse.getNCells(); iK++)
  {
    // pas raffine
    if(refinfo->getNCoarseNodes(iK)==4)
    {
      assert(refinfo->getNCoarseCells(iK)==1);
      int iKf = refinfo->getCoarseCellNumber(iK, 0);
      for(int ii=0;ii<4;ii++)
      {
        int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
        int nodeidf = quadmeshfine.getNodeIdOfCell(iKf,ii);
        // std::cerr << "UR nodeidc nodeidf " << nodeidc << " ---> " << nodeidf << "\n";
        for(int pos = SP.rowstart(nodeidf); pos < SP.rowstop(nodeidf); pos++)
        {
          if(SP.col(pos) == nodeidc)
          {
            mattransfer[pos] = 1.0;
          }
        }
      }
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      // std::cerr << "UR centernodec centernodef " << centernodec << " ---> " << centernodef << "\n";
      for(int pos = SP.rowstart(centernodef); pos < SP.rowstop(centernodef); pos++)
      {
        if(SP.col(pos) == centernodec)
        {
          mattransfer[pos] = 1.0;
        }
      }
      continue;
    }
    //  raffine
    for(int ii=0;ii<4;ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,ii);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfonode[ii]);
      // std::cerr << "nodeidc nodeidf " << nodeidc << " ---> " << nodeidf << "\n";
      for(int pos = SP.rowstart(nodeidf); pos < SP.rowstop(nodeidf); pos++)
      {
        if(SP.col(pos) == nodeidc)
        {
          mattransfer[pos] = 1.0;
        }
      }
    }
    int centernodec = quadmeshcoarse.getNNodes()+iK;
    int centernodef = refinfo->getCoarseNodesNumber(iK,3);
    // std::cerr << "centernodec centernodef " << centernodec << " ---> " << centernodef << "\n";
    for(int pos = SP.rowstart(centernodef); pos < SP.rowstop(centernodef); pos++)
    {
      if(SP.col(pos) == centernodec)
      {
        mattransfer[pos] = 1.0;
      }
    }
    for(int ii=0;ii<4;ii++)
    {
      int nodec1 = quadmeshcoarse.getNodeIdOfCell(iK,ii);
      int nodec2 = quadmeshcoarse.getNodeIdOfCell(iK,(ii+1)%4);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfoside[ii]);
      // std::cerr << "nodec1 nodec2  " << nodec1 << " : " << nodec2 << " ---> " << nodeidf << "\n";
      for(int pos = SP.rowstart(nodeidf); pos < SP.rowstop(nodeidf); pos++)
      {
        if(SP.col(pos) == nodec1)
        {
          mattransfer[pos] = 0.5;
        }
        else if(SP.col(pos) == nodec2)
        {
          mattransfer[pos] = 0.5;
        }
      }
    }
    for(int ii=0;ii<4;ii++)
    {
      int nodeidc = quadmeshcoarse.getNodeIdOfCell(iK,refinfoinfocell[ii]);
      int nodeidf = refinfo->getCoarseNodesNumber(iK,refinfoinfonode[ii]);
      int iKf = refinfo->getCoarseCellNumber(iK, ii);
      int centernodec = quadtotricoarse->getCenterNodeIdOfQuad(iK);
      int centernodef = quadtotrifine->getCenterNodeIdOfQuad(iKf);
      // std::cerr << "nodeidc centernodec  " << nodeidc << " : " << centernodec << " ---> " << centernodef << "\n";
      sparsitypatternsoft[centernodef].insert(centernodec);
      sparsitypatternsoft[centernodef].insert(nodeidc);
      for(int pos = SP.rowstart(centernodef); pos < SP.rowstop(centernodef); pos++)
      {
        if(SP.col(pos) == centernodec)
        {
          mattransfer[pos] = 0.5;
        }
        else if(SP.col(pos) == nodeidc)
        {
          mattransfer[pos] = 0.5;
        }
      }
    }
  }
  const Alat::SparsityPattern& sparsitypattern = *matrixtransfer.getSparsityPattern();
  for(Alat::IntMap::const_iterator p=nodeinv.begin(); p!=nodeinv.end(); p++)
  {
    int pos = sparsitypattern.rowstart(p->first);
    mattransfer[pos] = 1.0;
  }
  for(int i = 0; i < sparsitypattern.n(); i++)
  {
    double sum=0.0;
    for(int pos = sparsitypattern.rowstart(i); pos < sparsitypattern.rowstop(i); pos++)
    {
      sum += matrixtransfer.getValueSimple(pos);
    }
    if( fabs(sum-1.0)>1e-12)
    {
      std::cerr << "### ERROR in Line " << i <<"\n";
      std::cerr << "### ERROR in Line " << i <<"\n";
      std::cerr << "### ERROR in Line " << i <<"\n";
      std::cerr << "### ERROR in Line " << i <<"\n";
      for(int pos = sparsitypattern.rowstart(i); pos < sparsitypattern.rowstop(i); pos++)
      {
        std::cerr << sparsitypattern.col(pos) << " : " <<  matrixtransfer.getValueSimple(pos) << "\n";
      }
      std::cerr<< "nnodes coarse fine : "  << quadtotricoarse->getNNodes() << " " << quadtotrifine->getNNodes() << "\n";
      for(int i = 0; i < sparsitypattern.n(); i++)
      {
        if(sparsitypattern.rowsize(i)==3)
        {
          std::cerr << "i = " << i << " ==> ";
          for(int pos = sparsitypattern.rowstart(i); pos < sparsitypattern.rowstop(i); pos++)
          {
            std::cerr << sparsitypattern.col(pos) << " : " <<  matrixtransfer.getValueSimple(pos) << "\t";
          }
          std::cerr << "\n";
        }
      }
      // assert(0);
    }
  }

  // std::cerr<< "matrixtransfer: "  << matrixtransfer << "\n";
  matrixtransfer.suppressZeros();
  matrixproject.suppressZeros();
}
