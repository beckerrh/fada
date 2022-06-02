#include  "Alat/assemblevector.h"
#include  "Fada/dofinformationwithoutfem.h"
#include  "Fada/variableinterface.h"
#include  "Alat/intvector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DofInformationWithoutFem::~DofInformationWithoutFem()
{}

DofInformationWithoutFem::DofInformationWithoutFem() : DofInformationInterface()
{}

DofInformationWithoutFem::DofInformationWithoutFem( const DofInformationWithoutFem& dofinformationwithoutfem) : DofInformationInterface(dofinformationwithoutfem)
{
  ( *this ).operator=(dofinformationwithoutfem);
}

DofInformationWithoutFem& DofInformationWithoutFem::operator=( const DofInformationWithoutFem& dofinformationwithoutfem)
{
  DofInformationInterface::operator=(dofinformationwithoutfem);
  assert(0);
  return *this;
}

std::string DofInformationWithoutFem::getName() const
{
  return "DofInformationWithoutFem";
}

DofInformationWithoutFem* DofInformationWithoutFem::clone() const
{
  return new DofInformationWithoutFem(*this);
}

/*--------------------------------------------------------------------------*/
const LocalDofInformation* DofInformationWithoutFem::getLocalDofInformation() const
{                         
  return NULL;
  assert(0);
}

void DofInformationWithoutFem::setMeshLevel(int level)
{}

void DofInformationWithoutFem::basicInit(Fada::FemInterface* fem, const FadaMesh::MeshInterface* mesh)
{
  // _n = variable->getN();
}

int DofInformationWithoutFem::ndof() const
{
  return 1;
}

int DofInformationWithoutFem::ndof(const FadaMesh::MeshInterface* refinedmesh) const
{
  return 1;
}

int DofInformationWithoutFem::ndofpercell() const
{
  return 1;
}

int DofInformationWithoutFem::ndoflocal() const
{
  return 1;
}

int DofInformationWithoutFem::ndoflocalpatch() const
{
  return 1;
}

void DofInformationWithoutFem::getCellIndices(int iK, Alat::IntVector& indices) const
{
  indices.set_size(1);
  for(int i = 0; i < 1; i++)
  {
    indices[i] = i;
  }
}

void DofInformationWithoutFem::getCellIndicesInterpolateRefined(int iK, Alat::IntVector& indices, const FadaMesh::MeshInterface* refinedmesh, const FadaMesh::RefineInfo* refinfo) const
{
  indices.set_size(1);
  for(int i = 0; i < 1; i++)
  {
    indices[i] = i;
  }
}

void DofInformationWithoutFem::getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const
{
  assert(0);
}

FemInterface* DofInformationWithoutFem::getFem() const
{
  return NULL;
}

void DofInformationWithoutFem::interpolate(int iK, Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const
{
  int ncomp = uh.ncomp();
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int i = 0; i < 1; i++)
    {
      // uh[icomp][i] = uH[icomp][i];
      uh(icomp,i) = uH(icomp,i);
    }
  }
}

const Alat::DoubleMatrix& DofInformationWithoutFem::getInterpolationMatrix() const
{
  assert(0);
}

const Alat::DoubleMatrix& DofInformationWithoutFem::getProjectionMatrix() const
{
  assert(0);
}
