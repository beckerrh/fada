#include  "Alat/assemblevector.h"
#include  "Fada/feminterface.h"
#include  "FadaMesh/meshinterface.h"
// #include  "Fada/variableinterface.h"
#include  "Fada/dofinformation.h"
#include  "Fada/transfermatrixconstructor.h"
#include  "FadaMesh/refineinfo.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DofInformation::~DofInformation()
{}
DofInformation::DofInformation() : DofInformationInterface(), _fem(NULL), _mesh(NULL)
{}
DofInformation::DofInformation( const DofInformation& dofinformation) : DofInformationInterface(dofinformation), _fem(dofinformation._fem), _mesh(dofinformation._mesh)
{}
DofInformation& DofInformation::operator=( const DofInformation& dofinformation)
{
  assert(0);
  return *this;
}

std::string DofInformation::getName() const
{
  return "DofInformation";
}

DofInformation* DofInformation::clone() const
{
  assert(0);
  // return new DofInformation(*this);
}

/*--------------------------------------------------------------------------*/
const Fada::LocalDofInformation* DofInformation::getLocalDofInformation() const
{
  return &_localdofinformation;
}

/*--------------------------------------------------------------------------*/

void DofInformation::setMeshLevel(int level)
{
  _level = level;
  _mesh->setResolution(level);
}

/*--------------------------------------------------------------------------*/

void DofInformation::basicInit(Fada::FemInterface* fem, const FadaMesh::MeshInterface* mesh)
{
  // std::cerr << "DofInformation::basicInit() fem="<<fem->getName()<<"\n";

  _fem = fem;
  _mesh = mesh;
  assert(_mesh);
  if(fem)
  {
    fem->setDofDescription(_localdofinformation);
    // std::cerr << "DofInformation::basicInit() _localdofinformation " << _localdofinformation << "\n";

    _indicesinpatch.basicInit(fem->getReferenceElementType(), _localdofinformation);

    _ndoflocal = _localdofinformation.nodes()*_indicesinpatch.nnodes + _localdofinformation.sides()*_indicesinpatch.nsides + _localdofinformation.edges()*_indicesinpatch.nedges + _localdofinformation.cells()*_indicesinpatch.ncells;
    _ndoflocalpatch = _localdofinformation.nodes()*_indicesinpatch.nnodespatch + _localdofinformation.sides()*_indicesinpatch.nsidespatch + _localdofinformation.edges()*_indicesinpatch.nedgespatch  + _localdofinformation.cells()*_indicesinpatch.ncellspatch;
  }
  else
  {
    // _ndoflocal = variable->getN();
    _ndoflocal = 1;
  }
  int nlevels = mesh->getNLevels();
  // std::cerr << "DofInformation::basicInit() nlevels="<<nlevels<<"\n";
  _ndofs.set_size(nlevels);
  _offsetnodes.set_size(nlevels);
  _offsetesges.set_size(nlevels);
  _offsetsides.set_size(nlevels);
  _offsetcells.set_size(nlevels);
  _ndofspercell.set_size(nlevels);
  for(int level = 0; level < nlevels; level++)
  {
    mesh->setResolution(level);
    if(fem)
    {
      _ndofs[level] = _localdofinformation.nodes()*_mesh->getNNodes() + _localdofinformation.edges()* _mesh->getNEdges() + _localdofinformation.sides()* _mesh->getNSides() + _localdofinformation.cells()* _mesh->getNCells();
      // std::cerr << "DofInformation::basicInit() mesh->getName() level _ndofs[level] " << mesh->getName() << " " << level << " " << _ndofs[level] << "\n";
      _offsetnodes[level] = 0;
      _offsetesges[level] = _offsetnodes[level] + _localdofinformation.nodes()* _mesh->getNNodes();
      _offsetsides[level] = _offsetesges[level] + _localdofinformation.edges()* _mesh->getNEdges();
      _offsetcells[level] = _offsetsides[level] + _localdofinformation.sides()* _mesh->getNSides();
      int iK = 0;
      _ndofspercell[level] = _localdofinformation.nodes()*_mesh->getNNodesPerCell(iK) + _localdofinformation.edges()* _mesh->getNEdgesPerCell(iK) + _localdofinformation.sides()* _mesh->getNSidesPerCell(iK) + _localdofinformation.cells();
      // std::cerr << "DofInformation::basicInit() level _ndofspercell[level] " << level << " " << _ndofspercell[level] << "\n";
    }
    else
    {
      // _ndofs[level] = variable->getN();
      _ndofs[level] = 1;
    }
  }

  if( ( fem ) && ( _mesh->getType() != FadaMeshEnums::TriangleMesh ) )
  // if(fem )
  {
    TransferMatrixConstructor(_interpolationmatrix, _projectionmatrix, this);
  }
}

/*--------------------------------------------------------------------------*/

const Alat::DoubleMatrix& DofInformation::getInterpolationMatrix() const
{
  return _interpolationmatrix;
}

const Alat::DoubleMatrix& DofInformation::getProjectionMatrix() const
{
  return _projectionmatrix;
}

/*--------------------------------------------------------------------------*/

int DofInformation::ndof() const
{
  // std::cerr << "DofInformation::ndof()="<<_ndofs<<" _level="<<_level<<"\n";
  return _ndofs[_level];
}

/*--------------------------------------------------------------------------*/

int DofInformation::ndof(const FadaMesh::MeshInterface* refinedmesh) const
{
  return _localdofinformation.nodes()*refinedmesh->getNNodes() + _localdofinformation.edges()* refinedmesh->getNEdges() + _localdofinformation.sides()* refinedmesh->getNSides() + _localdofinformation.cells()* refinedmesh->getNCells();
}

/*--------------------------------------------------------------------------*/

int DofInformation::ndofpercell() const
{
  return _ndofspercell[_level];
}

/*--------------------------------------------------------------------------*/

int DofInformation::ndoflocal() const
{
  return _ndoflocal;
}

/*--------------------------------------------------------------------------*/

int DofInformation::ndoflocalpatch() const
{
  return _ndoflocalpatch;
}

/*--------------------------------------------------------------------------*/

Fada::FemInterface* DofInformation::getFem() const
{
  return _fem;
}

/*--------------------------------------------------------------------------*/

void DofInformation::getCellIndices(int iK, Alat::IntVector& indices) const
{
  indices.set_size( ndofpercell() );
  // assert( indices.size() == ndofpercell() );

  int count = 0;
  for(int j = 0; j < _mesh->getNNodesPerCell(iK); j++)
  {
    // std::cerr << "j="<<j<<" _mesh->getNodeIdOfCell(iK, j)="<<_mesh->getNodeIdOfCell(iK, j)<<"\n";
    for(int i = 0; i < _localdofinformation.nodes(); i++)
    {
      indices[count++] = _offsetnodes[_level] + _mesh->getNodeIdOfCell(iK, j)*_localdofinformation.nodes() + i;
    }
  }
  for(int j = 0; j < _mesh->getNEdgesPerCell(iK); j++)
  {
    for(int i = 0; i < _localdofinformation.edges(); i++)
    {
      indices[count++] = _offsetesges[_level] + _mesh->getEdgeIdOfCell(iK, j)*_localdofinformation.edges() + i;
    }
  }
  for(int j = 0; j < _mesh->getNSidesPerCell(iK); j++)
  {
    for(int i = 0; i < _localdofinformation.sides(); i++)
    {
      indices[count++] = _offsetsides[_level] + _mesh->getSideIdOfCell(iK, j)*_localdofinformation.sides() + i;
    }
  }
  for(int i = 0; i < _localdofinformation.cells(); i++)
  {
    indices[count++] = _offsetcells[_level] + iK*_localdofinformation.cells() + i;
  }
  // std::cerr << "DofInformation::getCellIndices() indices="<<indices<<"\n";
}

/*--------------------------------------------------------------------------*/
void DofInformation::getCellIndicesInterpolate(int iK, Alat::IntVector& indices) const
{
  getCellIndices(iK, indices);
}

/*--------------------------------------------------------------------------*/

void DofInformation::getCellIndicesInterpolateRefined(int iK, Alat::IntVector& indices, const FadaMesh::MeshInterface* refinedmesh, const FadaMesh::RefineInfo* refinfo) const
{
  assert(_fem);
  int offsetnodes, offsetesges, offsetsides, offsetcells;
  offsetnodes = 0;
  offsetesges = offsetnodes + _localdofinformation.nodes()* refinedmesh->getNNodes();
  offsetsides = offsetesges + _localdofinformation.edges()* refinedmesh->getNEdges();
  offsetcells = offsetsides + _localdofinformation.sides()* refinedmesh->getNSides();

  // const FadaMesh::GeometryObject* geo = refinedmesh->getGeometryObject("RefineInfo");
  // const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  assert(refinfo);
  int ncells = refinfo->getNCoarseCells(iK);
  int nnodes = refinfo->getNCoarseNodes(iK);
  int nsides = refinfo->getNCoarseSides(iK);
  int nedges = refinfo->getNCoarseEdges(iK);


  int ndoffine =  _localdofinformation.nodes()*nnodes + _localdofinformation.edges()*nedges + _localdofinformation.sides()*nsides + _localdofinformation.cells()*ncells;
  // std::cerr << "ndoffine = " << ndoffine << " nnodes = " << nnodes  << "\n";

  indices.set_size(ndoffine);

  int count = 0;
  for(int j = 0; j < nnodes; j++)
  {
    for(int i = 0; i < _localdofinformation.nodes(); i++)
    {
      indices[count++] = offsetnodes + refinfo->getCoarseNodesNumber(iK, j)*_localdofinformation.nodes() + i;
    }
  }
  for(int j = 0; j < nedges; j++)
  {
    for(int i = 0; i < _localdofinformation.edges(); i++)
    {
      indices[count++] = offsetesges + refinfo->getCoarseEdgesNumber(iK, j)*_localdofinformation.edges() + i;
    }
  }
  for(int j = 0; j < nsides; j++)
  {
    for(int i = 0; i < _localdofinformation.sides(); i++)
    {
      indices[count++] = offsetsides + refinfo->getCoarseSideNumber(iK, j)*_localdofinformation.sides() + i;
    }
  }
  for(int j = 0; j < ncells; j++)
  {
    for(int i = 0; i < _localdofinformation.cells(); i++)
    {
      indices[count++] = offsetcells + refinfo->getCoarseCellNumber(iK, j)*_localdofinformation.cells() + i;
    }
  }
}

/*---------------------------------------------------------*/
void DofInformation::getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const
{
  int offsetnodes, offsetesges, offsetsides, offsetcells;
  offsetnodes = 0;
  offsetesges = offsetnodes + _localdofinformation.nodes()* _indicesinpatch.nnodespatch;
  offsetsides = offsetesges + _localdofinformation.edges()* _indicesinpatch.nedgespatch;
  offsetcells = offsetsides + _localdofinformation.sides()* _indicesinpatch.nsidespatch;

  indices.set_size(_ndoflocal);
  int count = 0;
  for(int j = 0; j < _indicesinpatch.nnodes; j++)
  {
    for(int i = 0; i < _localdofinformation.nodes(); i++)
    {
      indices[count++] = offsetnodes + _indicesinpatch.nodeindices(iK)[j]*_localdofinformation.nodes() + i;
    }
  }
  for(int j = 0; j < _indicesinpatch.nedges; j++)
  {
    for(int i = 0; i < _localdofinformation.edges(); i++)
    {
      indices[count++] = offsetesges + _indicesinpatch.edgeindices(iK)[j]*_localdofinformation.edges() + i;
    }
  }
  for(int j = 0; j < _indicesinpatch.nsides; j++)
  {
    for(int i = 0; i < _localdofinformation.sides(); i++)
    {
      indices[count++] = offsetsides + _indicesinpatch.sideindices(iK)[j]*_localdofinformation.sides() + i;
    }
  }
  for(int j = 0; j < _indicesinpatch.ncells; j++)
  {
    for(int i = 0; i < _localdofinformation.cells(); i++)
    {
      indices[count++] = offsetcells + _indicesinpatch.cellindices(iK)[j]*_localdofinformation.cells() + i;
    }
  }
  // std::cerr<<"*** DofInformation::getLocalCellIndicesPatch() offsetnodes offsetesges offsetsides offsetcells " << offsetnodes << " " << offsetesges << " " <<  offsetsides << " " << offsetcells << "\n";
  // std::cerr<<"*** DofInformation::getLocalCellIndicesPatch() iK " << iK << " -> " << indices << "\n";
  // std::cerr<<"*** DofInformation::getLocalCellIndicesPatch() _indicesinpatch " << _indicesinpatch << "\n";
}

/*---------------------------------------------------------*/

void DofInformation::interpolate(int iK, Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const
{
  if( _fem->interpolationMatrixWrong() )
  {
    _fem->interpolate(uh, uH);
    return;
  }
  int ncomp = uh.ncomp();
  int n = uh.nloc();
  int m = uH.nloc();
  assert(n == _interpolationmatrix.n_cols);
  assert(m == _interpolationmatrix.n_rows);
  assert(uH.ncomp() == ncomp);
  uh.zeros();
  // std::cerr << "DofInformation::interpolate() " << _interpolationmatrix << "\n";
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < m; j++)
      {
        uh(icomp, i) += _interpolationmatrix(j, i)*uH(icomp, j);
      }
      // _interpolationmatrix.productVectorTransposed(uh[icomp], uH[icomp]);
    }
  }
}

/*---------------------------------------------------------*/

void DofInformation::project(int iK, Alat::AssembleVector& uH, const Alat::AssembleVector& uh) const
{
  int ncomp = uh.ncomp();
  int n = uh.nloc();
  assert(uH.ncomp() == ncomp);
  uH.zeros();
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    // _projectionmatrix.productVector(uH[icomp], uh[icomp]);
    for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < n; j++)
      {
        uH(icomp, i) += _projectionmatrix(i, j)*uh(icomp, j);
      }
    }
  }
  // std::cerr<<"uh --> uH "<<uh<<" --> "<<uH<<"\n";
}

/*---------------------------------------------------------*/

void DofInformation::setDofIdsOnSide(int iS, Alat::IntVector& ids) const
{
  // cette fonction calcule les dofs globaux pour un side, on suppose que
  // le side est sur un bord ou bien est un Hanging side
  assert(_mesh->getCellIdOfSide(iS, 1) < 0);
  Alat::IntVector localids;

  setLocalDofIdsOnSide(iS, localids);

  ids.set_size( localids.size() );
  int iK = _mesh->getCellIdOfSide(iS, 0);
  Alat::IntVector cellids;
  getCellIndices(iK, cellids);
  for(int ii = 0; ii < localids.size(); ii++)
  {
    ids[ii] = cellids[localids[ii]];
  }
}

/*---------------------------------------------------------*/

void DofInformation::setLocalDofIdsOnSide(int iS, Alat::IntVector& ids) const
{
  // cette fonction calcule les dofs locaux pour un side, on suppose que
  // le side est sur un bord ou bien est un Hanging side
  assert(_mesh->getCellIdOfSide(iS, 1) < 0);
  int nn = _mesh->getNNodesPerSide(iS);
  int ne = _mesh->getNEdgesPerSide(iS);
  // int ndof_on_node = getNDofOnNode();
  // int ndof_on_edge = getNDofOnEdge();
  // int ndof_on_side = getNDofOnSide();
  int ndof_on_node = _localdofinformation.nodes();
  int ndof_on_edge = _localdofinformation.edges();
  int ndof_on_side = _localdofinformation.sides();
  int ndoflocal = ndof_on_node*nn+ndof_on_edge*ne+ndof_on_side;
  ids.set_size(ndoflocal);
  int ind = 0;
  int iK = _mesh->getCellIdOfSide(iS, 0);
  int localdof_compt = 0;
  for(int ii = 0; ii < nn; ii++)
  {
    int in = _mesh->getNodeIdOfSide(iS, ii);
    int local_index = _mesh->localIndexOfNodeInCell(iK, in);
    for(int idof = 0; idof < ndof_on_node; idof++)
    {
      ids[ind] = idof+ndof_on_node*local_index;
      ind++;
    }
  }
  localdof_compt += _mesh->getNNodesPerCell(iK)*ndof_on_node;
  for(int ii = 0; ii < ne; ii++)
  {
    int ie = _mesh->getEdgeIdOfSide(iS, ii);
    int local_index = _mesh->localIndexOfEdgeInCell(iK, ie);
    for(int idof = 0; idof < ndof_on_edge; idof++)
    {
      ids[ind] = localdof_compt+idof+ndof_on_edge*local_index;
      ind++;
    }
  }
  localdof_compt += _mesh->getNEdgesPerCell(iK)*ndof_on_node;
  int local_index = _mesh->localIndexOfSideInCell(iK, iS);
  for(int idof = 0; idof < ndof_on_side; idof++)
  {
    ids[ind] = localdof_compt+idof+ndof_on_side*local_index;
    ind++;
  }
}
