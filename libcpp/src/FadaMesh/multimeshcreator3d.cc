#include  "FadaMesh/multimeshcreator3d.h"
#include  "FadaMesh/volumeinterface.h"
#include  "FadaMesh/refineinfo.h"
#include  <sstream>

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

void MultiMeshCreator3d::createMultiMesh(std::string dirname, std::string datatype, int nlevels, int ncells)
{
  string multimeshname = dirname+".fadalightmesh/MultiMesh";
  string cmd = "rm -rf "+multimeshname;
  int error = system( cmd.c_str() );
  assert(!error);
  cmd = "mkdir "+multimeshname;
  error = system( cmd.c_str() );
  assert(!error);

  int count = 1;
  bool coarsen = 1;
  _getMesh()->createGeometryObject("RefineInfo");
  FadaMesh::RefineInfo* _refineinfo= dynamic_cast<FadaMesh::RefineInfo*>(_getMesh()->getGeometryObject("RefineInfo"));
  if(_getVolumes().max_depth() == 0)
  {
    coarsen = 0;
  }
  while(coarsen)
  {
    _coarsener->globalCoarsen();
    _constructRefineInfo();
    std::stringstream ss0;
    ss0<<count;
    std::string outfilename = multimeshname+"/mesh."+ss0.str();
    coarsen = ( _getVolumes().max_depth() > 0 );
    _getMesh()->writeFadaMesh(outfilename, datatype);
    if(count == 1)
    {
      outfilename = dirname+".fadalightmesh/RefineInfo";
      _refineinfo->write(outfilename, datatype);
    }
    else
    {
      std::stringstream ss1;
      ss1<<count-1;
      outfilename = multimeshname+"/mesh."+ss1.str()+".fadalightmesh/RefineInfo";
      _refineinfo->write(outfilename, datatype);
    }
    count++;
  }

  string filename = multimeshname+"/n";
  ofstream file( filename.c_str() );
  assert( file.is_open() );
  file<<count<<"\n";
  file.close();
}

/*--------------------------------------------------------------------------*/

void MultiMeshCreator3d::_constructRefineInfo()
{
  int n = _coarsener->getVolumeCoarse().size();
  Alat::Vector<Alat::IntVector> SPC(n), SPF(n), SPE(n), SPN(n) ;
  for(volume_leafpointer v = _getVolumes().begin_leaf(); v != _getVolumes().end_leaf(); v++)
  {
    int i =  _coarsener->getVolumeId2Id()[( *v )->id()];
    
    SPC[i].set_size(_coarsener->getVolumeCoarse()[v].size());
    SPC[i] = _coarsener->getVolumeCoarse()[v];

    SPF[i].set_size(_coarsener->getFaceCoarse()[v].size());
    SPF[i] = _coarsener->getFaceCoarse()[v];

    SPE[i].set_size(_coarsener->getEdgeCoarse()[v].size());
    SPE[i] = _coarsener->getEdgeCoarse()[v];

    SPN[i].set_size(_coarsener->getNodeCoarse()[v].size());
    SPN[i] = _coarsener->getNodeCoarse()[v];


  }
  FadaMesh::RefineInfo* _refineinfo= dynamic_cast<FadaMesh::RefineInfo*>(_getMesh()->getGeometryObject("RefineInfo"));

  Alat::SparsityPattern& coarsenodeids = _refineinfo->getCoarseNodes();
  Alat::SparsityPattern& coarseedgeids = _refineinfo->getCoarseEdges();
  Alat::SparsityPattern& coarsesideids = _refineinfo->getCoarseSides();
  Alat::SparsityPattern& coarsecellids = _refineinfo->getCoarseCells();
  coarsenodeids.set_size(SPN);
  coarseedgeids.set_size(SPE);
  coarsesideids.set_size(SPF);
  coarsecellids.set_size(SPC);
}


