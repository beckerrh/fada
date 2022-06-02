#include  "Fada/femtype.h"
#include  "FadaMesh/meshinterface.h"
#include  <sstream>
#include  <stdlib.h>

namespace Fada
{
  bool femType(std::string& femout, const std::string& femin, const FadaMesh::MeshInterface* mesh)
  {
    if(femin.size() < 3)
    {
      std::cerr << "femin="<<femin<<"\n";
      assert(0);
    }
    bool dg = false;
    if(femin[0] == 'd' and femin[1] == 'g')
    {
      dg = true;
    }
    int order = atoi(&femin[2]);

    std::stringstream ss;
    ss <<  femin[0] << femin[1];
    if(mesh->getType() == FadaMeshEnums::TriangleMesh)
    {
      ss << "p";
    }
    else
    {
      ss << "q";
    }
    ss << order;
    if(mesh->getDimension() == 2)
    {
      ss << "2d";
    }
    else
    {
      ss << "3d";
    }
    femout = ss.str();
    return dg;
  }
}
