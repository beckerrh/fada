#include  "FadaMesh/refineinfo.h"
#include  "FadaMesh/quadtotri.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "FadaMesh/getmeshtype.h"
#include  <sstream>

/*----------------------------------------------------------*/

int main(int argc, char** argv)
{

  if( argc != 3)
  {
    std::cerr << "*** " << argv[0] << " usage : trimesh (FadaMesh) datatype\n";
    return 1;
  }
  std::string trimesh(argv[1]);
  std::string datatype(argv[2]);

  FadaMesh::QuadToTri quadtotri;
  quadtotri.readFadaMesh(trimesh);
  FadaMesh::MultiLevelMesh mlmeshquad("FadaMesh::QuadrilateralMesh");
  mlmeshquad.readFadaMesh(quadtotri.getFileNameQuad());
  int nlevels = mlmeshquad.getNLevels();
  
  std::string multimeshname = trimesh+".fadalightmesh/MultiMesh";
  std::string cmd = "rm -rf "+multimeshname;
  int error = system( cmd.c_str() );
  assert(!error);
  cmd = "mkdir "+multimeshname;
  error = system( cmd.c_str() );
  assert(!error);
  std::string filename = multimeshname+"/n";
  std::ofstream file( filename.c_str() );
  assert( file.is_open() );
  file<<nlevels<<"\n";
  file.close();
  
  
  for(int level=1;level<nlevels;level++)
  {
    std::string quadmeshname = mlmeshquad.getMesh(level)->getInfileName();
    // std::cerr << "quadmeshname = "<< quadmeshname << "\n";
    FadaMesh::QuadToTri trimesh =  FadaMesh::QuadToTri(quadmeshname);
    Alat::StringPair p = FadaMesh::getMeshType(quadmeshname);
    
    // std::cerr << "??????????????????????????????? reste a construire le refinfo !!!!!!!!!!\n";
    std::stringstream ss0;
    ss0<<level;
    std::string outfilename = multimeshname+"/mesh."+ss0.str();
    trimesh.writeFadaMesh(outfilename, p.second );
    if(datatype == "ascii")
    {
      trimesh.writeVtk(outfilename);
    }
  }
  return 0;
}
