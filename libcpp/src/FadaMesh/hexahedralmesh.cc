#include  "FadaMesh/hexahedralmesh.h"
#include  <algorithm>
#include  <fstream>

using namespace FadaMesh;
using namespace std;

Alat::FixArray<4, int> HexahedralMesh::_ind;
FadaMesh::Hexahedron HexahedralMesh::_hexahedron;

/*--------------------------------------------------------------------------*/

HexahedralMesh::HexahedralMesh() : FadaMesh::FadaMeshBase3d<8, 6, 12, 4>( )
{}

/*--------------------------------------------------------------------------*/

int HexahedralMesh::getLocalNodeIndiceOfSide(int ii, int isl) const
{
  return _hexahedron.getLocalNodeIndiceOfSide(ii, isl);
}

/*--------------------------------------------------------------------------*/

std::string HexahedralMesh::getName() const
{
  return "FadaMesh::HexahedralMesh";
}

std::string HexahedralMesh::getCellType() const
{
  return "Hexahedral";
}

int HexahedralMesh::getBoundaryVtkType() const
{
  // quad
  return 9;
}

FadaMeshEnums::meshtype HexahedralMesh::getType() const
{
  return FadaMeshEnums::HexahedralMesh;
}

//
// /*--------------------------------------------------------------------------*/
//
// void HexahedralMesh::getMeshSizeForStabilization(double& hs, int iS, int iK, int iil) const
// {
//   const HexahedralMesh::Side& side = _getSideOfCell(iK, iil);
//   Node N0 = getNode(side[0]);
//   double dmax = 0.0;
//   for(int i = 1; i < 4; i++)
//   {
//     Node Ni = getNode(side[i]);
//     Ni -= N0;
//     dmax = max( dmax, Ni.norm() );
//   }
//   hs = dmax;
// }

/*--------------------------------------------------------------------------*/

int HexahedralMesh::getCouplingOffset(int iS) const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/


Alat::Vector<HexahedralMesh::Hexahedral>& HexahedralMesh::getHexahedrals()
{
  return getCells();
}

const Alat::Vector<HexahedralMesh::Hexahedral>& HexahedralMesh::getHexahedrals() const
{
  return getCells();
}

const HexahedralMesh::Hexahedral& HexahedralMesh::getHexahedral(int i) const
{
  return getCell(i);
}

void HexahedralMesh::read(std::string filename)
{
  checkGeoFile(filename);
  readHex(filename);
}

void HexahedralMesh::write(std::string filename) const
{
  writeHex(filename);
}

HexahedralMesh::Side HexahedralMesh::getSideOfCell(int i, int ii) const
{
  return _getSideOfCell(i, ii);
}

int HexahedralMesh::getNodeIdOfSideOfCell(int iK, int iis, int ii) const
{
  int iln = getLocalNodeIndiceOfSide(ii, iis);
  return getNodeIdOfCell(iK, iln);
}

/*--------------------------------------------------------------------------*/

const HexahedralMesh::Side& HexahedralMesh::_getSideOfCell(int i, int ii) const
{
  if(ii == 0)
  {
    _S[0] = getNodeIdOfCell(i, 0);
    _S[1] = getNodeIdOfCell(i, 1);
    _S[2] = getNodeIdOfCell(i, 2);
    _S[3] = getNodeIdOfCell(i, 3);
  }
  else if(ii == 1)
  {
    _S[0] = getNodeIdOfCell(i, 0);
    _S[1] = getNodeIdOfCell(i, 4);
    _S[2] = getNodeIdOfCell(i, 5);
    _S[3] = getNodeIdOfCell(i, 1);
  }
  else if(ii == 2)
  {
    _S[0] = getNodeIdOfCell(i, 0);
    _S[1] = getNodeIdOfCell(i, 3);
    _S[2] = getNodeIdOfCell(i, 7);
    _S[3] = getNodeIdOfCell(i, 4);
  }
  else if(ii == 3)
  {
    _S[0] = getNodeIdOfCell(i, 1);
    _S[1] = getNodeIdOfCell(i, 5);
    _S[2] = getNodeIdOfCell(i, 6);
    _S[3] = getNodeIdOfCell(i, 2);
  }
  else if(ii == 4)
  {
    _S[0] = getNodeIdOfCell(i, 3);
    _S[1] = getNodeIdOfCell(i, 2);
    _S[2] = getNodeIdOfCell(i, 6);
    _S[3] = getNodeIdOfCell(i, 7);
  }
  else if(ii == 5)
  {
    _S[0] = getNodeIdOfCell(i, 4);
    _S[1] = getNodeIdOfCell(i, 7);
    _S[2] = getNodeIdOfCell(i, 6);
    _S[3] = getNodeIdOfCell(i, 5);
  }
  return _S;
}

/*--------------------------------------------------------------------------*/

const HexahedralMesh::Edge& HexahedralMesh::_getEdgeOfCell(int i, int ii) const
{
  if(ii == 0)
  {
    _E[0] = getNodeIdOfCell(i, 0);
    _E[1] = getNodeIdOfCell(i, 1);
  }
  else if(ii == 1)
  {
    _E[0] = getNodeIdOfCell(i, 1);
    _E[1] = getNodeIdOfCell(i, 2);
  }
  else if(ii == 2)
  {
    _E[0] = getNodeIdOfCell(i, 2);
    _E[1] = getNodeIdOfCell(i, 3);
  }
  else if(ii == 3)
  {
    _E[0] = getNodeIdOfCell(i, 3);
    _E[1] = getNodeIdOfCell(i, 0);
  }

  else if(ii == 4)
  {
    _E[0] = getNodeIdOfCell(i, 4);
    _E[1] = getNodeIdOfCell(i, 0);
  }
  else if(ii == 5)
  {
    _E[0] = getNodeIdOfCell(i, 1);
    _E[1] = getNodeIdOfCell(i, 5);
  }
  else if(ii == 6)
  {
    _E[0] = getNodeIdOfCell(i, 6);
    _E[1] = getNodeIdOfCell(i, 2);
  }
  else if(ii == 7)
  {
    _E[0] = getNodeIdOfCell(i, 3);
    _E[1] = getNodeIdOfCell(i, 7);
  }

  else if(ii == 8)
  {
    _E[0] = getNodeIdOfCell(i, 4);
    _E[1] = getNodeIdOfCell(i, 5);
  }
  else if(ii == 9)
  {
    _E[0] = getNodeIdOfCell(i, 5);
    _E[1] = getNodeIdOfCell(i, 6);
  }
  else if(ii == 10)
  {
    _E[0] = getNodeIdOfCell(i, 6);
    _E[1] = getNodeIdOfCell(i, 7);
  }
  else if(ii == 11)
  {
    _E[0] = getNodeIdOfCell(i, 7);
    _E[1] = getNodeIdOfCell(i, 4);
  }

  return _E;
}

/*--------------------------------------------------------------------------*/

const HexahedralMesh::Edge& HexahedralMesh::_getEdgeOfSide(int i, int ii) const
{
  if(ii == 0)
  {
    _E[0] = getNodeIdOfSide(i, 0);
    _E[1] = getNodeIdOfSide(i, 1);
  }
  else if(ii == 1)
  {
    _E[0] = getNodeIdOfSide(i, 1);
    _E[1] = getNodeIdOfSide(i, 2);
  }
  else if(ii == 2)
  {
    _E[0] = getNodeIdOfSide(i, 2);
    _E[1] = getNodeIdOfSide(i, 3);
  }
  else if(ii == 3)
  {
    _E[0] = getNodeIdOfSide(i, 3);
    _E[1] = getNodeIdOfSide(i, 0);
  }

  return _E;
}

/*--------------------------------------------------------------------------*/

void HexahedralMesh::writeHex(std::string filename) const
{
  string name = filename;
  name += ".hex";

  ofstream file( name.c_str() );
  assert( file.is_open() );

  file << getNNodes() << " ascii\n";
  for(int i = 0; i < getNNodes(); i++)
  {
    file << getNode(i) << "\n";
  }
  file << "\n";
  file << getNCells() << " ascii\n";
  for(int i = 0; i < getNCells(); i++)
  {
    for(int ii = 0; ii < getNNodesPerCell(i); ii++)
    {
      file << getNodeIdOfCell(i, ii) << " ";
    }
    file << "\n";
  }
  file << "\n";

  const FadaMesh::BoundaryInfo* BI = getBoundaryInfo();
  const Alat::IntVector& colors = BI->getColors();

  int nbdrysides = 0;
  for(int i = 0; i < colors.size(); i++)
  {
    int color = colors[i];
    nbdrysides += BI->getSidesOfColor(color).size();
  }
  file << nbdrysides << " ascii\n";
  for(int i = 0; i < colors.size(); i++)
  {
    int color = colors[i];
    const Alat::IntVector& sides = BI->getSidesOfColor(color);
    for(int j = 0; j < sides.size(); j++)
    {
      for(int ii = 0; ii < getNNodesPerSide(0); ii++)
      {
        file << getNodeIdOfSide(sides[j], ii) << " ";
      }
      file << color << std::endl;
    }
  }
  const CurvedBoundaryInformation* curvedboundaryinformation = getCurvedBoundaryInformation();
  assert(curvedboundaryinformation);
  curvedboundaryinformation->writeCurvedBoundaryDescription(file);
}

/*--------------------------------------------------------------------------*/

void HexahedralMesh::readHex(std::string filename)
{
  string name = filename;
  // name += ".hex";

  ifstream file( name.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** HexahedralMesh::readHex() : cannot read file \""<<filename<< "\"";
    assert(0);
  }

  streampos nodestart;
  string toto;
  while(1)
  {
    nodestart = file.tellg();
    getline(file, toto);
    if(toto[0] != '#')
    {
      break;
    }
  }
  file.seekg(nodestart);

  getAllNodes().loadFada(file);
  // std::cerr << "nodes " << getNodes() << "\n";

  // on n'utilse pas ma fonction read pour les quads, car on veut lire que les nodeids !
  Alat::Vector<Hexahedral>& cells = getHexahedrals();
  int nc;
  std::string datatype;
  file >> nc >> datatype;
  cells.set_size(nc);
  std::cerr << "@@@ nc " << nc << "\n";
  for(int i = 0; i < nc; i++)
  {
    for(int ii = 0; ii < 8; ii++)
    {
      file >> cells[i][ii];
    }
  }
  // std::cerr << "cells " << cells << "\n";

  HexahedralMesh::BoundarySideToColor bstc;
  HexahedralMesh::Side S;
  int nb;
  file >> nb >> datatype;
  std::cerr << "@@@ nb " << nb << "\n";
  for(int i = 0; i < nb; i++)
  {
    for(int ii = 0; ii < 4; ii++)
    {
      file >> S[ii];
    }
    int color;
    file >> color;
    sort( S.begin(), S.end() );
    // std::cerr << "@@@ color " << color << " " << S << "\n";
    bstc[S] = color;
  }
  constructSidesFromCells(bstc);

  CurvedBoundaryInformation* curvedboundaryinformation = getCurvedBoundaryInformation();
  curvedboundaryinformation->readCurvedBoundaryDescription(file);
  file.close();
  curvedboundaryinformation->constructBoundaryInformation(this);
}

//
// /*----------------------------------------------------------*/
//
void HexahedralMesh::writeVtk(std::string filename) const
{
  string name = filename;
  name += ".vtk";

  ofstream file( name.c_str() );
  assert( file.is_open() );


  file << "# vtk DataFile Version 4.0 "<<std::endl;
  file << "output from HexahedralMesh" << std::endl;
  file << "ASCII" << std::endl;
  //     file << "binary" << std::endl;
  file << "DATASET UNSTRUCTURED_GRID" << std::endl;
  file << std::endl;

  int nn = getNNodes();

  file << "POINTS " << nn;
  file << " FLOAT" << std::endl;
  for(int i = 0; i < nn; i++)
  {
    const HexahedralMesh::Node& v = getNode(i);
    file << v.x() << " " << v.y() << " " << v.z() << " " << std::endl;
  }
  file << std::endl;

  int nc = getNCells();
  int nle = 8;
  int length = nc*( nle+1 );

  file << std::endl << "CELLS " << nc <<" " << length << std::endl;

  for(int ic = 0; ic < nc; ic++)
  {
    file << nle << " ";
    for(int ii = 0; ii < nle; ii++)
    {
      file << getNodeIdOfCell(ic, ii) << " ";
    }
    file << std::endl;
  }
  file << std::endl << "CELL_TYPES " << nc << std::endl;
  for(int ic = 0; ic < nc; ic++)
  {
    file << 12 << " ";
  }
  file << std::endl;

  file.close();
}

// /*----------------------------------------------------------*/
//
// void HexahedralMesh::writeBoundaryVtk(std::string filename) const
// {
//   string name = filename;
//   name += "-boundary.vtk";
//
//   ofstream file( name.c_str() );
//   assert( file.is_open() );
//
//   file << "# vtk DataFile Version 4.0 "<<std::endl;
//   file << "output from HexahedralMesh" << std::endl;
//   file << "ASCII" << std::endl;
//   //     file << "binary" << std::endl;
//   file << "DATASET UNSTRUCTURED_GRID" << std::endl;
//   file << std::endl;
//
//   int nn = getNNodes();
//
//   file << "POINTS " << nn;
//   file << " FLOAT" << std::endl;
//   for(int i = 0; i < nn; i++)
//   {
//     const HexahedralMesh::Node& v = getNode(i);
//     file << v.x() << " " << v.y() << " " << v.z() << " " << std::endl;
//   }
//   file << std::endl;
//
//   const FadaMesh::BoundaryInfo* BI = getBoundaryInfo();
//   int nsides = BI->getNSides();
//   const Alat::IntVector& colors = BI->getColors();
//
//   int nle = 4;
//   int length = nsides*( nle+1 );
//   file << std::endl << "CELLS " << nsides <<" " << length << std::endl;
//
//   for(int i = 0; i < colors.size(); i++)
//   {
//     int color = colors[i];
//     const Alat::IntVector& sides = BI->getSidesOfColor(color);
//     for(int j = 0; j < sides.size(); j++)
//     {
//       file << nle << " ";
//       for(int ii = 0; ii < nle; ii++)
//       {
//         file << getNodeIdOfSide(sides[j], ii) << " ";
//       }
//       file << std::endl;
//     }
//   }
//   file << std::endl << "CELL_TYPES " << nsides << std::endl;
//   for(int c = 0; c < nsides; c++)
//   {
//     file << 9 << " ";
//   }
//   file << std::endl;
//   file << std::endl << "CELL_DATA " << nsides << std::endl;
//   file << std::endl << "SCALARS " << " bdry_colors " << " int " << 1 << std::endl;
//   file << std::endl << "LOOKUP_TABLE default" << std::endl;
//   for(int i = 0; i < colors.size(); i++)
//   {
//     int color = colors[i];
//     const Alat::IntVector& sides = BI->getSidesOfColor(color);
//     for(int j = 0; j < sides.size(); j++)
//     {
//       file << color << " ";
//       file << std::endl;
//     }
//     file << std::endl;
//   }
//
//   file.close();
// }
