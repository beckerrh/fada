#ifndef __FadaMesh_Hexahedron_h
#define __FadaMesh_Hexahedron_h

#include  "Alat/fixarray.h"
#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/

namespace FadaMesh
{
  class Hexahedron
  {
protected:
    Alat::FixArray<6, Alat::FixArray<4, int> > _localnodeid_of_localsideid;
    Alat::FixArray<3, Alat::FixArray<2, int> > _opposite_sides;
    Alat::FixArray<8, Alat::FixArray<3, int> > _localsideid_at_localnodeid;
    Alat::FixArray<3, Alat::FixArray<4, int> > _side_around_direction;
    Alat::FixArray<3, Alat::FixArray<4, int> > _edge_around_direction;
    Alat::FixArray<8, Alat::IntMap > _node_id_in_side;

public:
    ~Hexahedron();
    Hexahedron();
    Hexahedron( const Hexahedron& hexhadron);
    Hexahedron& operator=( const Hexahedron& hexhadron);
    std::string getName() const
    {
      return "Hexahedron";
    }

    const Alat::FixArray<4, int>& getLocalNodeIndiceOfSide(int isl) const
    {
      return _localnodeid_of_localsideid[isl];
    }

    int getLocalNodeIndiceOfSide(int ii, int isl) const
    {
      return _localnodeid_of_localsideid[isl][ii];
    }

    int getOppositeSideinDirection(int idir, int iis) const
    {
      return _opposite_sides[idir][iis];
    }

    int getLocalSideAtNode(int iin, int iis) const
    {
      return _localsideid_at_localnodeid[iin][iis];
    }

    int getSidesAroundDirection(int idir, int iis) const
    {
      return _side_around_direction[idir][iis];
    }

    int getEdgeAroundDirection(int idir, int iie) const
    {
      return _edge_around_direction[idir][iie];
    }

    int getNodeIdInSide(int in, int is) const
    {
      return _node_id_in_side[is][in];
    }
  };
}

/*--------------------------------------------------------------------------*/

#endif
