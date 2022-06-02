#include  "FadaMesh/hexahedralmesh.h"
#include  "FadaMesh/quadrilateralmesh.h"
// #include  "FadaMesh/trianglemesh.h"
#include  <iostream>

/* ---------------------------------- */

int main(int argc, char** argv)
{
  if(argc != 5)
  {
    std::cerr << "*** ERROR in " << argv[0] <<" usage: filein  fileout meshtype datatype\n";
    return 1;
  }
  std::string infilename(argv[1]);
  std::string outfilename(argv[2]);
  std::string mesh_type(argv[3]);
  std::string datatype(argv[4]);
  FadaMesh::MeshInterface* mesh;
  if(mesh_type == "quad")
  {
    mesh = new FadaMesh::QuadrilateralMesh;
    outfilename += ".quad";
  }
  else if(mesh_type == "hex")
  {
    mesh = new FadaMesh::HexahedralMesh;
    outfilename += ".hex";
  }
  else
  {
    std::cerr<<"***ERROR in " << argv[0] <<"  : invalid mesh type. Must be quad, hex or tri, but "<<mesh_type<<" given."<<'\n';
    assert(0);
    exit(1);
  }
  mesh->readFadaMesh(infilename);
  mesh->write(outfilename);
  // std::cerr << "@@@@@@@@@@@@@@@@@@@@ " << argv[0] <<  ": " << infilename << " ====>>> " << outfilename << "\n";
  // assert(0);
  delete mesh;
}
