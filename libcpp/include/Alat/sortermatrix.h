#ifndef __Alat_SorterMatrix_h
#define __Alat_SorterMatrix_h

#include  "sorterinterface.h"
#ifdef BOOST
#include  "boost/graph/properties.hpp"
#include  "boost/config.hpp"
#include  "boost/graph/adjacency_list.hpp"
#include  "boost/graph/graphviz.hpp"
#include  "boost/graph/topological_sort.hpp"
#endif

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
  class SystemMatrixInterface;
}

namespace Alat
{
  class SorterMatrix : public Alat::SorterInterface
  {
protected:
#ifdef BOOST
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::property<boost::vertex_color_t, boost::default_color_type> > Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;

    // // template<class Graph, class vertex_t>
    // bool _has_cycle_dfs(const Graph& g, vertex_t u, boost::default_color_type* color) const;

    void _createGraphUnstructured(Graph& G, const Alat::VariableMatrixInterface* matrix) const;
 #endif

public:
    ~SorterMatrix();
    SorterMatrix();
    SorterMatrix( const SorterMatrix& sortermatrix);
    SorterMatrix& operator=( const SorterMatrix& sortermatrix);
    std::string getName() const;
    SorterMatrix* clone() const;

    void sort(Alat::IntVector& p, const Alat::VariableMatrixInterface* matrix) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
