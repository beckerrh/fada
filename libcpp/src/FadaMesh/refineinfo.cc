#include  "FadaMesh/refineinfo.h"
#include  <fstream>
#include  <iostream>

using namespace FadaMesh;

/*---------------------------------------------------------*/

RefineInfo::~RefineInfo(){}
RefineInfo::RefineInfo(){}
RefineInfo::RefineInfo(const RefineInfo& refineinfo){}
RefineInfo& RefineInfo::operator=( const RefineInfo& refineinfo)
{
  assert(0);
}

std::string RefineInfo::getName() const
{
  return "RefineInfo";
}

/*---------------------------------------------------------*/

Alat::SparsityPattern& RefineInfo::getCoarseNodes()
{
  return _coarsenodeids;
}

Alat::SparsityPattern& RefineInfo::getCoarseCells()
{
  return _coarsecellids;
}

Alat::SparsityPattern& RefineInfo::getCoarseSides()
{
  return _coarsesideids;
}

Alat::SparsityPattern& RefineInfo::getCoarseEdges()
{
  return _coarseedgeids;
}

const Alat::SparsityPattern& RefineInfo::getCoarseNodes() const
{
  return _coarsenodeids;
}

const Alat::SparsityPattern& RefineInfo::getCoarseCells() const
{
  return _coarsecellids;
}

const Alat::SparsityPattern& RefineInfo::getCoarseSides() const
{
  return _coarsesideids;
}

const Alat::SparsityPattern& RefineInfo::getCoarseEdges() const
{
  return _coarseedgeids;
}

int RefineInfo::getNCoarseCells(int i) const
{
  return _coarsecellids.rowsize(i);
}

int RefineInfo::getNCoarseSides(int i) const
{
  return _coarsesideids.rowsize(i);
}

int RefineInfo::getNCoarseNodes(int i) const
{
  return _coarsenodeids.rowsize(i);
}

int RefineInfo::getNCoarseEdges(int i) const
{
  return _coarseedgeids.rowsize(i);
}

int RefineInfo::getCoarseCellNumber(int i, int ii) const
{
  return _coarsecellids.get(i, ii);
}

int RefineInfo::getCoarseSideNumber(int i, int ii) const
{
  return _coarsesideids.get(i, ii);
}

int RefineInfo::getCoarseNodesNumber(int i, int ii) const
{
  return _coarsenodeids.get(i, ii);
}

int RefineInfo::getCoarseEdgesNumber(int i, int ii) const
{
  return _coarseedgeids.get(i, ii);
}

Alat::IntVector& RefineInfo::getNodeIds()
{
  return _nodeids;
}

const Alat::IntVector& RefineInfo::getNodeIds() const
{
  return _nodeids;
}

Alat::IntVector& RefineInfo::getSideIds()
{
  return _sideids;
}

const Alat::IntVector& RefineInfo::getSideIds() const
{
  return _sideids;
}

Alat::IntVector& RefineInfo::getCellIds()
{
  return _cellids;
}

const Alat::IntVector& RefineInfo::getCellIds() const
{
  return _cellids;
}

/*---------------------------------------------------------*/

void RefineInfo::read(std::string filename)
{
  std::ifstream file( filename.c_str() );
  assert( file.is_open() );
  _coarsenodeids.loadFada(file);
  _coarsecellids.loadFada(file);
  _coarsesideids.loadFada(file);
  // _coarseedgeids.read(file);
  _nodeids.loadFada(file);
  _sideids.loadFada(file);
  _cellids.loadFada(file);
  refinfoinfonode.read(file);
  refinfoinfoside.read(file);
  refinfoinfocell.read(file);
  // std::cerr << "RefineInfo::read() " << filename << " n = " << _coarsenodeids.n() << "\n";
  file.close();
}

/*---------------------------------------------------------*/

void RefineInfo::write(std::string filename, std::string datatype) const
{
  // std::cerr << "RefineInfo::write() " << filename << " n = " << _coarsenodeids.n() << "\n";
  std::ofstream file( filename.c_str() );
  assert( file.is_open() );
  _coarsenodeids.saveFada(file, datatype);
  _coarsecellids.saveFada(file, datatype);
  _coarsesideids.saveFada(file, datatype);
  // _coarseedgeids.write(file, datatype);
  _nodeids.saveFada(file, datatype);
  _sideids.saveFada(file, datatype);
  _cellids.saveFada(file, datatype);
  refinfoinfonode.write(file, datatype);
  refinfoinfoside.write(file, datatype);
  refinfoinfocell.write(file, datatype);
  file.close();
}
