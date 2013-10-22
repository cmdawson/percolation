#ifndef _INC_PERC_HONEYCOMB_H
#define _INC_PERC_HONEYCOMB_H

#include <vector>
#include <list>
#include <string>
#include <ostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/random.hpp>
#include "point2d.h"

class Honeycomb
{
private:
	struct VertexT {
		int index;
		double x,y;
		boost::default_color_type color;
	};
	struct EdgeT {
		int group; // no idea why there is the 'maybe used uninitialized' warning.
	};
	//struct edge_type_t {
	//	typedef boost::edge_property_tag kind;
	//};
	
public:
	typedef boost::adjacency_list<
		boost::listS, boost::listS, boost::undirectedS, VertexT, EdgeT> Lattice;
	typedef boost::graph_traits<Lattice>::vertex_descriptor Vertex;
	typedef boost::graph_traits<Lattice>::edge_descriptor Edge;
	
protected:
	typedef boost::graph_traits<Lattice>::vertex_iterator vertex_iter;
	typedef boost::graph_traits<Lattice>::edge_iterator edge_iter;
	typedef boost::graph_traits <Lattice>::vertices_size_type vertices_size_type;
	
	static boost::uniform_01<boost::random::mt19937,double> random01;
	class Visitor;

	int _n;
	Lattice lattice;
	static const double dx;
	static const double dy;
	
public:
	Honeycomb(int n);
	Honeycomb& operator=(const Honeycomb& G);
	~Honeycomb();
	
	void percolate(const double& p, const double& q);
	bool is_crossable(void);
	friend std::ostream &operator<<(std::ostream &output, const Honeycomb &H);
};

#endif
