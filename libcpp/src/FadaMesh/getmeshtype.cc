#include  "FadaMesh/getmeshtype.h"
#include  <cassert>
#include  <fstream>
#include  <iostream>

/*--------------------------------------------------------------------------*/

Alat::StringPair FadaMesh::getMeshType(const std::string& meshname)
{
  std::string filename = meshname + ".fadalightmesh/name";

  std::ifstream file( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** ERROR  getMeshType() : cannot open file " << filename << "\n";
    assert(0);
  }
  std::string type;
  file >> type;
  std::string datatype;
  file >> datatype;
  return Alat::StringPair(type, datatype);
}
