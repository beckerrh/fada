#ifndef __FadaMesh_HangingNodeInfo_h
#define __FadaMesh_HangingNodeInfo_h

#include  "geometryobject.h"
#include  "Alat/sparsitypattern.h"

/*---------------------------------------------------------*/
namespace FadaMesh
{
  class HangingNodeInfo : public FadaMesh::GeometryObject, public Alat::SparsityPattern
  {
  public:
    ~HangingNodeInfo();
    HangingNodeInfo();
    HangingNodeInfo(const HangingNodeInfo& hangingnodeinfo);
    HangingNodeInfo& operator=(const HangingNodeInfo& hangingnodeinfo);

    std::string getName() const;
    int getCellNumber(int i) const;
    int getLocalSide(int i) const;
    int getNumberOfHangingNodes(int i) const;
    int getHangingNodes(int i, int in) const;
    int& getCellNumber(int i);
    int& getLocalSide(int i);
    int& getHangingNodes(int i, int in);
    void set_size(int n_hanging, int n_local_data);
    void read(std::string filename );
    void write(std::string filename, std::string datatype) const;
  };
}


#endif
