#ifndef __FadaMesh_FadaMeshBase3d_h
#define __FadaMesh_FadaMeshBase3d_h

#include  "fadalightmeshbase.h"

/*---------------------------------------------------------*/

namespace FadaMesh
{
  template<int NODESPERCELL, int SIDESPERCELL, int EDGESPERCELL, int NODESPERSIDE>
  class FadaMeshBase3d : public FadaMeshBase<3, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>
  {
public:
    typedef CellBase<2> Edge;
    typedef CellBase<EDGESPERCELL> EdgeCell;
    typedef CellBase<NODESPERSIDE> EdgeSide;
    typedef CellBase<SIDESPERCELL> SideCell;
    typedef CellBase<NODESPERSIDE> Side;
    // typedef std::map<Side, int> BoundarySideToColor;
    typedef typename FadaMeshBase<3, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::BoundarySideToColor BoundarySideToColor;

private:
    static Edge _dummyedge;
    Alat::Vector<Edge>   _edges;
    Alat::Vector<EdgeCell>   _edges_of_cells;
    Alat::Vector<EdgeSide>   _edges_of_sides;

protected:
    virtual const Edge& _getEdgeOfCell(int i, int ii) const;
    virtual const Edge& _getEdgeOfSide(int i, int ii) const;

public:
    FadaMeshBase3d<NODESPERCELL, SIDESPERCELL, EDGESPERCELL, NODESPERSIDE>( );
    Alat::Vector<Edge>& getEdges();
    Alat::Vector<EdgeCell>&  getEdgesOfCells();
    Alat::Vector<EdgeSide>&  getEdgesOfSides();
    const Alat::Vector<Edge>&   getEdges() const;
    const Alat::Vector<EdgeCell>&  getEdgesOfCells() const;
    const Alat::Vector<EdgeSide>&  getEdgesOfSides() const;
    const Edge&   getEdge(int i) const;
    const EdgeCell&  getEdgesOfCell(int i) const;
    const EdgeSide&  getEdgesOfSides(int i) const;
    std::string getName() const;
    int getNEdgesPerCell(int i) const;
    int getNEdgesPerSide(int i) const;
    int getNEdges() const;
    int getNodeIdOfEdge(int i, int ii) const;
    int getEdgeIdOfCell(int i, int ii) const;
    int getEdgeIdOfSide(int i, int ii) const;
    Alat::Node getNodeOfEdge(int ie) const;
    void readFadaMesh(const std::string& basefilename);
    void writeFadaMesh(const std::string& basefilename, std::string datatype = "binary") const;
    void constructSidesFromCells(BoundarySideToColor& bsides, int color_default = 0);
    // std::string getEnsightType() const;
    int getVtkType() const;
  };
}

/*---------------------------------------------------------*/

#endif
