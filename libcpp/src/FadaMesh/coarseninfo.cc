#include  "FadaMesh/coarseninfo.h"
#include  <fstream>
#include  <iostream>

using namespace FadaMesh;

/*---------------------------------------------------------*/
CoarsenInfo::~CoarsenInfo(){}
CoarsenInfo::CoarsenInfo(){}
CoarsenInfo::CoarsenInfo(const CoarsenInfo& coarseninfo){}
CoarsenInfo& CoarsenInfo::operator=( const CoarsenInfo& coarseninfo)
{
  assert(0);
}

std::string CoarsenInfo::getName() const
{
  return "CoarsenInfo";
}

/*---------------------------------------------------------*/

Alat::SparsityPattern& CoarsenInfo::getOldNodes()
{
  return _oldnodeids;
}

Alat::SparsityPattern& CoarsenInfo::getOldCells()
{
  return _oldcellids;
}

Alat::SparsityPattern& CoarsenInfo::getOldSides()
{
  return _oldsideids;
}

Alat::SparsityPattern& CoarsenInfo::getOldEdges()
{
  return _oldedgeids;
}

int CoarsenInfo::getNNodes() const
{
  return _oldnodeids.n();
}

int CoarsenInfo::getNOldNodes(int i) const
{
  return _oldnodeids.rowsize(i);
}

int CoarsenInfo::getOldNode(int i, int ii) const
{
  return _oldnodeids.col(_oldnodeids.rowstart(i)+ii);
}

int CoarsenInfo::getNCells() const
{
  return _oldcellids.n();
}

int CoarsenInfo::getNOldCells(int i) const
{
  return _oldcellids.rowsize(i);
}

int CoarsenInfo::getOldCell(int i, int ii) const
{
  return _oldcellids.col(_oldcellids.rowstart(i)+ii);
}

int CoarsenInfo::getNSides() const
{
  return _oldsideids.n();
}

int CoarsenInfo::getNOldSides(int i) const
{
  return _oldsideids.rowsize(i);
}

int CoarsenInfo::getOldSide(int i, int ii) const
{
  return _oldsideids.col(_oldsideids.rowstart(i)+ii);
}

int CoarsenInfo::getNEdges() const
{
  return _oldedgeids.n();
}

int CoarsenInfo::getNOldEdges(int i) const
{
  return _oldedgeids.rowsize(i);
}

int CoarsenInfo::getOldEdge(int i, int ii) const
{
  return _oldedgeids.col(_oldedgeids.rowstart(i)+ii);
}

Alat::IntVector& CoarsenInfo::getNodeNewToOld()
{
  return _nodenewtoold;
}

const Alat::IntVector& CoarsenInfo::getNodeNewToOld() const
{
  return _nodenewtoold;
}

/*---------------------------------------------------------*/

void CoarsenInfo::read(std::string filename)
{
  std::ifstream file( filename.c_str() );
  assert( file.is_open() );
  _oldnodeids.loadFada(file);
  _oldcellids.loadFada(file);
  _oldsideids.loadFada(file);
  // _oldedgeids.read(file);
  _nodenewtoold.loadFada(file);
  file.close();
}

/*---------------------------------------------------------*/

void CoarsenInfo::write(std::string filename, std::string datatype) const
{
  std::ofstream file( filename.c_str() );
  assert( file.is_open() );
  _oldnodeids.saveFada(file, datatype);
  _oldcellids.saveFada(file, datatype);
  _oldsideids.saveFada(file, datatype);
  // _oldedgeids.write(file, datatype);
  _nodenewtoold.saveFada(file, datatype);
  file.close();
}
