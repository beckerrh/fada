#ifndef  __FadaMesh_TriangleMesh_h
#define  __FadaMesh_TriangleMesh_h

#include  "fadameshbase2d.h"

/*-----------------------------------------*/

namespace FadaMesh
{
  class TriangleMesh : public virtual FadaMesh::FadaMeshBase2d<3>
  {
public:
    typedef FadaMesh::FadaMeshBase2d<3>::Cell Triangle;
    typedef FadaMesh::FadaMeshBase2d<3>::Side Edge;
    typedef Alat::Node Node;

private:
    mutable Side _S;
    double _ComputeArea(const Triangle& K) const;
    
    //Noeuds de l'element de reference
    Alat::Vector<Alat::Node> _vhat;
    //Numero des noeuds de chaque face de l'element de reference
    Alat::Vector< Alat::Vector<int> > _hatnode_id_of_side;
    // Alat::Vector< Alat::Vector<int> > _hatnode_id_of_edge;
    //Noeuds du patch pour raffinement
    Alat::Vector<Alat::Node> _patchvhat;
    //Numero des noeuds de chaque face de l'element de volume de reference
    Alat::Vector< Alat::Vector<int> > _patch_hatnode_id_of_cell;
    // int _nsidespatch, _nedgespatch;

protected:
    const Side& _getSideOfCell(int iK, int ii) const;
    void _hatNodesOfCell();
    void _hatNodeIdOfSide();
    void _hatNodesOfPatch();
    void _patchHatNodeIdOfCell();
    
public:
    ~TriangleMesh();
    TriangleMesh();
    TriangleMesh(const TriangleMesh& trianglemesh);
    TriangleMesh& operator=(const TriangleMesh& trianglemesh);
    std::string getName() const;

    int getNodeIdOfSideOfCell(int iK, int iis, int ii) const;

    int getVtkType() const
    {
      return 5;
    }

    void writeMedit(std::string filename) const;
    void ReadTri(std::string filename);
    FadaMeshEnums::meshtype getType() const;
    std::string getCellType() const;
    int getCouplingOffset(int iS) const;
    void getLocalIndicesOfSidesInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
    void getLocalIndicesOfSidesAndDiagonalsInCell(Alat::IntVector& sideindex_a, Alat::IntVector& sideindex_e) const;
  };
}

#endif
