#include  "FadaMesh/hexahedralmesh.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "FadaMesh/quadtotri.h"
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
  FadaMesh::MeshInterface* M;
  if(mesh_type == "quad")
  {
    M = new FadaMesh::QuadrilateralMesh;
    FadaMesh::QuadrilateralMesh* Mquad = dynamic_cast<FadaMesh::QuadrilateralMesh*>( M );
    infilename += ".quad";
    Mquad->readQuad(infilename);
  }
  else if(mesh_type == "hex")
  {
    M = new FadaMesh::HexahedralMesh;
    FadaMesh::HexahedralMesh* Mhex = dynamic_cast<FadaMesh::HexahedralMesh*>( M );
    infilename += ".hex";
    Mhex->readHex(infilename);
  }
  else if(mesh_type == "quadtotri")
  {
    M = new FadaMesh::QuadToTri(infilename);
  }
  // else if(mesh_type == "tri")
  // {
  //   M = new FadaMesh::TriangleMesh;
  //   FadaMesh::TriangleMesh* Mtri = dynamic_cast<FadaMesh::TriangleMesh*>( M );
  //   Mtri->ReadTri(infilename);
  // }
  else
  {
    std::cerr<<"***ERROR in " << argv[0] <<"  : invalid mesh type. Must be quad, hex or tri, but "<<mesh_type<<" given."<<'\n';
    assert(0);
    exit(1);
  }
  M->writeFadaMesh(outfilename, datatype);
  if(datatype == "ascii")
  {
    M->writeVtk(outfilename);
  }
  M->writeBoundaryVtk(outfilename);

  delete M;

  return 0;
}
