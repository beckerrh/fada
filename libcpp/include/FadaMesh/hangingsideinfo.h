#ifndef __FadaMesh_HangingSideInfo_h
#define __FadaMesh_HangingSideInfo_h

#include  "geometryobject.h"
#include  "Alat/sparsitypattern.h"

/*---------------------------------------------------------*/
namespace FadaMesh
{
  class HangingSideInfo : public FadaMesh::GeometryObject, public Alat::SparsityPattern
  {
  public:
    ~HangingSideInfo();
    HangingSideInfo();
    HangingSideInfo(const HangingSideInfo& hangingsideinfo);
    HangingSideInfo& operator=(const HangingSideInfo& hangingsideinfo);

    std::string getName() const;
    int getCellNumber(int i) const;
    int getLocalSide(int i) const;
    int getNumberOfHangingSides(int i) const;
    int getHangingSides(int i, int in) const;
    int& getCellNumber(int i);
    int& getLocalSide(int i);
    int& getHangingSides(int i, int in);
    void set_size(int n_hanging, int n_local_data);
    std::vector<int> getNeighbourCellsOfSide(int iK, int iis, const FadaMesh::MeshInterface* mesh);
    void read(std::string filename );
    void write(std::string filename, std::string datatype) const;
  };
}


#endif
