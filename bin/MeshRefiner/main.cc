#include  "Alat/tokenize.h"
#include  "FadaMesh/constructadaptivemeshandadaptor.h"
#include  <dirent.h>

using namespace FadaMesh;
using namespace std;

/*----------------------------------------------------------*/

int main(int argc, char** argv)
{
  if( argc != 4 and argc != 5)
  {
    std::cerr << "*** " << argv[0] << " usage : infilename outfilename (FadaMesh) datatype  \n";
    std::cerr << "*** or (global refine) "<< "usage : infilename outfilename (FadaMesh) datatype  nref\n";
    return 1;
  }
  bool globalrefine = false;
  int nref;
  if(argc == 5)
  {
    globalrefine = true;
    nref = atoi(argv[4]);
  }
  std::string infilename(argv[1]);
  std::string outfilename(argv[2]);
  std::string datatype(argv[3]);

  vector<string> bouts = Alat::Tokenize(infilename, "/");
  string lastnamein = *bouts.rbegin();
  bouts = Alat::Tokenize(outfilename, "/");
  string lastnameout = *bouts.rbegin();
  AdaptiveMeshInterface* M = NULL;
  MeshAdaptorInterface* AM = NULL;
  ConstructAdaptiveMeshAndAdaptor(M, AM, infilename);
  assert(M);
  assert(AM);
  bool first = 1;
//
  std::string indir = infilename+".fadalightmesh";
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
  }
  //
  // std::cerr << " globalrefine= " << globalrefine << " ?\n";
  // std::cerr << " nref= " << nref << " ?\n";
  // std::cerr << " adaptname= " << adaptname << " ?\n";
  // std::cerr << "first = " << first << " ?\n";


  closedir(dp);
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
    M->readAdaptiveMesh(infilename, lastnamein);
  }

  std::string markedfilename = infilename+".fadalightmesh/marked_cells";


  if(globalrefine)
  {
    AM->globalRefine(nref);
  }
  else
  {
    AM->adaptMesh(markedfilename);
  }


  M->writeFadaMesh(outfilename, datatype);
  std::string outadaptive = outfilename+".fadalightmesh/"+lastnameout;
  // std::cerr << "outadaptive " << outadaptive << "\n";
  M->writeAdaptiveMesh(outadaptive);
  if(datatype=="ascii")
  {
    M->writeVtk(outfilename);
  }
  delete M;
  delete AM;
  return 0;
}
