#ifndef __FadaMesh_QuadToTri_h
#define __FadaMesh_QuadToTri_h

#include  "quadrilateralmesh.h"
#include  "trianglemesh.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class QuadrilateralMesh;
  class PatchInfo;
  
  class TriOfQuad
  {
protected:
    Alat::Vector<Alat::Vector<Alat::FixArray<2, int> > > _data;

public:
    void write(std::ostream& out, std::string datatype = "ascii") const;
    void read(std::istream& in);
    int getNTrianglesOfQuad(int iquad) const;
    int getTriIdOfQuad(int iquad, int iit) const;
    int getLocalIdOfQuadSide(int iquad, int iit) const;
    Alat::Vector<Alat::Vector<Alat::FixArray<2, int> > >& getData();
  };

  /*--------------------------------------------------------------------------*/

  class QuadToTri : public TriangleMesh
  {
protected:
    TriOfQuad _tri_of_quad;
    Alat::IntVector _quad_of_tri;
    std::string _infilenamequad;
    QuadrilateralMesh _quadmesh;
    void _makeCrissCross(const FadaMesh::MeshInterface& QM);

public:
    ~QuadToTri();
    QuadToTri();
    QuadToTri(std::string quadfilename);
    QuadToTri& operator=(const QuadToTri& quadtotri);
    std::string getName() const;
    std::string getFileNameQuad() const;
    void convertMesh(const FadaMesh::MeshInterface& QM, std::string type="crisscross");
    void writeFadaMesh(const std::string& basefilename, std::string type) const;
    void readFadaMesh(const std::string& basefilename);
    const TriOfQuad& getTriOfQuad() const;
    TriOfQuad& getTriOfQuad();
    Alat::Vector<Alat::Vector<Alat::FixArray<2, int> > >& getTriOfQuadData();
    const Alat::IntVector& getQuadOfTri() const;
    const QuadrilateralMesh& getQuadrilateralMesh() const;
    int getCenterNodeIdOfQuad(int iK) const;
    void constructPatchInfo(PatchInfo& patchinfo) const;    
  };
}

/*--------------------------------------------------------------------------*/

#endif
