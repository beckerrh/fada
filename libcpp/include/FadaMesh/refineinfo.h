#ifndef __FadaMesh_RefineInfo_h
#define __FadaMesh_RefineInfo_h

#include  "geometryobject.h"
#include  "Alat/sparsitypattern.h"
#include  "Alat/fixarray.h"

/*---------------------------------------------------------*/

namespace FadaMesh
{
  /// ce n'est que pour les quads en ce moment !
  class RefineInfo : public FadaMesh::GeometryObject
  {
protected:
    Alat::SparsityPattern _coarsenodeids, _coarsecellids, _coarsesideids, _coarseedgeids;
    Alat::IntVector _nodeids, _sideids, _cellids;

public:
    ~RefineInfo();
    RefineInfo();
    RefineInfo(const RefineInfo& refineinfo);
    RefineInfo& operator=( const RefineInfo& refineinfo);
    std::string getName() const;

    Alat::FixArray<4,int>  refinfoinfonode, refinfoinfoside, refinfoinfocell;

    Alat::SparsityPattern& getCoarseNodes();
    Alat::SparsityPattern& getCoarseCells();
    Alat::SparsityPattern& getCoarseSides();
    Alat::SparsityPattern& getCoarseEdges();
    Alat::IntVector& getNodeIds();
    Alat::IntVector& getSideIds();
    Alat::IntVector& getCellIds();
    const Alat::SparsityPattern& getCoarseNodes() const;
    const Alat::SparsityPattern& getCoarseCells() const;
    const Alat::SparsityPattern& getCoarseSides() const;
    const Alat::SparsityPattern& getCoarseEdges() const;
    const Alat::IntVector& getNodeIds() const;
    const Alat::IntVector& getSideIds() const;
    const Alat::IntVector& getCellIds() const;
    int getNCoarseCells(int i) const;
    int getNCoarseSides(int i) const;
    int getNCoarseNodes(int i) const;
    int getNCoarseEdges(int i) const;
    int getCoarseCellNumber(int i, int ii) const;
    int getCoarseSideNumber(int i, int ii) const;
    int getCoarseNodesNumber(int i, int ii) const;
    int getCoarseEdgesNumber(int i, int ii) const;
    void read(std::string filename);
    void write(std::string filename, std::string datatype = "binary") const;
  };
}
#endif
