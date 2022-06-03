#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/quadtotri.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/patchinfo.h"
#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/fixarray.h"
#include  <fstream>
#include  <algorithm>
#include  "Alat/armadillo.h"

using namespace FadaMesh;
using namespace std;

///*--------------------------------------------------------------------------*/

////            3--------------------2
////            | \                 /|
////            |   \     2       /  |
////            |     \         /    |
////            |       \     /      |
////            |         \ /        |
////            |  3      / \    1   |
////            |       /     \      |
////            |     /        \     |
////            |   /     0     \    |
////            | /               \  |
////            0--------------------1

////            3----------|----------2
////            | \       /|\       /|
////            |   \   /  |  \   /  |
////            |     \    |    \/   |
////            |   /  \   |   / \   |
////            | /     \  |  /   \  |
////            |_________\|/ ______\|
////            |\        /|\       /|
////            | \    /  |  \   /   |
////            |   \/    |   \/     |
////            |   / \   |   / \    |
////            | /     \ | /     \  |
////            0---------|----------1

/*--------------------------------------------------------------------------*/

void TriOfQuad::write(std::ostream& out, std::string datatype) const
{
  out<<_data.size()<<'\n';
  for(int i = 0; i < _data.size(); i++)
  {
    _data[i].saveFada(out, datatype);
  }
}
void TriOfQuad::read(std::istream& in)
{
  int n;
  in>>n;
  _data.set_size(n);
  for(int i = 0; i < _data.size(); i++)
  {
    _data[i].loadFada(in);
  }
}
int TriOfQuad::getNTrianglesOfQuad(int iquad) const
{
  return _data[iquad].size();
}
int TriOfQuad::getTriIdOfQuad(int iquad, int iit) const
{
  return _data[iquad][iit][1];
}
int TriOfQuad::getLocalIdOfQuadSide(int iquad, int iit) const
{
  return _data[iquad][iit][0];
}
Alat::Vector<Alat::Vector<Alat::FixArray<2, int> > >& TriOfQuad::getData()
{
  return _data;
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

QuadToTri::~QuadToTri(){}
QuadToTri::QuadToTri() : TriangleMesh(), _infilenamequad("none"){}
QuadToTri::QuadToTri(std::string quadfilename): TriangleMesh()
{
  _infilenamequad = quadfilename;
  _quadmesh.readFadaMesh(quadfilename);
  convertMesh(_quadmesh);
}
QuadToTri& QuadToTri::operator=(const QuadToTri& quadtotri) {assert(0);}
std::string QuadToTri::getName() const
{
  return "FadaMesh::QuadToTri";
}
std::string QuadToTri::getFileNameQuad() const
{
  return _infilenamequad;
}

const TriOfQuad& QuadToTri::getTriOfQuad() const
{
  return _tri_of_quad;
}

TriOfQuad& QuadToTri::getTriOfQuad()
{
  return _tri_of_quad;
}

Alat::Vector<Alat::Vector<Alat::FixArray<2, int> > >& QuadToTri::getTriOfQuadData()
{
  return getTriOfQuad().getData();
}

const Alat::IntVector& QuadToTri::getQuadOfTri() const
{
  return _quad_of_tri;
}

const QuadrilateralMesh& QuadToTri::getQuadrilateralMesh() const
{
  return _quadmesh;
}

/*--------------------------------------------------------------------------*/
void QuadToTri::convertMesh(const FadaMesh::MeshInterface& M, std::string type)
{
  const FadaMesh::QuadrilateralMesh* quadmesh = dynamic_cast<const FadaMesh::QuadrilateralMesh*>( &M );
  assert(quadmesh);
  // std::cerr << "QuadToTri::convertMesh() " << quadmesh->getInfileName() << " type= "<< type << "\n";
  // if(type == "crisscross")
  // {
  //   _makeCrissCross(_quadmesh);
  // }
  // else
  // {
  //   _MakeDiagonal(_quadmesh);
  // }
  _makeCrissCross(_quadmesh);

  FadaMesh::TriangleMesh::BoundarySideToColor bstc;
  const FadaMesh::BoundaryInfo* BI = _quadmesh.getBoundaryInfo();
  const Alat::IntVector& colors = BI->getColors();
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& bic = BI->getSidesOfColor(color);
    for(int i = 0; i < bic.size(); i++)
    {
      const TriangleMesh::Side& SQ = _quadmesh.getSide(bic[i]);
      TriangleMesh::Side S;
      for(int ii = 0; ii < 2; ii++)
      {
        S[ii] = SQ[ii];
      }
      sort( S.begin(), S.end() );
      bstc[S] = color;
    }
  }
  constructSidesFromCells(bstc);
  // std::cerr << "QuadToTri::convertMesh() nnodes(quad) " << quadmesh->getNNodes() << " nnodes= "<< getNNodes()  << "\n";
}

/*--------------------------------------------------------------------------*/
void QuadToTri::writeFadaMesh(const std::string&  name, std::string datatype) const
{
  TriangleMesh::writeFadaMesh(name, datatype);
  std::string cfilename = name + ".fadalightmesh/tri_of_quad";
  std::ofstream file( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  _tri_of_quad.write(file);
  file.close();
  cfilename = name + ".fadalightmesh/quad_of_tri";
  file.open( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  _quad_of_tri.saveFada(file, datatype);
  file.close();
  cfilename = name + ".fadalightmesh/quadfilename";
  file.open( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  file << _infilenamequad;
  file.close();

  // _quadmesh.writeFadaMesh(name + ".fadalightmesh/quadmesh", datatype);
}

/*--------------------------------------------------------------------------*/
void QuadToTri::readFadaMesh(const std::string& name)
{
  TriangleMesh::readFadaMesh(name);
  std::string cfilename = name + ".fadalightmesh/tri_of_quad";
  std::ifstream file( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  _tri_of_quad.read(file);
  file.close();
  cfilename = name + ".fadalightmesh/quad_of_tri";
  file.open( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  _quad_of_tri.loadFada(file);
  file.close();
  cfilename = name + ".fadalightmesh/quadfilename";
  file.open( cfilename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** QuadToTri::QuadToTri() could not open file " << cfilename << "\n";
    assert(0);
  }
  file >> _infilenamequad;
  file.close();

  _quadmesh.readFadaMesh(_infilenamequad);
}
/*--------------------------------------------------------------------------*/
int QuadToTri::getCenterNodeIdOfQuad(int iK) const
{
  return _quadmesh.getNNodes()+iK;
}

/*--------------------------------------------------------------------------*/
void QuadToTri::_makeCrissCross(const FadaMesh::MeshInterface& QM)
{
  const FadaMesh::GeometryObject* geo = QM.getGeometryObject("HangingNodeInfo");
  const FadaMesh::HangingNodeInfo* hninfo = dynamic_cast<const FadaMesh::HangingNodeInfo*>( geo );
  assert(hninfo);

  int nnodes = QM.getNNodes() + QM.getNCells();
  // std::cerr << "##### nnodes="<< nnodes << " QM.getNNodes() " << QM.getNNodes() << " QM.getNCells() " <<  QM.getNCells() << "\n";
  int nhinfo = std::max( 0, hninfo->n() );
  int ncells = 4*QM.getNCells() + nhinfo;

  _quad_of_tri.set_size(ncells);
  getTriOfQuadData().set_size( QM.getNCells() );

  Alat::Vector<Alat::Node >& nodes = getAllNodes();
  nodes.set_size(nnodes);
  for(int i = 0; i < QM.getNNodes(); i++)
  {
    nodes[i].x() = QM.getNode(i).x();
    nodes[i].y() = QM.getNode(i).y();
  }
  arma::mat a(2,2);
  arma::vec x(2), b(2);
  for(int i = 0; i < QM.getNCells(); i++)
  {
    a(0,0) = nodes[QM.getNodeIdOfCell(i, 2)].x() - nodes[QM.getNodeIdOfCell(i, 0)].x();
    a(0,1) = nodes[QM.getNodeIdOfCell(i, 1)].x() - nodes[QM.getNodeIdOfCell(i, 3)].x();
    a(1,0) = nodes[QM.getNodeIdOfCell(i, 2)].y() - nodes[QM.getNodeIdOfCell(i, 0)].y();
    a(1,1) = nodes[QM.getNodeIdOfCell(i, 1)].y() - nodes[QM.getNodeIdOfCell(i, 3)].y();
    b[0] = nodes[QM.getNodeIdOfCell(i, 1)].x() - nodes[QM.getNodeIdOfCell(i, 0)].x();
    b[1] = nodes[QM.getNodeIdOfCell(i, 1)].y() - nodes[QM.getNodeIdOfCell(i, 0)].y();
    x = arma::solve(a,b);
    nodes[QM.getNNodes()+i].x() = nodes[QM.getNodeIdOfCell(i, 0)].x() + x[0]* (nodes[QM.getNodeIdOfCell(i, 2)].x() - nodes[QM.getNodeIdOfCell(i, 0)].x());
    nodes[QM.getNNodes()+i].y() = nodes[QM.getNodeIdOfCell(i, 0)].y() + x[0]* (nodes[QM.getNodeIdOfCell(i, 2)].y() - nodes[QM.getNodeIdOfCell(i, 0)].y());


    // std::cerr << "iK=" << i << " : ";
    // double x = 0.0, y = 0.0;
    // for(int ii = 0; ii < 4; ii++)
    // {
    //   // std::cerr << QM.getNodeIdOfCell(i, ii) << " ";
    //   x += 0.25 * QM.getNode( QM.getNodeIdOfCell(i, ii) ).x();
    //   y += 0.25 * QM.getNode( QM.getNodeIdOfCell(i, ii) ).y();
    // }
    // // std::cerr << " ---> " << QM.getNNodes()+i << "\n";
    // nodes[QM.getNNodes()+i].x() = x;
    // nodes[QM.getNNodes()+i].y() = y;
  }

  Alat::Vector<TriangleMesh::Triangle>& tr = getCells();
  assert(ncells);
  tr.reserve(ncells);
  tr.resize(ncells);
  for(int i = 0; i < QM.getNCells(); i++)
  {
    getTriOfQuadData()[i].set_size(4);
    for(int ii = 0; ii < 4; ii++)
    {
      int jj = ( ii+1 ) % 4;
      tr[4*i+ii][0] = QM.getNodeIdOfCell(i, ii);
      tr[4*i+ii][1] = QM.getNodeIdOfCell(i, jj);
      tr[4*i+ii][2] = QM.getNNodes()+i;
      _quad_of_tri[4*i+ii] = i;
      getTriOfQuadData()[i][ii][0] = ii;
      getTriOfQuadData()[i][ii][1] = 4*i+ii;
    }
  }
  for(int i = 0; i < hninfo->n(); i++)
  {
    int ic = hninfo->getCellNumber(i);
    int is = hninfo->getLocalSide(i);
    int in = hninfo->getHangingNodes(i, 0);

    int inold = tr[4*ic+is][1];
    tr[4*ic+is][1] = in;

    tr[4*QM.getNCells()+i][0] = in;
    tr[4*QM.getNCells()+i][1] = inold;
    tr[4*QM.getNCells()+i][2] = tr[4*ic+is][2];
    _quad_of_tri[4*QM.getNCells()+i] = ic;
    Alat::FixArray<2, int> p;
    p[0] = is;
    p[1] = 4*QM.getNCells()+i;
    getTriOfQuadData()[ic].push_back(p);
  }
}

/*--------------------------------------------------------------------------*/
void QuadToTri::constructPatchInfo(PatchInfo& patchinfo) const
{
  int ncells =  getNCells();
  int nnodes =  getNNodes();
  Alat::Map<int,Alat::IntSet> cellsofpatch;
  for(int iK=0;iK<ncells;iK++)
  {
    for(int ii=0;ii<getNNodesPerCell(iK); ii++)
    {
      int iN = getNodeIdOfCell(iK, ii);
      cellsofpatch[iN].insert(iK);
    }
  }
  patchinfo.cells.set_size(nnodes);
  patchinfo.edgesinner.set_size(nnodes);
  patchinfo.edgesouter.set_size(nnodes);
  patchinfo.nodes.set_size(nnodes);
  for(int iN=0; iN<nnodes;iN++)
  {
    int npatch = cellsofpatch[iN].size();
    Alat::Set<Alat::FixArray<3,int> > allsides;
    for(Alat::IntSet::const_iterator p = cellsofpatch[iN].begin(); p!= cellsofpatch[iN].end(); p++)
    {
      int iK = *p;
      for(int ii=0;ii<getNSidesPerCell(iK); ii++)
      {
        Alat::FixArray<3,int> fa;
        fa[0] = getSideIdOfCell(iK, ii);
        fa[1] = iK;
        fa[2] = ii;
        allsides.insert(fa);
      }
    }
    // std::cerr << "allsides="<<allsides << "\n";
    Alat::Map<int, Alat::IntSet > edgesinner2cell;
    for(Alat::Set<Alat::FixArray<3,int> >::const_iterator p = allsides.begin(); p!= allsides.end(); p++)
    {
      int iS = (*p)[0];
      int iK = (*p)[1];
      for(int ii=0;ii<getNNodesPerSide(iS);ii++)
      {
        int iM = getNodeIdOfSide(iS, ii);
        if (iM==iN)
        {
          edgesinner2cell[iS].insert(iK);
          break;
        }
      }
    }
    // std::cerr << "edgesinner2cell="<<edgesinner2cell << "\n";
    Alat::Map<int, Alat::IntSet > cell2edgesinner;
    for(Alat::Map<int, Alat::IntSet >::const_iterator p = edgesinner2cell.begin(); p!= edgesinner2cell.end(); p++)
    {
      int iS = p->first;
      for(Alat::IntSet::const_iterator q=p->second.begin(); q!=p->second.end();q++)
      {
        cell2edgesinner[*q].insert(iS);
      }
    }
    // std::cerr << "cell2edgesinner="<<cell2edgesinner << "\n";

    int firstinneredge=-1;
    int celloffirstinneredge=-1;
    for(Alat::Map<int, Alat::IntSet >::const_iterator p = edgesinner2cell.begin(); p!= edgesinner2cell.end(); p++)
    {
      int iS = p->first;
      if(p->second.size()==1)
      {
        firstinneredge = iS;
        celloffirstinneredge = *p->second.begin();
        break;
      }
    }
    int npatch2=npatch;
    if (firstinneredge==-1)
    {
      Alat::Map<int, Alat::IntSet >::const_iterator p = edgesinner2cell.begin();
      firstinneredge = p->first;
      celloffirstinneredge = *p->second.begin();
    }
    else
    {
      npatch2 = npatch+1;
    }
    patchinfo.cells[iN].set_size(npatch);
    patchinfo.edgesouter[iN].set_size(npatch);
    patchinfo.edgesinner[iN].set_size(npatch2);
    patchinfo.nodes[iN].set_size(npatch2);

    // std::cerr << "npatch2="<<npatch2 << "\n";
    patchinfo.edgesinner[iN][0] = firstinneredge;
    patchinfo.cells[iN][0] = celloffirstinneredge;
    for(int ii=1;ii<npatch2; ii++)
    {
      // std::cerr << ii << " firstinneredge = " << firstinneredge << "celloffirstinneredge = " << celloffirstinneredge<< "\n";
      bool found = false;
      for(Alat::IntSet::const_iterator q = cell2edgesinner[celloffirstinneredge].begin(); q != cell2edgesinner[celloffirstinneredge].end();q++)
      {
        if(*q!=firstinneredge)
        {
          firstinneredge = *q;
          found = true;
          break;
        }
      }
      assert(found);
      // std::cerr << "firstinneredge = " << firstinneredge << "\n";
      found = false;
      for(Alat::IntSet::const_iterator q = edgesinner2cell[firstinneredge].begin(); q != edgesinner2cell[firstinneredge].end();q++)
      {
        if(*q!=celloffirstinneredge)
        {
          celloffirstinneredge = *q;
          found = true;
          break;
        }
      }
      if(ii<npatch)
      {
        assert(found);
        patchinfo.cells[iN][ii] = celloffirstinneredge;
      }
      else
      {
        assert(not found);
      }
      patchinfo.edgesinner[iN][ii] = firstinneredge;
    }
    for(int ii=0;ii<npatch2; ii++)
    {
      int iS = patchinfo.edgesinner[iN][ii];
      for(int iii=0;iii<getNNodesPerSide(iS);iii++)
      {
        int iM = getNodeIdOfSide(iS, iii);
        if (iM!=iN)
        {
          patchinfo.nodes[iN][ii] = iM;
          break;
        }
      }
    }
    for(int ii=0;ii<npatch; ii++)
    {
      int iK = patchinfo.cells[iN][ii];
      for(int iii=0;iii<getNSidesPerCell(iK);iii++)
      {
        int iS = getSideIdOfCell(iK, iii);
        bool found=false;
        for(int iiii=0;iiii<getNNodesPerSide(iS);iiii++)
        {
          int iM = getNodeIdOfSide(iS, iiii);
          if(iM==iN)
          {
            found=true;
            break;
          }
        }
        if(not found)
        {
          patchinfo.edgesouter[iN][ii] = iS;
        }
      }
    }

    // std::cerr << iN << " patchinfo.edgesinner[iN]="<<patchinfo.edgesinner[iN] << "\n";
    // std::cerr << iN << " patchinfo.nodes[iN]="<<patchinfo.nodes[iN] << "\n";
    // std::cerr << iN << " patchinfo.cells[iN]="<<patchinfo.cells[iN] << "\n";
    // std::cerr << iN << " patchinfo.edgesouter[iN]="<<patchinfo.edgesouter[iN] << "\n";

  }

  // arma::field<arma::mat> sidecoeffs;
  // arma::field<arma::imat> sideindices;

  patchinfo.sidecoeffs.set_size(nnodes);
  patchinfo.sideindices.set_size(nnodes);
  for(int iN=0; iN<nnodes;iN++)
  {
    int npatch = patchinfo.cells[iN].size();
    if(npatch != patchinfo.edgesinner[iN].size())
    {
      continue;
    }
    patchinfo.sidecoeffs[iN].set_size(npatch, 2);
    patchinfo.sideindices[iN].set_size(npatch, 2);


    arma::mat S(npatch, 2);
    const Alat::Node& N = getNode(iN);
    for(int ip=0;ip<npatch;ip++)
    {
      Alat::Node M = getNode(patchinfo.nodes[iN][ip]);
      M.add(-1.0, N);
      S(ip, 0) = M.x();
      S(ip, 1) = M.y();
    }
    arma::mat A(2, 2);
    arma::vec x(2), b(2);
    for(int ip=0;ip<npatch;ip++)
    {
      bool found=false;
      for(int ii=0;ii<npatch-2;ii++)
      {
        int iq0 = (ip + ii + 1)%npatch;
        int iq1 = (ip + ii + 2)%npatch;
        b[0] = S(ip, 0) * S(iq0, 0) + S(ip, 1) * S(iq0, 1);
        b[1] = S(ip, 0) * S(iq1, 0) + S(ip, 1) * S(iq1, 1);
        A(0,0) = S(iq0, 0) * S(iq0, 0) + S(iq0, 1) * S(iq0, 1);
        A(0,1) = S(iq0, 0) * S(iq1, 0) + S(iq0, 1) * S(iq1, 1);
        A(1,0) = A(0,1);
        A(1,1) = S(iq1, 0) * S(iq1, 0) + S(iq1, 1) * S(iq1, 1);
        x = arma::solve(A, b);
        if(x[0]<=0.0 and x[1]<=0.0)
        {
          patchinfo.sidecoeffs[iN](ip, 0) = x[0];
          patchinfo.sidecoeffs[iN](ip, 1) = x[1];
          patchinfo.sideindices[iN](ip, 0) = patchinfo.nodes[iN][iq0];
          patchinfo.sideindices[iN](ip, 1) = patchinfo.nodes[iN][iq1];
          found = true;
          break;
        }
      }
      assert(found);
    }
    // std::cerr << iN << " patchinfo.sidecoeffs[iN]="<<patchinfo.sidecoeffs[iN] << "\n";
    // std::cerr << iN << " patchinfo.sideindices[iN]="<<patchinfo.sideindices[iN] << "\n";
  }
}
