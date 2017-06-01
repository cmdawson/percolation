#ifndef _INC_PERC_GRAPH_H
#define _INC_PERC_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/graph/copy.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <vector>

class Graph
{
public:
	typedef boost::numeric::ublas::vector<double, std::vector<double>> coordinate;

    struct Vertex
	{
		int id;
		coordinate x;
		boost::default_color_type color;
    };

    struct Edge
	{
		boost::default_color_type color;
		int group = 0; // 'maybe used uninitialized' warning.
    };
	
    typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, Vertex, Edge> AdjacencyList;
    typedef boost::graph_traits<AdjacencyList>::vertex_descriptor vertex_descriptor;
    typedef boost::graph_traits<AdjacencyList>::edge_descriptor edge_descriptor;
    typedef boost::graph_traits<AdjacencyList>::vertex_iterator vertex_iter;
    typedef boost::graph_traits<AdjacencyList>::edge_iterator edge_iter;
    typedef boost::graph_traits<AdjacencyList>::vertices_size_type vertices_size_t;
    typedef boost::graph_traits<AdjacencyList>::edges_size_type edges_size_t;

    //static boost::uniform_01<boost::random::mt19937,double> random01;

protected:
    AdjacencyList _adjacency_list;

	void write_to_ptree(boost::property_tree::ptree& ptree) const;
	void read_from_ptree(const boost::property_tree::ptree& ptree);

public:

    Graph() 
    {
		_adjacency_list = AdjacencyList();
    }

    Graph& operator=(const Graph& G)
    {
		if (&G == this) 
			return *this; 

		boost::copy_graph(G._adjacency_list, _adjacency_list, vertex_index_map(get(&Vertex::id, _adjacency_list)));

		return *this;
    }

    ~Graph() {}

	AdjacencyList& adjacency_list(void)
	{
		return _adjacency_list;
	}

    friend std::ostream& operator<<(std::ostream &output, const Graph &G);
	friend std::istream& operator>>(std::istream &input, Graph &G);
};

#endif
