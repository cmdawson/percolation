#ifndef _INC_PERC_LATTICE_H
#define _INC_PERC_LATTICE_H

#include <vector>
#include <list>
#include <string>
#include <ostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/random.hpp>
#include <iostream>

using namespace std;
class Lattice
{
protected:
    struct VertexT {
	int index;
	double x,y,z;
	boost::default_color_type color;
	enum TraverseFlag {NEITHER,NEAR,FAR} traverse;
	//TraverseFlag: Middle/Neither, Near, Far
    };
    struct EdgeT {
	int group; // no idea why there is the 'maybe used uninitialized' warning.
    };

	
public:
    typedef boost::adjacency_list<
	boost::listS, boost::listS, boost::undirectedS, VertexT, EdgeT> AdjacencyList;
    typedef boost::graph_traits<AdjacencyList>::vertex_descriptor Vertex;
    typedef boost::graph_traits<AdjacencyList>::edge_descriptor Edge;
    
protected:
    typedef boost::graph_traits<AdjacencyList>::vertex_iterator vertex_iter;
    typedef boost::graph_traits<AdjacencyList>::edge_iterator edge_iter;
    typedef boost::graph_traits <AdjacencyList>::vertices_size_type vertices_size_type;

    static boost::uniform_01<boost::random::mt19937,double> random01;

    int _n, _ny, _nz;
    AdjacencyList adjacency_list;

    class Visitor : public boost::default_dfs_visitor
    {
    public:
	bool* crossed;
	int* size;
	Visitor(bool* cc, int* s) : crossed(cc), size(s) {}
	void discover_vertex(Vertex v, const AdjacencyList& g) 
	{
		*size=*size+1; //this keeps a running count of all the vertices in a connected cluster. 
	    	if (g[v].traverse == VertexT::FAR)
		{
		*crossed = true;
		}
	}
    };
    
public:
    Lattice(int n, int ny, int nz) : _n(n), _ny(ny), _nz(nz)
    {
	if (_n < 1)
	    throw std::invalid_argument("Lattice must be at least 1x1");
		
	adjacency_list = AdjacencyList();
    }

    Lattice& operator=(const Lattice& G)
    {
	if (&G == this) 
		return *this; 

	_n = G._n;
	_ny = G._ny;
	_nz = G._nz;
	copy_graph(G.adjacency_list, adjacency_list,
	   vertex_index_map(get(&VertexT::index, adjacency_list)));
	return *this;
    }

    ~Lattice() {}

    void setSeed(unsigned u)
    {
	boost::random::mt19937 gen(u);
	random01 = boost::random::uniform_01<boost::random::mt19937,double>(gen);

    }

    bool is_crossable(double* Fsize)
    {
	// The (percolated) lattice is said to be crossable if there is
	// a connected component which includes at least one vertex on one 
	// 'boundary' (labelled NEAR) and at least one on the 'FAR' boundary
	std::vector<Vertex> start_vertices;
	start_vertices.reserve(_n);
	typedef boost::color_traits<boost::default_color_type> Color;
	//boost::graph_traits<AdjacencyList>::vertex_iterator v0, v1;
	vertex_iter v0, v1;
	boost::tie(v0,v1) = boost::vertices(adjacency_list);

	while(v0 != v1)
	{
	    if (adjacency_list[*v0].traverse == VertexT::NEAR)
		start_vertices.push_back(*v0);
	    adjacency_list[*v0].color = Color::white();
	    ++v0;
	}
	for (unsigned jj=0;jj<start_vertices.size();jj++)
	{
	    if (adjacency_list[start_vertices[jj]].color == Color::black())
		    continue;   // already visited this one
		    
	    bool xing = false;
	    int ClusterSize=0;
	    *Fsize=0;//Fsize is a pointer to the variable tempsize in SAMPLE loop in test.cpp
	    Visitor xvis(&xing, &ClusterSize);
	    boost::depth_first_visit(adjacency_list, start_vertices[jj], xvis, get(&VertexT::color, adjacency_list));
	    
	    if (xing)
		{
		*Fsize=ClusterSize;
		//cout<<"Cluster size upon success is "<<*Fsize<<endl; //only care about Fsize variable if xing is true. Since start vertices are not 
								//sampled after a successful
								//crossing is found, ClusterSize only gives the size of the one of the possible trees that reach the FAR end of a lattice. It may not matter
								//for us though since we are above percolation and expect the size of the connected 
								//cluster to go to infinity. Plus, we sample 100 times or more.
	
		return true;
		}
	}
	return false;
    }

    virtual void percolate(const double& p, const double& q) = 0;

    friend std::ostream &operator<<(std::ostream &output, const Lattice &H);
};

#endif
