#include  "Alat/intvector.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/sparsitypattern.h"
#include  "Alat/sortermatrix.h"
#ifdef BOOST
#include  "boost/config.hpp"
#include  "boost/graph/adjacency_list.hpp"
#include  "boost/graph/graphviz.hpp"
#include  "boost/graph/topological_sort.hpp"
#endif
#include  <algorithm>
#include  <cassert>
#include  <iostream>
#include  <iterator>
#include  <list>
#include  <utility>

using namespace Alat;

/*--------------------------------------------------------------------------*/
SorterMatrix::~SorterMatrix()
{}

SorterMatrix::SorterMatrix() : Alat::SorterInterface()
{}

SorterMatrix::SorterMatrix( const SorterMatrix& sortermatrix) : Alat::SorterInterface(sortermatrix)
{
  assert(0);
}

SorterMatrix& SorterMatrix::operator=( const SorterMatrix& sortermatrix)
{
  Alat::SorterInterface::operator=(sortermatrix);
  assert(0);
  return *this;
}

std::string SorterMatrix::getName() const
{
  return "SorterMatrix";
}

SorterMatrix* SorterMatrix::clone() const
{
  assert(0);
//return new SorterMatrix(*this);
}

/*--------------------------------------------------------------------------*/
#ifdef BOOST
// // template<class Graph, class vertex_t>
// bool SorterMatrix::_has_cycle_dfs(const Graph& g, vertex_t u, boost::default_color_type* color) const
// {
//   typename boost::property_map<Graph, boost::vertex_index_t>::type index = get(boost::vertex_index, g);
//   color[u] = boost::gray_color;
//   // std::cerr<<index[u]<<":"<<color[u]<<"\n";
//   typename boost::graph_traits<Graph>::adjacency_iterator vi, vi_end;
//   for(tie(vi, vi_end) = adjacent_vertices(u, g); vi != vi_end; ++vi)
//   {
//     // std::cerr<<index[u]<<"->"<<color[u]<<"\t";
//     if(color[*vi] == boost::white_color)
//     {
//       if( _has_cycle_dfs(g, *vi, color) )
//       {
//         return true;         // cycle detected, return immediately
//       }
//     }
//     else if(color[*vi] == boost::gray_color)      // *vi is an ancestor!
//     {
//       return true;
//     }
//   }
//   color[u] = boost::black_color;
//   // std::cerr<<index[u]<<":"<<color[u]<<"\n";
//   return false;
// }

/*--------------------------------------------------------------------------*/
template <typename G>
struct TopoVisitor : public boost::dfs_visitor<>
{
  // TopoVisitor(Alat::IntVector& p, const G& g) : _p( p.rbegin() )
  TopoVisitor(Alat::IntVector& p, const G& g) : _p( p.begin() )
  {
    id = get(boost::vertex_index, g);
  }

  template <typename Edge, typename Graph>
  void back_edge(const Edge&, Graph&)
  {
    // std::cerr<<"Graph has cycle !!\n";
    // throw not_a_dag();
  }

  template <typename Vertex, typename Graph>
  void finish_vertex(const Vertex& u, Graph&)
  {
    *_p++ = id[u];
  }

  // Alat::IntVector::reverse_iterator _p;
  Alat::IntVector::iterator _p;
  typename boost::property_map<G, boost::vertex_index_t>::type id; // = get(vertex_index, G);
};

/*--------------------------------------------------------------------------*/
void SorterMatrix::sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const
{
  // const Alat::SparsityPattern& sparsitypattern = *matrix->getSparsityPattern();

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::property<boost::vertex_color_t, boost::default_color_type> > Graph;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef boost::graph_traits<Graph>::edge_descriptor Edge;

  Graph G( matrix->getN() );  
    _createGraphUnstructured(G, matrix);    
  boost::property_map<Graph, boost::vertex_index_t>::type id = get(boost::vertex_index, G);
  TopoVisitor<Graph> vis(p, G);
  depth_first_search( G, visitor(vis) );
}

/*--------------------------------------------------------------------------*/
void SorterMatrix::_createGraphUnstructured(Graph& G, const Alat::VariableMatrixInterface* matrix) const
{
  const Alat::SparsityPattern& sparsitypattern = *matrix->getSparsityPattern();
  for(int i = 0; i < sparsitypattern.n(); i++)
  {
    bool found = 0;
    int posi, posj;
    for(posi = sparsitypattern.rowstart(i); posi < sparsitypattern.rowstop(i); posi++)
    {
      int j = sparsitypattern.col(posi);
      if(j == i)
      {
        //continue;
      }
      for(posj = sparsitypattern.rowstart(j); posj < sparsitypattern.rowstop(j); posj++)
      {
        if(sparsitypattern.col(posj) == i)
        {
          if( matrix->greaterThan(posi, posj) )
          {
            if(!edge(j, i, G).second)
            {
              add_edge(i, j, G);
            }
            found = 1;
          }
        }
      }
    }
    if(!found)
    {
      // std::cerr << "*** SorterMatrix::Sort() no edge found for index " << i << "\n";
    }
  }
}

#else
void SorterMatrix::sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const
{
  _error_string("sort", "requires boost !");
}

#endif
