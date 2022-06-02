#include  "FadaMesh/hangingnodeinfo.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/sparsitypattern.h"
#include  <fstream>
#include  <list>
#include  <map>
#include  <set>

using namespace FadaMesh;

/*---------------------------------------------------------*/
HangingNodeInfo::~HangingNodeInfo() {}
HangingNodeInfo::HangingNodeInfo() : FadaMesh::GeometryObject(),  Alat::SparsityPattern() {}
HangingNodeInfo::HangingNodeInfo(const HangingNodeInfo& hangingnodeinfo) : FadaMesh::GeometryObject(hangingnodeinfo), Alat::SparsityPattern(hangingnodeinfo) {}
HangingNodeInfo& HangingNodeInfo::operator=(const HangingNodeInfo& hangingnodeinfo)
{
  assert(0);
}

std::string HangingNodeInfo::getName() const
{
  return "HangingNodeInfo";
}

int HangingNodeInfo::getCellNumber(int i) const
{
  return col( rowstart(i) );
}

int HangingNodeInfo::getLocalSide(int i) const
{
  return col(rowstart(i)+1);
}

int HangingNodeInfo::getNumberOfHangingNodes(int i) const
{
  return rowstart(i+1)-rowstart(i)-2;
}

int HangingNodeInfo::getHangingNodes(int i, int in) const
{
  return col(rowstart(i)+in+2);
}

int& HangingNodeInfo::getCellNumber(int i)
{
  return col( rowstart(i) );
}

int& HangingNodeInfo::getLocalSide(int i)
{
  return col(rowstart(i)+1);
}

int& HangingNodeInfo::getHangingNodes(int i, int in)
{
  return col(rowstart(i)+in+2);
}

void HangingNodeInfo::set_size(int n_hanging, int n_local_data)
{
  int n_data = n_local_data*n_hanging;
  col().set_size(n_data);
  rowstart().set_size(n_hanging+1);
  for(int i = 0; i <= n_hanging; i++)
  {
    // rowstart(i) = i*n_local_data;
    _rowstart[i] = i*n_local_data;
  }
}

void HangingNodeInfo::read(std::string filename )
{
  std::ifstream file( filename.c_str() );
  assert( file.is_open() );
  col().loadFada(file);
  rowstart().loadFada(file);
  file.close();
}

void HangingNodeInfo::write(std::string filename, std::string datatype) const
{
  std::ofstream file( filename.c_str() );
  assert( file.is_open() );
  col().saveFada(file, datatype);
  rowstart().saveFada(file, datatype);
  file.close();
}
