#include  "Alat/iomanager.h"
#include  "Alat/tokenize.h"
#include  "Alat/variablevector.h"
#include  "FadaMesh/markercoarsepercentage.h"
#include  "FadaMesh/markermean.h"
#include  "FadaMesh/markeroptimal.h"
#include  "FadaMesh/markeroptimalcoarse.h"
#include  "FadaMesh/markerpercentage.h"
#include  "FadaMesh/markerrandom.h"
#include  <dirent.h>
#include  <stdlib.h>
#include  <fstream>

using namespace std;

/// This program generates a set of marked cells from cell-wise values of an error estimator.
/// For this purpose we suppose that the run-directory contains a file 'Estimator.fadalightsolution'.
/// It produces as output a file 'marked_cells' in the mesh-directory

/*----------------------------------------------------------*/

int getNumberOfCells(std::string inmeshname)
{
  std::string meshname = inmeshname+".fadalightmesh/name";
  ifstream meshnamefile( meshname.c_str() );
  if( not meshnamefile.is_open() )
  {
    std::cerr<<"*** ERROR in getNumberOfCells(): cannot open file \""<<meshname<<"\"\n";
    assert(0);
    exit(1);
  }
  std::string toto;
  meshnamefile>>toto;
  meshnamefile>>toto;
  meshnamefile>>toto;
  vector<string> bouts = Alat::Tokenize(toto, "_");
  assert(bouts.size() > 1);
  meshnamefile.close();
  return atoi( bouts[bouts.size()-2].c_str() );
}

/*----------------------------------------------------------*/

int getDimension(std::string inmeshname)
{
  std::string meshname = inmeshname+".fadalightmesh/name";
  ifstream meshnamefile( meshname.c_str() );
  if( not meshnamefile.is_open() )
  {
    std::cerr<<"*** ERROR in getNumberOfCells(): cannot open file \""<<meshname<<"\"\n";
    assert(0);
    exit(1);
  }
  std::string toto;
  meshnamefile>>toto;
  meshnamefile.close();
  int dimension = 3;
  if( ( toto == "FadaMesh::QuadrilateralMesh" )||( toto == "FadaMesh::TriangleMesh" ) )
  {
    dimension = 2;
  }
  else if(toto == "FadaMesh::HexaedralMesh" )
  {
    dimension = 3;
  }
  else
  {
    std::cerr << "*** ERROR in getDimension(): unknown mesh " << toto << "\n";
    assert(0);
    exit(1);
  }
  return dimension;
}

/*----------------------------------------------------------*/

int main(int argc, char** argv)
{
  if(argc < 6)
  {
    std::cerr << "***ERROR in "<<argv[0]<<" expected parameters : inmeshname estimatorfile datatype marker marking_parameter\n";
    std::cerr << "I got " << argc << "\n";
    std::cerr << "I got " << argv << "\n";
    return 1;
  }
  std::string inmeshname(argv[1]);
  std::string estimatorfile(argv[2]);
  std::string datatype(argv[3]);
  std::string marker(argv[4]);
  double marking_parameter( atof(argv[5]) );
  // std::cerr << "\n\n XXXXXXXXXXXXXXXXXXX estimatorfile " << estimatorfile << "\n";


  FadaMesh::MarkerInterface* MARKER;
  // std::cerr << "\n\n XXXXXXXXXXXXXXXXXXX marker " << marker << "\n";
  if(marker == "percentage")
  {
    MARKER = new FadaMesh::MarkerPercentage(marking_parameter);
  }
  else if(marker == "coarsepercentage")
  {
    MARKER = new FadaMesh::MarkerCoarsePercentage(marking_parameter);
  }
  else if(marker == "mean")
  {
    MARKER = new FadaMesh::MarkerMean(marking_parameter);
  }
  else if(marker == "optimal")
  {
    int dimension = getDimension(inmeshname);
    MARKER = new FadaMesh::MarkerOptimal(marking_parameter, dimension);
  }
  else if(marker == "optimalcoarse")
  {
    int dimension = getDimension(inmeshname);
    MARKER = new FadaMesh::MarkerOptimalCoarse(marking_parameter, dimension);
  }
  else if(marker == "random")
  {
    int n = getNumberOfCells(inmeshname);
    MARKER = new FadaMesh::MarkerRandom(n, marking_parameter);
  }
  else
  {
    std::cerr << "*** ERROR in " <<argv[0] << " unknown marker \"" << marker << "\"\n";
    assert(0);
    exit(1);
  }
  if(marker != "random")
  {

    Alat::DoubleVector& indicator = MARKER->getIndicator();

    std::ifstream fileestimator( estimatorfile.c_str() );

    Alat::VariableVector estimator;
    estimator.loadFada(fileestimator);
    // std::cerr << "@@@@@ estimator " << estimator.n() << " " << estimator.getNComponents() << "\n";

    int n = estimator.n();
    indicator.resize(n);
    int icomp = 0;
    for(int i = 0; i < n; i++)
    {
      // indicator[i] = estimator(0, i);
      indicator[i] = estimator(i, icomp);
    }
    // std::cerr << "@@@@@ indicator " << indicator << "\n";
  }
  std::string outfilename = inmeshname+".fadalightmesh/marked_cells";
  MARKER->write(outfilename, datatype);
  delete MARKER;
}
