#include  "FadaMesh/fadameshbase1d.h"
#include  <fstream>

using namespace FadaMesh;
using namespace std;

/*-------------------------------------------------------*/

template<int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
Alat::Node FadaMeshBase1d<NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeOfCell(int iK) const
{
  Alat::Node v;
  double d = 1.0/ double(NODESPERCELL);
  for(int ii = 0; ii < NODESPERCELL; ii++)
  {
    const Alat::Node& vii = getNodeOfCell(iK, ii);
    v.x() += d*vii.x();
  }
  return v;
}

/*---------------------------------------------------------*/
/*---------------------------------------------------------*/

// line mesh
#define NODESPERCELL 2
#define SIDESPERCELL 2
#define NODESPERSIDE 1
template class FadaMesh::FadaMeshBase1d<NODESPERCELL, SIDESPERCELL, NODESPERSIDE>;
#undef NODESPERCELL
#undef SIDESPERCELL
#undef NODESPERSIDE
