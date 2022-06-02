#ifndef __FadaMesh_QuadrilateralMesh_h
#define __FadaMesh_QuadrilateralMesh_h

#include  "fadalightmeshbase2d.h"


//        2
//     3 -- 2
//   3 |    |  1
//     0 -- 1
//        0

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class QuadrilateralMesh : public FadaMesh::FadaMeshBase2d<4>
  {
public:
    typedef FadaMesh::FadaMeshBase2d<4>::Cell Quadrilateral;
    typedef FadaMesh::FadaMeshBase2d<4>::Side Edge;
    typedef Alat::Node Node;

private:
    mutable Side _S;

protected:
    const Side& _getSideOfCell(int iK, int iis) const;

public:
    ~QuadrilateralMesh();
    QuadrilateralMesh();
    QuadrilateralMesh(const QuadrilateralMesh& quadrilateralmesh);
    std::string getName() const;
    std::string getCellType() const;

    int getNodeIdOfSideOfCell(int iK, int iis, int ii) const;
    void read(std::string filename);
    void write(std::string filename) const;

    void readQuad(std::string filename);
    void writeQuad(std::string filename) const;
    void check() const;
    int getCouplingOffset(int iS) const;
    FadaMeshEnums::meshtype getType() const;
    void getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
    void getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
