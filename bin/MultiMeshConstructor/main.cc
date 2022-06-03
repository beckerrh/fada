#include  "Alat/tokenize.h"
#include  "FadaMesh/constructadaptivemeshandadaptor.h"
#include  "FadaMesh/multimeshcreator.h"
#include  "FadaMesh/multilevelmesh.h"
#include  <dirent.h>
#include  <sstream>

using namespace FadaMesh;
using namespace std;

/*----------------------------------------------------------*/

int main(int argc, char** argv)
{
  if( argc != 5)
  {
    std::cerr << "*** " << argv[0] << " usage : infilename (FadaMesh) datatype nlevels ncells\n";
    return 1;
  }
  std::string infilename(argv[1]);
  std::string datatype(argv[2]);
  int nlevels = atoi(argv[3]);
  int ncells = atoi(argv[4]);

  vector<string> bouts = Alat::Tokenize(infilename, "/");
  string lastnamein = *bouts.rbegin();

  FadaMesh::AdaptiveMeshInterface* M;
  FadaMesh::MeshAdaptorInterface* AM;
  FadaMesh::ConstructAdaptiveMeshAndAdaptor(M, AM, infilename, "coarse");


  //
  bool first = 1;
  std::string indir = infilename+".fadalightmesh";
  // std::cerr<<"LastNameIn "<<lastnamein<<'\n';
  std::string adaptname = lastnamein+".fadalightmeshadaptive";
  DIR* dp = opendir( indir.c_str() );
  if(dp == NULL)
  {
    std::cerr << "cannot open directory indir = \"" << indir << "\"\n";
    return 1;
  }
  dirent* ep;
  while( ( ep = readdir(dp) ) != NULL )
  {
    std::string fname(ep->d_name);
    if(fname == adaptname)
    {
      first = 0;
      break;
    }
    // std::cerr << fname << " = " << adaptname << " ?\n";
  }
  closedir(dp);
  //
  std::string inadaptive = infilename;
  // std::cerr << "inadaptive = " << inadaptive << "\n";
  if(first)
  {
    // s'assurer qu'il n'y a pas de hanging sides !!
    std::string filename = indir + "/HangingSideInfo";
    std::ifstream file( filename.c_str() );
    if( not file.is_open() )
    {
      std::cerr<<"*** Warning "<<argv[0]<<" cannot open file \""<<filename<<"\"\n";
    }
    else
    {
      int hnsides;
      file >> hnsides;
      if(hnsides)
      {
        std::cerr<<"*** ERROR in "<<argv[0]<<" no adaptivemesh found but hnsides = "<<hnsides<<"\n";
        exit(1);
      }
    }
    M->readFadaMeshAndInitTrees(infilename);
  }
  else
  {
    M->readAdaptiveMesh(inadaptive, lastnamein);
  }
  {
    FadaMesh::Coarsener2d* coarsener = dynamic_cast<FadaMesh::Coarsener2d*>( AM );
    FadaMesh::MultiMeshCreator multimeshcreator(coarsener);
    multimeshcreator.createMultiMesh(infilename, datatype, nlevels, ncells);
  }

 if(datatype == "ascii")
 {
   FadaMesh::MultiLevelMesh mmesh("FadaMesh::QuadrilateralMesh");
   mmesh.readFadaMesh(infilename);
   std::string name("MultiMeshConstructor_");
   for(int level = 0; level < mmesh.getNLevels(); level++)
   {
     std::stringstream ss;
     ss << level;
     std::string filename = name + ss.str();
     mmesh.getMesh(level)->writeVtk(filename);
   }
 }
  return 0;
}
