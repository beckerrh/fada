#ifndef __FadaMesh_CoarsenInfo_h
#define __FadaMesh_CoarsenInfo_h

#include  "geometryobject.h"
#include  "Alat/sparsitypattern.h"

/*---------------------------------------------------------*/

namespace FadaMesh
{
  /// c'est que pour les quads en ce moments !
  /// cellid -->
  class CoarsenInfo : public FadaMesh::GeometryObject
  {
protected:
    Alat::SparsityPattern _oldnodeids, _oldcellids, _oldsideids, _oldedgeids;
    Alat::IntVector _nodenewtoold;

public:
  ~CoarsenInfo();
    CoarsenInfo();
    CoarsenInfo(const CoarsenInfo& coarseninfo);
    CoarsenInfo& operator=( const CoarsenInfo& coarseninfo);
    std::string getName() const;

    Alat::SparsityPattern& getOldNodes();
    Alat::SparsityPattern& getOldCells();
    Alat::SparsityPattern& getOldSides();
    Alat::SparsityPattern& getOldEdges();
    int getNNodes() const;
    int getNOldNodes(int i) const;
    int getOldNode(int i, int ii) const;
    int getNCells() const;
    int getNOldCells(int i) const;
    int getOldCell(int i, int ii) const;
    int getNSides() const;
    int getNOldSides(int i) const;
    int getOldSide(int i, int ii) const;
    int getNEdges() const;
    int getNOldEdges(int i) const;
    int getOldEdge(int i, int ii) const;
    Alat::IntVector& getNodeNewToOld();
    const Alat::IntVector& getNodeNewToOld() const;
    void read(std::string filename);
    void write(std::string filename, std::string datatype = "binary") const;
  };
}
#endif
