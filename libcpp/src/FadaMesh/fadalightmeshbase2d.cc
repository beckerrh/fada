#include  "FadaMesh/fadameshbase2d.h"
#include  <fstream>

using namespace FadaMesh;
using namespace std;

/*---------------------------------------------------------*/

template<int NODESPERCELL>
FadaMeshBase2d<NODESPERCELL>::FadaMeshBase2d() : FadaMeshBase<2, NODESPERCELL, NODESPERCELL, 2>( )
{}

template<int NODESPERCELL>
std::string FadaMeshBase2d<NODESPERCELL>::getName() const
{
  return "FadaMesh::FadaMeshBase2d";
}

/*---------------------------------------------------------*/

template<int NODESPERCELL>
int FadaMeshBase2d<NODESPERCELL>::getNEdgesPerCell(int i) const
{
  return 0;
}

/*---------------------------------------------------------*/

template<int NODESPERCELL>
int FadaMeshBase2d<NODESPERCELL>::getNEdgesPerSide(int i) const
{
  return 0;
}

/*---------------------------------------------------------*/

template<int NODESPERCELL>
int FadaMeshBase2d<NODESPERCELL>::getVtkType() const
{
  if(NODESPERCELL == 3)
  {
    return 5;
  }
  else
  {
    return 9;
  }
}

/*---------------------------------------------------------*/

template<int NODESPERCELL>
int FadaMeshBase2d<NODESPERCELL>::getBoundaryVtkType() const
{
  return 3;
}

/*---------------------------------------------------------*/

// triangle mesh
template class FadaMesh::FadaMeshBase2d<3>;

// quadrilateral mesh
template class FadaMesh::FadaMeshBase2d<4>;
