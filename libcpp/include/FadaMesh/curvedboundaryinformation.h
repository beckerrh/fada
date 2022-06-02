#ifndef  __FadaMesh_CurvedBoundaryInformation_h
#define  __FadaMesh_CurvedBoundaryInformation_h

#include  "curvedboundarydescriptioninterface.h"
// #include  "curvedboundaryinformationinterface.h"
#include  "geometryobject.h"
#include  "meshinterface.h"
#include  "Alat/sparsitypatternfixarray.h"
#include  "Alat/map.h"
#include  <fstream>

/*---------------------------------------------------*/

namespace FadaMesh
{
  class CurvedBoundaryInformation : public FadaMesh::GeometryObject
  // , public FadaMesh::CurvedBoundaryInformationInterface
  {
public:
    typedef Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>::const_iterator const_iterator;
    typedef Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>::iterator iterator;

private:
    Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*> _curvedboundaries;
    Alat::IntVector _celliscurved;
    Alat::SparsityPatternFixArray<2> _spfa;

protected:
  Alat::SparsityPatternFixArray<2>& curvedCellIndices();

public:
    ~CurvedBoundaryInformation();
    CurvedBoundaryInformation();
    CurvedBoundaryInformation( const CurvedBoundaryInformation& curvedboundaryinformation);
    CurvedBoundaryInformation& operator=( const CurvedBoundaryInformation& curvedboundaryinformation);
    std::string getName() const;

    const Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>& get() const;
    const FadaMesh::CurvedBoundaryDescriptionInterface* get(int color) const;
    FadaMesh::CurvedBoundaryDescriptionInterface* get(int color);
    FadaMesh::CurvedBoundaryDescriptionInterface*& getPointer(int color);
    bool boundaryColorIsCurved(int color) const;
    void readCurvedBoundaryDescription(std::istream& in);
    void writeCurvedBoundaryDescription(std::ostream& out) const;
    void read(std::string filename);
    void write(std::string filename, std::string datatype) const;
    void set_size(const CurvedBoundaryInformation& curvedboundaryinformation);

    const Alat::SparsityPatternFixArray<2>& curvedCellIndices() const;
    bool cellIsCurved(int iK) const;
    int rowstart(int iK) const;
    int rowstop(int iK) const;
    const Alat::FixArray<2, int>& getCurvedInfo(int pos) const;
    void constructBoundaryInformation(const FadaMesh::MeshInterface* M);
  };
}

/*---------------------------------------------------*/

#endif
