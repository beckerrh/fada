#include  "Fada/indicesinpatch.h"
#include  "Alat/intvector.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IndicesInPatch::~IndicesInPatch()
{}
IndicesInPatch::IndicesInPatch()
{}
IndicesInPatch::IndicesInPatch( const IndicesInPatch& indicesinpatch)
{
  assert(0);
}

IndicesInPatch& IndicesInPatch::operator=( const IndicesInPatch& indicesinpatch)
{
  assert(0);
  return *this;
}

std::string IndicesInPatch::getName() const
{
  return "IndicesInPatch";
}

IndicesInPatch* IndicesInPatch::clone() const
{
  return new IndicesInPatch(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& Fada::operator<<(std::ostream& os, const IndicesInPatch& A)
{
  os << "ncellspatch="<<A.getNcellspatch()<<"\n";
  for(int iK = 0; iK < A.getNcellspatch(); iK++)
  {
    os << "iK=" << iK << " nodeindices=" <<A.nodeindices(iK) << "\n";
    os << "iK=" << iK << " edgeindices=" <<A.nodeindices(iK) << "\n";
    os << "iK=" << iK << " sideindices=" <<A.sideindices(iK) << "\n";
    os << "iK=" << iK << " cellindices=" <<A.cellindices(iK) << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
int IndicesInPatch::getNcellspatch() const
{
  return _nodesinpatch.size();
}

const Alat::IntVector& IndicesInPatch::nodeindices(int iK) const
{
  return _nodesinpatch[iK];
}

const Alat::IntVector& IndicesInPatch::edgeindices(int iK) const
{
  return _edgesinpatch[iK];
}

const Alat::IntVector& IndicesInPatch::sideindices(int iK) const
{
  return _sidesinpatch[iK];
}

const Alat::IntVector& IndicesInPatch::cellindices(int iK) const
{
  return _cellsinpatch[iK];
}

/*--------------------------------------------------------------------------*/

void IndicesInPatch::basicInit(std::string referenceelementname, const LocalDofInformation& dofinfo)
{
  _setDimensions(referenceelementname);

  _nodesinpatch.set_size(ncellspatch);
  _edgesinpatch.set_size(ncellspatch);
  _sidesinpatch.set_size(ncellspatch);
  _cellsinpatch.set_size(ncellspatch);
  for(int iK = 0; iK < ncellspatch; iK++)
  {
    _nodesinpatch[iK].set_size(nnodes);
    _edgesinpatch[iK].set_size(nedges);
    _sidesinpatch[iK].set_size(nsides);
    _cellsinpatch[iK].set_size(ncells);
  }

  if(referenceelementname == "Triangle")
  {
    _basicInitTri(dofinfo);
  }
  else if( referenceelementname == "Quadrilateral" )
  {
    _basicInitQuad(dofinfo);
  }
  else if( referenceelementname == "Hexahedral" )
  {
    _basicInitHex(dofinfo);
  }
  else
  {
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/
void IndicesInPatch::_setDimensions(std::string referenceelementname)
{
  if(referenceelementname == "Triangle")
  {
    ncells = 1;
    nsides = 3;
    nedges = 0;
    nnodes = 3;
    ncellspatch = 4;
    nsidespatch = 9;
    nedgespatch = 0;
    nnodespatch = 6;
  }
  else if( referenceelementname == "Quadrilateral" )
  {
    ncells = 1;
    nsides = 4;
    nedges = 0;
    nnodes = 4;
    ncellspatch = 4;
    nsidespatch = 12;
    nedgespatch = 0;
    nnodespatch = 9;
  }
  else if( referenceelementname == "Hexahedral" )
  {
    ncells = 1;
    nsides = 6;
    nedges = 12;
    nnodes = 8;
    ncellspatch = 8;
    nsidespatch = 32;
    nedgespatch = 54;
    nnodespatch = 27;
  }
  else
  {
    assert(0);
  }
}

/*--------------------------------------------------------------------------*/

void IndicesInPatch::_basicInitTri(const LocalDofInformation& dofinfo)
{
  // crisscross
  _cellsinpatch[0][0] = 0;
  _cellsinpatch[1][0] = 1;
  _cellsinpatch[2][0] = 2;

  _nodesinpatch[0][0] = 2;
  _nodesinpatch[0][1] = 5;
  _nodesinpatch[0][2] = 4;

  _nodesinpatch[1][0] = 4;
  _nodesinpatch[1][1] = 5;
  _nodesinpatch[1][2] = 0;

  _nodesinpatch[2][0] = 0;
  _nodesinpatch[2][1] = 3;
  _nodesinpatch[2][2] = 4;

  _nodesinpatch[3][0] = 4;
  _nodesinpatch[3][1] = 3;
  _nodesinpatch[3][2] = 1;
  
  return;
  
  
//         2
//        | \
//        |0 \
//        0---1
//        |\3 |\
//        |1\ |2\
//        4--3---5

//
//        2
//        | \
//        |  \
//        |   \
//        0----1
  _cellsinpatch[0][0] = 0;
  _cellsinpatch[1][0] = 1;
  _cellsinpatch[2][0] = 2;

  _nodesinpatch[0][0] = 0;
  _nodesinpatch[0][1] = 1;
  _nodesinpatch[0][2] = 2;

  _nodesinpatch[1][0] = 4;
  _nodesinpatch[1][1] = 3;
  _nodesinpatch[1][2] = 0;

  _nodesinpatch[2][0] = 3;
  _nodesinpatch[2][1] = 5;
  _nodesinpatch[2][2] = 1;

  _nodesinpatch[3][0] = 1;
  _nodesinpatch[3][1] = 0;
  _nodesinpatch[3][2] = 3;

  std::cerr << "IndicesInPatch::_basicInitTri() _sidesinpatch not written (and _nodesinpatch probably wrong)\n";  
}

/*--------------------------------------------------------------------------*/

void IndicesInPatch::_basicInitQuad(const LocalDofInformation& dofinfo)
{
  //        8 --10-- 5 -- 5-- 4
  //        |        |        |
  //       11   3    6    2   4
  //        |        |        |
  //        7 --8 -- 3 -- 2-- 2
  //        |        |        |
  //        9   0    3   1    1
  //        |        |        |
  //        6 --7 -- 0 ---0-- 1
  //

  // _cellsinpatch[0][0] = 1;
  // _cellsinpatch[1][0] = 3;
  // _cellsinpatch[2][0] = 2;
  // _cellsinpatch[3][0] = 0;

  _cellsinpatch[0][0] = 2;
  _cellsinpatch[1][0] = 0;
  _cellsinpatch[2][0] = 1;
  _cellsinpatch[3][0] = 3;

  // _cellsinpatch[0][0] = 0;
  // _cellsinpatch[1][0] = 1;
  // _cellsinpatch[2][0] = 2;
  // _cellsinpatch[3][0] = 3;

  _nodesinpatch[0][0] = 6;
  _nodesinpatch[0][1] = 0;
  _nodesinpatch[0][2] = 3;
  _nodesinpatch[0][3] = 7;

  _nodesinpatch[1][0] = 0;
  _nodesinpatch[1][1] = 1;
  _nodesinpatch[1][2] = 2;
  _nodesinpatch[1][3] = 3;

  _nodesinpatch[2][0] = 3;
  _nodesinpatch[2][1] = 2;
  _nodesinpatch[2][2] = 4;
  _nodesinpatch[2][3] = 5;

  _nodesinpatch[3][0] = 7;
  _nodesinpatch[3][1] = 3;
  _nodesinpatch[3][2] = 5;
  _nodesinpatch[3][3] = 8;

  _sidesinpatch[0][0] = 7;
  _sidesinpatch[0][1] = 3;
  _sidesinpatch[0][2] = 8;
  _sidesinpatch[0][3] = 9;

  _sidesinpatch[1][0] = 0;
  _sidesinpatch[1][1] = 1;
  _sidesinpatch[1][2] = 2;
  _sidesinpatch[1][3] = 3;

  _sidesinpatch[2][0] = 2;
  _sidesinpatch[2][1] = 4;
  _sidesinpatch[2][2] = 5;
  _sidesinpatch[2][3] = 6;

  _sidesinpatch[3][0] = 8;
  _sidesinpatch[3][1] = 6;
  _sidesinpatch[3][2] = 10;
  _sidesinpatch[3][3] = 11;
}

/*--------------------------------------------------------------------------*/

void IndicesInPatch::_basicInitHex(const LocalDofInformation& dofinfo)
{
  _cellsinpatch[0][0] = 0;
  _cellsinpatch[1][0] = 1;
  _cellsinpatch[2][0] = 2;
  _cellsinpatch[3][0] = 3;
  _cellsinpatch[4][0] = 4;
  _cellsinpatch[5][0] = 5;
  _cellsinpatch[6][0] = 6;
  _cellsinpatch[7][0] = 7;

//-------------------------------

  _nodesinpatch[0][0] = 0;
  _nodesinpatch[0][1] = 1;
  _nodesinpatch[0][2] = 4;
  _nodesinpatch[0][3] = 3;
  _nodesinpatch[0][4] = 9;
  _nodesinpatch[0][5] = 10;
  _nodesinpatch[0][6] = 13;
  _nodesinpatch[0][7] = 12;

  _nodesinpatch[1][0] = 1;
  _nodesinpatch[1][1] = 2;
  _nodesinpatch[1][2] = 5;
  _nodesinpatch[1][3] = 4;
  _nodesinpatch[1][4] = 10;
  _nodesinpatch[1][5] = 11;
  _nodesinpatch[1][6] = 14;
  _nodesinpatch[1][7] = 13;

  _nodesinpatch[2][0] = 3;
  _nodesinpatch[2][1] = 4;
  _nodesinpatch[2][2] = 7;
  _nodesinpatch[2][3] = 6;
  _nodesinpatch[2][4] = 12;
  _nodesinpatch[2][5] = 13;
  _nodesinpatch[2][6] = 16;
  _nodesinpatch[2][7] = 15;

  _nodesinpatch[3][0] = 4;
  _nodesinpatch[3][1] = 5;
  _nodesinpatch[3][2] = 8;
  _nodesinpatch[3][3] = 7;
  _nodesinpatch[3][4] = 13;
  _nodesinpatch[3][5] = 14;
  _nodesinpatch[3][6] = 17;
  _nodesinpatch[3][7] = 16;

  _nodesinpatch[4][0] =  9;
  _nodesinpatch[4][1] = 10;
  _nodesinpatch[4][2] = 13;
  _nodesinpatch[4][3] = 12;
  _nodesinpatch[4][4] = 18;
  _nodesinpatch[4][5] = 19;
  _nodesinpatch[4][6] = 22;
  _nodesinpatch[4][7] = 21;

  _nodesinpatch[5][0] = 10;
  _nodesinpatch[5][1] = 11;
  _nodesinpatch[5][2] = 14;
  _nodesinpatch[5][3] = 13;
  _nodesinpatch[5][4] = 19;
  _nodesinpatch[5][5] = 20;
  _nodesinpatch[5][6] = 23;
  _nodesinpatch[5][7] = 22;

  _nodesinpatch[6][0] = 12;
  _nodesinpatch[6][1] = 13;
  _nodesinpatch[6][2] = 16;
  _nodesinpatch[6][3] = 15;
  _nodesinpatch[6][4] = 21;
  _nodesinpatch[6][5] = 22;
  _nodesinpatch[6][6] = 25;
  _nodesinpatch[6][7] = 24;

  _nodesinpatch[7][0] = 13;
  _nodesinpatch[7][1] = 14;
  _nodesinpatch[7][2] = 17;
  _nodesinpatch[7][3] = 16;
  _nodesinpatch[7][4] = 22;
  _nodesinpatch[7][5] = 23;
  _nodesinpatch[7][6] = 26;
  _nodesinpatch[7][7] = 25;


  //  copie de Fada
  // _nodesinpatch[0][0] = 7;
  // _nodesinpatch[0][1] = 3;
  // _nodesinpatch[0][2] = 2;
  // _nodesinpatch[0][3] = 6;
  // _nodesinpatch[0][4] = 16;
  // _nodesinpatch[0][5] = 12;
  // _nodesinpatch[0][6] = 11;
  // _nodesinpatch[0][7] = 15;
  //
  // _nodesinpatch[1][0] = 3;
  // _nodesinpatch[1][1] = 0;
  // _nodesinpatch[1][2] = 1;
  // _nodesinpatch[1][3] = 2;
  // _nodesinpatch[1][4] = 12;
  // _nodesinpatch[1][5] = 9;
  // _nodesinpatch[1][6] = 10;
  // _nodesinpatch[1][7] = 11;
  //
  // _nodesinpatch[2][0] = 2;
  // _nodesinpatch[2][1] = 1;
  // _nodesinpatch[2][2] = 4;
  // _nodesinpatch[2][3] = 5;
  // _nodesinpatch[2][4] = 11;
  // _nodesinpatch[2][5] = 10;
  // _nodesinpatch[2][6] = 13;
  // _nodesinpatch[2][7] = 14;
  //
  // _nodesinpatch[3][0] = 6;
  // _nodesinpatch[3][1] = 2;
  // _nodesinpatch[3][2] = 5;
  // _nodesinpatch[3][3] = 8;
  // _nodesinpatch[3][4] = 15;
  // _nodesinpatch[3][5] = 11;
  // _nodesinpatch[3][6] = 14;
  // _nodesinpatch[3][7] = 17;
  //
  // _nodesinpatch[4][0] = 16;
  // _nodesinpatch[4][1] = 12;
  // _nodesinpatch[4][2] = 11;
  // _nodesinpatch[4][3] = 15;
  // _nodesinpatch[4][4] = 25;
  // _nodesinpatch[4][5] = 21;
  // _nodesinpatch[4][6] = 20;
  // _nodesinpatch[4][7] = 24;
  //
  // _nodesinpatch[5][0] = 12;
  // _nodesinpatch[5][1] = 9;
  // _nodesinpatch[5][2] = 10;
  // _nodesinpatch[5][3] = 11;
  // _nodesinpatch[5][4] = 21;
  // _nodesinpatch[5][5] = 18;
  // _nodesinpatch[5][6] = 19;
  // _nodesinpatch[5][7] = 20;
  //
  // _nodesinpatch[6][0] = 11;
  // _nodesinpatch[6][1] = 10;
  // _nodesinpatch[6][2] = 13;
  // _nodesinpatch[6][3] = 14;
  // _nodesinpatch[6][4] = 20;
  // _nodesinpatch[6][5] = 19;
  // _nodesinpatch[6][6] = 22;
  // _nodesinpatch[6][7] = 23;
  //
  // _nodesinpatch[7][0] = 15;
  // _nodesinpatch[7][1] = 11;
  // _nodesinpatch[7][2] = 14;
  // _nodesinpatch[7][3] = 17;
  // _nodesinpatch[7][4] = 24;
  // _nodesinpatch[7][5] = 20;
  // _nodesinpatch[7][6] = 23;
  // _nodesinpatch[7][7] = 26;
}
