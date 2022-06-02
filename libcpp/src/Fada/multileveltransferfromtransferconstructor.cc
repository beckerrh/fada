#include  "Fada/multileveltransferfromtransferconstructor.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "Fada/feminterface.h"
#include  "Fada/dofinformation.h"
#include  "FadaMesh/refineinfo.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferFromTransferConstructor::~MultiLevelTransferFromTransferConstructor()
{
}

MultiLevelTransferFromTransferConstructor::MultiLevelTransferFromTransferConstructor(): Fada::MultiLevelTransferSingleFemInterface()
{
}

MultiLevelTransferFromTransferConstructor::MultiLevelTransferFromTransferConstructor( const MultiLevelTransferFromTransferConstructor& multileveltransferfromtransferconstructor): Fada::MultiLevelTransferSingleFemInterface(multileveltransferfromtransferconstructor)
{
(*this).operator=(multileveltransferfromtransferconstructor);
}

MultiLevelTransferFromTransferConstructor& MultiLevelTransferFromTransferConstructor::operator=( const MultiLevelTransferFromTransferConstructor& multileveltransferfromtransferconstructor)
{
Fada::MultiLevelTransferSingleFemInterface::operator=(multileveltransferfromtransferconstructor);
assert(0);
return *this;
}

std::string MultiLevelTransferFromTransferConstructor::getName() const
{
return "MultiLevelTransferFromTransferConstructor";
}

MultiLevelTransferFromTransferConstructor* MultiLevelTransferFromTransferConstructor::clone() const
{
return new MultiLevelTransferFromTransferConstructor(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferFromTransferConstructor::basicInit(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem)
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( mesh );
  assert(mlmesh);
  Fada::DofInformation dofinfo;
  Fada::FemInterface* fem2 = fem->clone();
  dofinfo.basicInit(fem2, mlmesh);
  int nlevels = mlmesh->getNLevels();
  _matrixtransfer.set_size(nlevels-1);
  _matrixproject.set_size(nlevels-1);
  for(int level = 0; level < nlevels-1; level++)
  {
    std::cerr << "MultiLevelTransferFromTransferConstructor ### from " << level << " to " << level+1 << "\n";
    mlmesh->setResolution(level+1);
    dofinfo.setMeshLevel(level+1);
    const FadaMesh::MeshInterface* coarsemesh = mlmesh->getMesh(level+1);
    const FadaMesh::MeshInterface* finemesh = mlmesh->getMesh(level);
    _constructWithTransferConstructor(&dofinfo, _matrixtransfer[level], _matrixproject[level], coarsemesh, finemesh);
  }
}
void MultiLevelTransferFromTransferConstructor::prolongate(int level, Alat::VariableVectorInterface* ufine, const Alat::VariableVectorInterface* ucoarse, double d) const
{
  assert(0);
}
void MultiLevelTransferFromTransferConstructor::restrict(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine ) const
{
  assert(0);
}
void MultiLevelTransferFromTransferConstructor::project(int level, Alat::VariableVectorInterface* ucoarse, const Alat::VariableVectorInterface* ufine) const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferFromTransferConstructor::_constructWithTransferConstructor(const Fada::DofInformation* dofinfo, Alat::VariableMatrix& matrixtransfer, Alat::VariableMatrix& matrixproject, const FadaMesh::MeshInterface* coarsemesh, const FadaMesh::MeshInterface* finemesh)
{
  const FadaMesh::GeometryObject* geo = coarsemesh->getGeometryObject("RefineInfo");
  const FadaMesh::RefineInfo* refinfo = dynamic_cast<const FadaMesh::RefineInfo*>( geo );
  // std::cerr << "_constructWithTransferConstructor fine coarse " << finemesh->getNNodes()<< " " << coarsemesh->getNNodes()<<"\n";
  int ncells = coarsemesh->getNCells();
  Alat::IntVector olddof, newdof;
  int ndofnew = dofinfo->ndof(finemesh);
  // std::cerr << "_constructWithTransferConstructor ndofnew " << ndofnew<<"\n";
  Alat::SparsityPatternSoft sparsitypatternsoft(ndofnew);
  for(int iK = 0; iK < ncells; iK++)
  {
    dofinfo->getCellIndices(iK, olddof);
    dofinfo->getCellIndicesInterpolateRefined(iK, newdof, finemesh, refinfo);
    // std::cerr << "_constructWithTransferConstructor olddof " << olddof << " newdof " << newdof<<"\n";
    for(int ii = 0; ii < newdof.size(); ii++)
    {
      int inew = newdof[ii];
      for(int jj = 0; jj < olddof.size(); jj++)
      {
        sparsitypatternsoft[inew].insert(olddof[jj]);
      }
    }
  }
  matrixtransfer.set_size(sparsitypatternsoft);
  matrixproject.set_size(sparsitypatternsoft);
  const Alat::SparsityPattern& SP = *matrixtransfer.getSparsityPattern();
  Alat::DoubleVector& mattransfer = matrixtransfer.getSimpleMatrix();
  Alat::DoubleVector& matproject = matrixproject.getSimpleMatrix();


  mattransfer.zeros();
  matproject.zeros();
  Alat::IntVector count(ndofnew);
  count.zeros();
  const Alat::DoubleMatrix& interpolmatrix = dofinfo->getInterpolationMatrix();
  const Alat::DoubleMatrix& projectionmatrix = dofinfo->getProjectionMatrix();

  for(int iK = 0; iK < ncells; iK++)
  {
    dofinfo->getCellIndices(iK, olddof);
    dofinfo->getCellIndicesInterpolateRefined(iK, newdof, finemesh, refinfo);
    if( olddof.size() == newdof.size() )
    {
      for(int ii = 0; ii < newdof.size(); ii++)
      {
        count[newdof[ii]]++;
        int inew = newdof[ii];
        for(int pos = SP.rowstart(inew); pos < SP.rowstop(inew); pos++)
        {
          if(SP.col(pos) == olddof[ii])
          {
            mattransfer[pos] += 1.0;
            matproject[pos] += 1.0;
          }
        }
      }
    }
    else
    {
      if( interpolmatrix.n_rows != olddof.size() )
      {
        _error_string( "basicInit", "M.n() olddof.size()", interpolmatrix.n_rows, olddof.size() );
      }
      if( interpolmatrix.n_cols != newdof.size() )
      {
        _error_string( "basicInit", "M.m() newdof.size()", interpolmatrix.n_cols, newdof.size() );
      }
      for(int ii = 0; ii < newdof.size(); ii++)
      {
        count[newdof[ii]]++;
        int inew = newdof[ii];
        for(int jj = 0; jj < olddof.size(); jj++)
        {
          for(int pos = SP.rowstart(inew); pos < SP.rowstop(inew); pos++)
          {
            if(SP.col(pos) == olddof[jj])
            {
              mattransfer[pos] += interpolmatrix(jj, ii);
              matproject[pos] += projectionmatrix(jj, ii);
            }
          }
        }
      }
    }
  }
  for(int i = 0; i < ndofnew; i++)
  {
    for(int pos = SP.rowstart(i); pos < SP.rowstop(i); pos++)
    {
      mattransfer[pos] /= (double) count[i];
      matproject[pos] /= (double) count[i];
    }
  }
  matrixtransfer.suppressZeros();
  matrixproject.suppressZeros();
}
