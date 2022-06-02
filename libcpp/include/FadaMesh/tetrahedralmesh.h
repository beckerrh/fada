#ifndef  __FadaMesh_TetrahedralMesh_h
#define  __FadaMesh_TetrahedralMesh_h

#include  "fadalightmeshbase3d.h"

/*-----------------------------------------*/

namespace FadaMesh
{
  /*!
     *This class is a wrapper of Alat::DoubleVectorTetrahedralMesh introduced to
     *satisfy the conditions of FadaMesh::MeshInterface.
   */
  class TetrahedralMesh : public virtual FadaMesh::FadaMeshBase3d<4, 4, 6, 3>
  {
public:
    typedef FadaMesh::FadaMeshBase3d<4, 4, 6, 3>::Cell Tetrahedron;
    typedef FadaMesh::FadaMeshBase3d<4, 4, 6, 3>::Side Face;
    typedef Alat::Node Node;

private:
    mutable Node _v;
    mutable Edge _E;
    mutable Side _S;
    double _ComputeVolume(const Tetrahedron& K) const;

protected:
    const Alat::Node& _getNodeOfCell(int iK) const
// ça renvoie les coordonnees du centre de gravite de la cellule iK
    {
      for(int ii = 0; ii < 4; ii++)
      {
        const Alat::Node& vii = FadaMesh::MeshInterface::getNodeOfCell(iK, ii);
        _v.x() += vii.x()/4.0;
        _v.y() += vii.y()/4.0;
        _v.z() += vii.z()/4.0;
      }
      return _v;
    }

/*--------------------------------------------------------------------------*/
    const Alat::Node& _getNodeOfSide(int is) const
// ça renvoie les coordonnees du centre de gravite de la face is
    {
      const Alat::Node& v1 = FadaMesh::FadaMeshBase3d<4, 4, 6, 3>::getNodeOfSide(is, 0);
      const Alat::Node& v2 = FadaMesh::FadaMeshBase3d<4, 4, 6, 3>::getNodeOfSide(is, 1);
      const Alat::Node& v3 = FadaMesh::FadaMeshBase3d<4, 4, 6, 3>::getNodeOfSide(is, 2);
      _v.x() = ( v1.x()+v2.x()+v3.x() )/3.0;
      _v.y() = ( v1.y()+v2.y()+v3.y() )/3.0;
      _v.z() = ( v1.z()+v2.z()+v3.z() )/3.0;
      return _v;
    }

/*--------------------------------------------------------------------------*/
    const Edge& _getEdgeOfCell(int i, int ii) const
// ça renvoie les 2 noeuds de l'arete ii de la cellule i
    {
      if(ii == 0)
      {
        _E[0] = getNodeIdOfCell(i, 0);
        _E[1] = getNodeIdOfCell(i, 1);
      }
      else if(ii == 1)
      {
        _E[0] = getNodeIdOfCell(i, 0);
        _E[1] = getNodeIdOfCell(i, 2);
      }
      else if(ii == 2)
      {
        _E[0] = getNodeIdOfCell(i, 0);
        _E[1] = getNodeIdOfCell(i, 3);
      }
      else if(ii == 3)
      {
        _E[0] = getNodeIdOfCell(i, 1);
        _E[1] = getNodeIdOfCell(i, 2);
      }
      else if(ii == 4)
      {
        _E[0] = getNodeIdOfCell(i, 1);
        _E[1] = getNodeIdOfCell(i, 3);
      }
      else if(ii == 5)
      {
        _E[0] = getNodeIdOfCell(i, 2);
        _E[1] = getNodeIdOfCell(i, 3);
      }
      return _E;
    }

/*--------------------------------------------------------------------------*/
    const Edge& _getEdgeOfSide(int i, int ii) const
// ça renvoie les 2 noeuds de l'arete ii de la face i
    {
      int count = 0;
      for(int jj = 0; jj < 3; jj++)
      {
        if(jj != ii)
        {
          _E[count++] = getNodeIdOfSide(i, jj);
        }
      }
      return _E;
    }

/*--------------------------------------------------------------------------*/
    const Side& _getSideOfCell(int i, int ii) const
// ça renvoie les 3 noeuds de la face ii de la cellule i
    {
      int count = 0;
      for(int jj = 0; jj < 4; jj++)
      {
        if(jj != ii)
        {
          _S[count++] = getNodeIdOfCell(i, jj);
        }
      }
      return _S;
    }

/*--------------------------------------------------------------------------*/

public:

    std::string getName() const
    {
      return "FadaMesh::TetrahedralMesh";
    }

    Alat::Vector<TetrahedralMesh::Tetrahedron>& getTetrahedrons()
    {
      return getCells();
    }

    const Alat::Vector<TetrahedralMesh::Tetrahedron>& getTetrahedrons() const
    {
      return getCells();
    }

    const Tetrahedron& getTetrahedron(int i) const
    {
      return getCell(i);
    }

    // void writeVtk(std::string filename) const;
    // void writeBoundaryVtk(std::string filename) const;
  };
}

#endif
