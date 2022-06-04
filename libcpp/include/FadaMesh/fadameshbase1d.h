#ifndef __FadaMesh_FadaMeshBase1d_h
#define __FadaMesh_FadaMeshBase1d_h

#include  "fadameshbase.h"

/*---------------------------------------------------------*/

namespace FadaMesh
{
  template<int NODESPERCELL, int SIDESPERCELL, int NODESPERSIDE>
  class FadaMeshBase1d : public FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>
  {
public:

private:

public:

    FadaMeshBase1d<NODESPERCELL, SIDESPERCELL, NODESPERSIDE>() : FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>() {}

    std::string getName() const
    {
      return "FadaMesh::FadaMeshBase1d";
    }

    const Alat::Node& getNode(int i) const
    {
      return FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::_nodes[i];
    }

    const Alat::Node& getNodeOfSide(int is, int ii) const
    {
      int i =  FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeIdOfSide(is, ii);
      return getNode(i);
    }

    const Alat::Node& getNodeOfCell(int iK, int ii) const
    {
      int i =  FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNodeIdOfCell(iK, ii);
      assert(i >= 0);
      assert( i < getNNodes() );
      return getNode(i);
    }

    Alat::Node getNodeOfCell(int iK) const;

    int getNNodes() const
    {
      return FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNNodes();
    }

    int getNCells() const
    {
      return FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNCells();
    }

    int getNSides() const
    {
      return FadaMeshBase<1, NODESPERCELL, SIDESPERCELL, NODESPERSIDE>::getNSides();
    }

    // int  getNNodesPerSide() {return NODESPERSIDE;}
  };
}

/*---------------------------------------------------------*/

#endif
