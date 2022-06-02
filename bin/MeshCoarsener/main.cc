#include  "FadaMesh/constructadaptivemeshandadaptor.h"
#include  "Alat/tokenize.h"
#include  <dirent.h>

using namespace std;
using namespace FadaMesh;

/*----------------------------------------------------------*/

int main(int argc, char** argv)
{   
  if( argc!=4)
  {
    std::cerr << "*** " << argv[0] << " usage : infilename outfilename (FadaMesh) datatype\n";
    return 1;
  }   
  std::string infilename(argv[1]);
  std::string outfilename(argv[2]);
  std::string datatype(argv[3]);

  vector<string> bouts = Alat::Tokenize(infilename,"/");
  string lastnamein = *bouts.rbegin();
  bouts = Alat::Tokenize(outfilename,"/");
  string lastnameout = *bouts.rbegin();

//                  
  std::string indir = infilename+".fadalightmesh";
  std::string adaptname = lastnamein+".fadalightmeshadaptive";
  DIR* dp = opendir(indir.c_str());
  if(dp == NULL)
  {
    std::cerr << "cannot open directory indir = \"" << indir << "\"\n";
    return 1;
  }
  dirent *ep;
  while ((ep = readdir(dp)) != NULL)
  {  
    std::string fname(ep->d_name); 
    if(fname==adaptname) {break;}
    // std::cerr << fname << " = " << adaptname << " ?\n"; 
  }
  closedir(dp);
//
  std::string markedfilename = infilename+".fadalightmesh/marked_cells";
  AdaptiveMeshInterface* M;
  MeshAdaptorInterface* AM;
  ConstructAdaptiveMeshAndAdaptor(M,AM,infilename,"coarse");
  M->readAdaptiveMesh(infilename, lastnamein);
  AM->adaptMesh(markedfilename);
  M->writeFadaMesh(outfilename, datatype);
  std::string outadaptive = outfilename+".fadalightmesh/"+lastnameout;
  M->writeAdaptiveMesh(outadaptive);
  //M->writeVtk(outfilename+".fadalightmesh");
  //M->writeEnsightGeometry(outfilename);

  return 0;
}
