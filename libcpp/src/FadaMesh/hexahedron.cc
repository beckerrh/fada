#include  "FadaMesh/hexahedron.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

Hexahedron::~Hexahedron()
{}

/*--------------------------------------------------------------------------*/

Hexahedron::Hexahedron()
{
  _localnodeid_of_localsideid[0][0] = 0;
  _localnodeid_of_localsideid[0][1] = 1;
  _localnodeid_of_localsideid[0][2] = 2;
  _localnodeid_of_localsideid[0][3] = 3;
  _localnodeid_of_localsideid[1][0] = 0;
  _localnodeid_of_localsideid[1][1] = 4;
  _localnodeid_of_localsideid[1][2] = 5;
  _localnodeid_of_localsideid[1][3] = 1;
  _localnodeid_of_localsideid[2][0] = 0;
  _localnodeid_of_localsideid[2][1] = 3;
  _localnodeid_of_localsideid[2][2] = 7;
  _localnodeid_of_localsideid[2][3] = 4;
  _localnodeid_of_localsideid[3][0] = 1;
  _localnodeid_of_localsideid[3][1] = 5;
  _localnodeid_of_localsideid[3][2] = 6;
  _localnodeid_of_localsideid[3][3] = 2;
  _localnodeid_of_localsideid[4][0] = 3;
  _localnodeid_of_localsideid[4][1] = 2;
  _localnodeid_of_localsideid[4][2] = 6;
  _localnodeid_of_localsideid[4][3] = 7;
  _localnodeid_of_localsideid[5][0] = 4;
  _localnodeid_of_localsideid[5][1] = 7;
  _localnodeid_of_localsideid[5][2] = 6;
  _localnodeid_of_localsideid[5][3] = 5;

  _opposite_sides[0][0] = 0;  _opposite_sides[0][1] = 5;
  _opposite_sides[1][0] = 2;  _opposite_sides[1][1] = 3;
  _opposite_sides[2][0] = 1;  _opposite_sides[2][1] = 4;

  _localsideid_at_localnodeid[0][0]=0;
  _localsideid_at_localnodeid[0][1]=1;
  _localsideid_at_localnodeid[0][2]=2;
  _localsideid_at_localnodeid[1][0]=0;
  _localsideid_at_localnodeid[1][1]=3;
  _localsideid_at_localnodeid[1][2]=1;
  _localsideid_at_localnodeid[2][0]=0;
  _localsideid_at_localnodeid[2][1]=4;
  _localsideid_at_localnodeid[2][2]=3;
  _localsideid_at_localnodeid[3][0]=0;
  _localsideid_at_localnodeid[3][1]=2;
  _localsideid_at_localnodeid[3][2]=4;
  _localsideid_at_localnodeid[4][0]=5;
  _localsideid_at_localnodeid[4][1]=1;
  _localsideid_at_localnodeid[4][2]=2;
  _localsideid_at_localnodeid[5][0]=5;
  _localsideid_at_localnodeid[5][1]=3;
  _localsideid_at_localnodeid[5][2]=1;
  _localsideid_at_localnodeid[6][0]=5;
  _localsideid_at_localnodeid[6][1]=4;
  _localsideid_at_localnodeid[6][2]=3;
  _localsideid_at_localnodeid[7][0]=5;
  _localsideid_at_localnodeid[7][1]=2;
  _localsideid_at_localnodeid[7][2]=4;

  _side_around_direction[0][0]=1;
  _side_around_direction[0][1]=3;
  _side_around_direction[0][2]=4;
  _side_around_direction[0][3]=2;
  _side_around_direction[1][0]=5;
  _side_around_direction[1][1]=1;
  _side_around_direction[1][2]=0;
  _side_around_direction[1][3]=4;
  _side_around_direction[2][0]=2;
  _side_around_direction[2][1]=5;
  _side_around_direction[2][2]=3;
  _side_around_direction[2][3]=0;


  _edge_around_direction[0][0]=5;
  _edge_around_direction[0][1]=6;
  _edge_around_direction[0][2]=7;
  _edge_around_direction[0][3]=4;
  _edge_around_direction[1][0]=10;
  _edge_around_direction[1][1]=8;
  _edge_around_direction[1][2]=0;
  _edge_around_direction[1][3]=2;
  _edge_around_direction[2][0]=11;
  _edge_around_direction[2][1]=9;
  _edge_around_direction[2][2]=1;
  _edge_around_direction[2][3]=3;

  _node_id_in_side[0][0]=0;
  _node_id_in_side[0][1]=1;
  _node_id_in_side[0][2]=2;
  _node_id_in_side[0][3]=3;

  _node_id_in_side[1][0]=0;
  _node_id_in_side[1][4]=1;
  _node_id_in_side[1][5]=2;
  _node_id_in_side[1][1]=3;

  _node_id_in_side[2][0]=0;
  _node_id_in_side[2][3]=1;
  _node_id_in_side[2][7]=2;
  _node_id_in_side[2][4]=3;

  _node_id_in_side[3][1]=0;
  _node_id_in_side[3][5]=1;
  _node_id_in_side[3][6]=2;
  _node_id_in_side[3][2]=3;

  _node_id_in_side[4][3]=0;
  _node_id_in_side[4][2]=1;
  _node_id_in_side[4][6]=2;
  _node_id_in_side[4][7]=3;

  _node_id_in_side[5][4]=0;
  _node_id_in_side[5][7]=1;
  _node_id_in_side[5][6]=2;
  _node_id_in_side[5][5]=3;

}

/*--------------------------------------------------------------------------*/

Hexahedron::Hexahedron( const Hexahedron& hexhadron)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

Hexahedron& Hexahedron::operator=( const Hexahedron& hexhadron)
{
  assert(0);
  return *this;
}
