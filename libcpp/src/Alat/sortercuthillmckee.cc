#include  "Alat/sparsitypattern.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/sortercuthillmckee.h"
#ifdef BOOST
#include  "boost/config.hpp"
#include  "boost/graph/adjacency_list.hpp"
#include  "boost/graph/bandwidth.hpp"
#include  "boost/graph/cuthill_mckee_ordering.hpp"
#include  "boost/graph/properties.hpp"
#endif
#include  <cassert>
#include  <iostream>
#include  <vector>

using namespace Alat;

/*--------------------------------------------------------------------------*/

SorterCuthillMcKee::~SorterCuthillMcKee()
{}

SorterCuthillMcKee::SorterCuthillMcKee() : Alat::SorterInterface()
{}

SorterCuthillMcKee::SorterCuthillMcKee( const SorterCuthillMcKee& sortercuthillmckee) : Alat::SorterInterface(sortercuthillmckee)
{
  assert(0);
}

SorterCuthillMcKee& SorterCuthillMcKee::operator=( const SorterCuthillMcKee& sortercuthillmckee)
{
  Alat::SorterInterface::operator=(sortercuthillmckee);
  assert(0);
  return *this;
}

std::string SorterCuthillMcKee::getName() const
{
  return "SorterCuthillMcKee";
}

SorterCuthillMcKee* SorterCuthillMcKee::clone() const
{
  assert(0);
//return new SorterCuthillMcKee(*this);
}

/*--------------------------------------------------------------------------*/

#ifdef BOOST
void SorterCuthillMcKee::sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const
{
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_color_t, boost::default_color_type, boost::property<boost::vertex_degree_t, int> > > Graph;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef boost::graph_traits<Graph>::vertices_size_type size_type;

  Graph G( sparsitypattern->n() );

  for(int i = 0; i < sparsitypattern->n(); i++)
  {
    for(int posi = sparsitypattern->rowstart(i); posi < sparsitypattern->rowstop(i); posi++)
    {
      int j = sparsitypattern->col(posi);
      if(j == i)
      {
        continue;
      }
      add_edge(i, j, G);
    }
  }

  boost::graph_traits<Graph>::vertex_iterator ui, ui_end;
  boost::property_map<Graph, boost::vertex_degree_t>::type deg = get(boost::vertex_degree, G);
  for(boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
  {
    deg[*ui] = degree(*ui, G);
  }

  boost::property_map<Graph, boost::vertex_index_t>::type index_map = get(boost::vertex_index, G);

  // std::cout<<"original bandwidth: "<<bandwidth(G)<< " ";

  std::vector<Vertex> inv_perm( num_vertices(G) );
  std::vector<size_type> perm( num_vertices(G) );

  {
    //reverse cuthill_mckee_ordering
    cuthill_mckee_ordering( G, inv_perm.rbegin(), get(boost::vertex_color, G), make_degree_map(G) );

    // cout<<"Reverse Cuthill-McKee ordering:"<<endl;
    // cout<<"  ";
    int count = 0;
    for(std::vector<Vertex>::const_iterator i = inv_perm.begin(); i != inv_perm.end(); ++i)
    {
      // cout<<index_map[*i]<<" ";
      p[count++] = index_map[*i];
    }
    // cout<<endl;

    for(size_type c = 0; c != inv_perm.size(); ++c)
    {
      perm[index_map[inv_perm[c]]] = c;
    }
    // std::cout<<"  bandwidth: " <<bandwidth( G, make_iterator_property_map(&perm[0], index_map, perm[0]) )<<std::endl;
  }
}

#else
void SorterCuthillMcKee::sort(Alat::IntVector& p, const Alat::SparsityPattern* sparsitypattern) const
{
  _error_string("sort", "requires boost !");
}

#endif

void SorterCuthillMcKee::sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const
{
  sort( p, matrix->getSparsityPattern() );
}