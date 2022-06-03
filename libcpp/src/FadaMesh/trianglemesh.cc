#include  "FadaMesh/trianglemesh.h"
#include  <fstream>
#include  <algorithm>
#include  <math.h>

using namespace FadaMesh;
using namespace std;

/*---------------------------------------------------------*/
TriangleMesh::~TriangleMesh() {}
TriangleMesh::TriangleMesh() : FadaMesh::FadaMeshBase2d<3>( )
{
  _hatNodesOfCell();
  _hatNodeIdOfSide();
  _hatNodesOfPatch();
  _patchHatNodeIdOfCell();
}
TriangleMesh::TriangleMesh(const TriangleMesh& trianglemesh)
{
  assert(0);
}
TriangleMesh& TriangleMesh::operator=(const TriangleMesh& trianglemesh)
{
  assert(0);
}

std::string TriangleMesh::getName() const
{
  return "FadaMesh::TriangleMesh";
}

/*---------------------------------------------------------*/
void TriangleMesh::ReadTri(std::string filename)
{
  string name = filename;
  name += ".tri";

  ifstream file( name.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** TriangleMesh::ReadTri() : cannot read file \""<<filename<< "\"";
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
  // std::  << "nodes " << getNodes() << "\n";

  // on n'utilse pas ma fonction Read pour les quads, car on veut lire que les nodeids !
  Alat::Vector<Triangle>& cells = getCells();
  int nc;
  std::string datatype;
  file >> nc >> datatype;
  cells.set_size(nc);
  for(int i = 0; i < nc; i++)
  {
    for(int ii = 0; ii < 3; ii++)
    {
      file >> cells[i][ii];
    }
  }
  // std::cerr << "cells " << cells << "\n";

  TriangleMesh::BoundarySideToColor bstc;
  TriangleMesh::Side S;
  int nb;
  file >> nb >> datatype;
  for(int i = 0; i < nb; i++)
  {
    for(int ii = 0; ii < 2; ii++)
    {
      file >> S[ii];
    }
    int color;
    file >> color;
    sort( S.begin(), S.end() );
    bstc[S] = color;
  }
  constructSidesFromCells(bstc);

  file.close();
}
/*-------------------------------------------------------*/
const TriangleMesh::Side& TriangleMesh::_getSideOfCell(int iK, int ii) const
{
  int count = 0;
  for(int jj = 0; jj < 3; jj++)
  {
    if(jj != ii)
    {
      _S[count++] = getNodeIdOfCell(iK, jj);
    }
  }
  return _S;
}

/*-------------------------------------------------------*/
int TriangleMesh::getNodeIdOfSideOfCell(int iK, int iis, int ii) const
{
  // iK : cellule
  // iis : num local d'un side de iK
  // ii : num local du node dans iK
  int S;
  if(iis == 0)
  {
    if(ii == 0)
    {
      S = getNodeIdOfCell(iK, 1);
    }
    else
    {
      S = getNodeIdOfCell(iK, 2);
    }
  }
  else if(iis == 1)
  {
    if(ii == 0)
    {
      S = getNodeIdOfCell(iK, 2);
    }
    else
    {
      S = getNodeIdOfCell(iK, 0);
    }
  }
  else if(iis == 2)
  {
    if(ii == 0)
    {
      S = getNodeIdOfCell(iK, 0);
    }
    else
    {
      S = getNodeIdOfCell(iK, 1);
    }
  }
  return S;
}

/*-------------------------------------------------------*/
double TriangleMesh::_ComputeArea(const Triangle& K) const
{
  double dx1 = getNode(K[1]).x() - getNode(K[0]).x();
  double dx2 = getNode(K[2]).x() - getNode(K[0]).x();
  double dy1 = getNode(K[1]).y() - getNode(K[0]).y();
  double dy2 = getNode(K[2]).y() - getNode(K[0]).y();
  return 0.5*fabs(dx1*dy2 - dx2*dy1);
}

/*----------------------------------------------------------*/

void TriangleMesh::writeMedit(std::string filename) const
{
  string name = filename;
  name += ".mesh";

  ofstream file( name.c_str() );
  assert( file.is_open() );

  file << "MeshVersionFormatted 1\n";
  file << "Dimension 3\n";

  int nn = getNNodes();
  file << "Vertices " << nn << std::endl;
  for(int i = 0; i < nn; i++)
  {
    int ref = 0;
    const TriangleMesh::Node& v = getNode(i);
    file << v.x() << " " << v.y() << " " << 0. << " "<< ref << std::endl;
  }
  file << std::endl;

  int nc = getNCells();
  file << std::endl << "Triangles " << nc <<" " << std::endl;
  for(int ic = 0; ic < nc; ic++)
  {
    int ref = 0;
    for(int ii = 0; ii < 3; ii++)
    {
      file << getNodeIdOfCell(ic, ii)+1 << " ";
    }
    file << ref << std::endl;
  }

  int ns = getNSides();
  file << std::endl << "Edges " << ns <<" " << std::endl;
  for(int is = 0; is < ns; is++)
  {
    int ref = 0;
    for(int ii = 0; ii < 2; ii++)
    {
      file << getNodeIdOfSide(is, ii)+1 << " ";
    }
    file << ref << std::endl;
  }
  file << std::endl << "End" << std::endl;
}

/*--------------------------------------------------------------*/
/* Permet de retrouver les noeuds de l'element de reference     */
/*--------------------------------------------------------------*/
void TriangleMesh::_hatNodesOfCell()
{
  _vhat.set_size(3);
  _vhat[0].x() = 0.;
  _vhat[0].y() = 0.;
  _vhat[0].z() = 0.;
  _vhat[1].x() = 1.;
  _vhat[1].y() = 0.;
  _vhat[1].z() = 0.;
  _vhat[2].x() = 0.;
  _vhat[2].y() = 1.;
  _vhat[2].z() = 0.;
}

/*--------------------------------------------------------------------*/
/* Permet de retrouver les noeuds des faces l'element de reference    */
/*--------------------------------------------------------------------*/
void TriangleMesh::_hatNodeIdOfSide()
{
  _hatnode_id_of_side.set_size(3);
  for(int ii = 0; ii < 3; ii++)
  {
    _hatnode_id_of_side[ii].set_size(2);
  }
  _hatnode_id_of_side[0][0] = 1;
  _hatnode_id_of_side[0][1] = 2;
  _hatnode_id_of_side[1][0] = 2;
  _hatnode_id_of_side[1][1] = 0;
  _hatnode_id_of_side[2][0] = 0;
  _hatnode_id_of_side[2][1] = 1;
}

/*--------------------------------------------------------------*/
/* Permet de retrouver les noeuds sur le patch pour un raffinement */
/*--------------------------------------------------------------*/
void TriangleMesh::_hatNodesOfPatch()
{
  _patchvhat.set_size(6);
  _patchvhat[0].x() =  0.;
  _patchvhat[0].y() = 0.5;
  _patchvhat[0].z() = 0.;
  _patchvhat[1].x() =  0.5;
  _patchvhat[1].y() = 0.5;
  _patchvhat[1].z() = 0.;
  _patchvhat[2].x() =  0.;
  _patchvhat[2].y() =  1.;
  _patchvhat[2].z() = 0.;
  _patchvhat[3].x() =  0.5;
  _patchvhat[3].y() =  0.;
  _patchvhat[3].z() = 0.;
  _patchvhat[4].x() =  0.;
  _patchvhat[4].y() =  0.;
  _patchvhat[4].z() = 0.;
  _patchvhat[5].x() =  1.;
  _patchvhat[5].y() =  0.;
  _patchvhat[5].z() = 0.;
}

/*--------------------------------------------------------------*/
/* Permet de retrouver les noeuds des cells sur le patch        */
/*--------------------------------------------------------------*/
void TriangleMesh::_patchHatNodeIdOfCell()
{
  _patch_hatnode_id_of_cell.set_size(4);
  for(int ii = 0; ii < 4; ii++)
  {
    _patch_hatnode_id_of_cell[ii].set_size(3);
  }
  _patch_hatnode_id_of_cell[0][0] = 0;
  _patch_hatnode_id_of_cell[0][1] = 1;
  _patch_hatnode_id_of_cell[0][2] = 2;
  _patch_hatnode_id_of_cell[1][0] = 4;
  _patch_hatnode_id_of_cell[1][1] = 3;
  _patch_hatnode_id_of_cell[1][2] = 0;
  _patch_hatnode_id_of_cell[2][0] = 3;
  _patch_hatnode_id_of_cell[2][1] = 5;
  _patch_hatnode_id_of_cell[2][2] = 1;
  _patch_hatnode_id_of_cell[3][0] = 1;
  _patch_hatnode_id_of_cell[3][1] = 0;
  _patch_hatnode_id_of_cell[3][2] = 3;
}
/*--------------------------------------------------------------*/

FadaMeshEnums::meshtype TriangleMesh::getType() const
{
  return FadaMeshEnums::TriangleMesh;
}

/*--------------------------------------------------------------*/

std::string TriangleMesh::getCellType() const
{
  return "Triangle";
}
/*--------------------------------------------------------------------------*/

int TriangleMesh::getCouplingOffset(int iS) const
{
  return -1;
}
void TriangleMesh::getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const
{
  sideindex_a.set_size(3);
  sideindex_e.set_size(3);
  sideindex_a[0] = 0; sideindex_e[0] = 1;
  sideindex_a[1] = 1; sideindex_e[1] = 2;
  sideindex_a[2] = 2; sideindex_e[2] = 0;
}
void TriangleMesh::getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const
{
  getLocalIndicesOfSidesInCell(sideindex_a, sideindex_e);
}
