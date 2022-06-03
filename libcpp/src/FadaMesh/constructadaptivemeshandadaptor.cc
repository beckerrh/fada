#include  "FadaMesh/basicadaptivemesh2d.h"
#include  "FadaMesh/coarsener2d.h"
#include  "FadaMesh/constructadaptivemeshandadaptor.h"
#include  "FadaMesh/refiner2d.h"
#include  "FadaMesh/getmeshtype.h"

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

ConstructAdaptiveMeshAndAdaptor::ConstructAdaptiveMeshAndAdaptor(AdaptiveMeshInterface*& M, MeshAdaptorInterface*& AM, const std::string& meshname, const std::string& adaption_type)
{
  pair<string,string> p = FadaMesh::getMeshType(meshname);
  string type = p.first;
  string datatype = p.second;

  if(type == "FadaMesh::TriangleMesh")
  {
    M = new BasicAdaptiveMesh2d<3>;
  }
  else if(type == "FadaMesh::QuadrilateralMesh")
  {
    M = new BasicAdaptiveMesh2d<4>;
  }
  else
  {
    std::cerr<<"***Error: ConstructAdaptiveMeshAndAdaptor: bad  type: "<<type<<'\n';
    assert(0);
    exit(1);
  }
  if ((type == "FadaMesh::QuadrilateralMesh") or (type == "FadaMesh::TriangleMesh"))
  {
     if(adaption_type == "coarse")
     {
       AM = new  Coarsener2d(M);
     }
     else if(adaption_type == "refine")
     {
       AM = new  Refiner2d(M);
     }
     else
     {
       std::cerr<<"***Error: ConstructAdaptiveMeshAndAdaptor: bad adaption type: "<<adaption_type<<'\n';
       assert(0);
       exit(1);
     }
  }
}
