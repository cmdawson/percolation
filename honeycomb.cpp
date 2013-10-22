#include "honeycomb.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <map>
#include <boost/config.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/copy.hpp>

using namespace std;
using namespace boost;

random::uniform_01<random::mt19937,double> Honeycomb::random01(random::mt19937(42));

const double PI = math::constants::pi<double>();
const double sin_pi_6 = sin(PI/6.0);
const double cos_pi_6 = cos(PI/6.0);

const double Honeycomb::dx = 2.0 * cos_pi_6;
const double Honeycomb::dy = 1.0 + sin_pi_6;

class Honeycomb::Visitor : public default_dfs_visitor
{
public:
	bool* crossed;
	double xmax;
	Visitor(bool* cc, const double& xm) : crossed(cc), xmax(xm) {}
	void discover_vertex(Vertex v, const Lattice& g) 
	{
		if (g[v].x > xmax)
			*crossed = true;
	}
};

Honeycomb::Honeycomb(int n) : _n(n)
{
	if (_n < 1)
		throw invalid_argument("Honeycomb must be at least 1x1");
		
	lattice = Lattice();
	typedef color_traits<default_color_type> Color;
	map<point2d,Vertex> points;
	point2d p0, p1;
	int counter = 0;
	
	for (int r=0;r<_n;r++)
	{
		double yc = r*dy, x0 = (r%2) ? cos_pi_6 : 0.0;
		for (int c=0;c<_n;c++)
		{
			double xc = x0 + c*dx;
			double alpha = 5.0*PI/3.0;
			p0.x = xc + sin(alpha);
			p0.y = yc + cos(alpha);	
			if (points.find(p0) == points.end())
			{
				points[p0] = add_vertex(lattice);
				lattice[points[p0]].index = counter++;
				lattice[points[p0]].x = p0.x;
				lattice[points[p0]].y = p0.y;
				lattice[points[p0]].color = Color::white();
			}
			
			for (int a=0;a<6;a++)
			{
				alpha += PI/3.0;
				p1.x = xc + sin(alpha);		
				p1.y = yc + cos(alpha);
				if (points.find(p1) == points.end())
				{
					points[p1] = add_vertex(lattice);
					lattice[points[p1]].index = counter++;
					lattice[points[p1]].x = p1.x;
					lattice[points[p1]].y = p1.y;
					lattice[points[p0]].color = Color::white();
				}
				if (!edge(points[p0], points[p1], lattice).second)
					add_edge(points[p0], points[p1], lattice); // TODO edge type?
				p0 = p1;
			}
		}
	}
}

Honeycomb& Honeycomb::operator=(const Honeycomb& G)
{
	if (&G == this) 
		return *this; 

	_n = G._n;
	copy_graph(G.lattice, lattice, vertex_index_map(get(&VertexT::index, lattice)));
	return *this;
}
 
Honeycomb::~Honeycomb() {}

void Honeycomb::percolate(const double&p, const double& q)
{
	double EPS = dx * 0.01;
	graph_traits<Lattice>::vertex_iterator v0, v1, va, vj;
	tie(v0, v1) = vertices(lattice);

	// Remove vertices with probability 1-p
	for (va = v0; v0!=v1; v0 = va)
	{
		++va;
		if (random01() > p)
		{
			clear_vertex(*v0, lattice);
			remove_vertex(*v0, lattice);
		}
	}
	
	// Remove edges with probability 1-q
	graph_traits<Lattice>::edge_iterator e0, e1, ea;
	tie(e0,e1) = edges(lattice);
	for (ea = e0; e0 != e1; e0 = ea)
	{
		++ea;
		if (abs(lattice[source(*e0,lattice)].y - lattice[target(*e0,lattice)].y) < EPS)
			continue; // 'vertical' bonds guaranteed
		else if (random01() > q)
			remove_edge(*e0, lattice);
	}
	
	// Remove orphans and re-index
	int jj = 0;
	tie(v0, v1) = vertices(lattice);
	for (va = v0; v0 != v1; v0 = va)
	{
		++va;
		if (degree(*v0,lattice) == 0)
			remove_vertex(*v0,lattice);
		else
			lattice[*v0].index = jj++;
	}	
}

bool Honeycomb::is_crossable(void)
{
	// The (percolated) 2d honeycomb is said to be crossable if there is a connected
	// compenent which includes at least one vertex on the left boundary (x=0) and at
	// least one on the right boundary (x=xmax, see below)	
	vector<Vertex> left_vertices;
	left_vertices.reserve(_n);
	typedef color_traits<default_color_type> Color;
	graph_traits<Lattice>::vertex_iterator v0, v1;
	tie(v0,v1) = vertices(lattice);
	double xmax = 2.0 * _n * cos_pi_6;
	double EPS = dx * 0.01;

	while(v0 != v1)
	{
		if (lattice[*v0].x < EPS)
			left_vertices.push_back(*v0);
		lattice[*v0].color = Color::white();
		++v0;
	}
	
	for (unsigned jj=0;jj<left_vertices.size();jj++)
	{
		if (lattice[left_vertices[jj]].color == Color::black())
			continue;
			
		bool xing = false;
		Visitor xvis(&xing, xmax-EPS);
		depth_first_visit(lattice, left_vertices[jj], xvis, get(&VertexT::color, lattice));
		
		if (xing)
			return true;
	}
	return false;
}

ostream &operator<<(ostream &output, const Honeycomb &H)
{
	Honeycomb::vertex_iter v0, v1;
	
	tie(v0,v1) = vertices(H.lattice);
	while (v0 != v1)
	{
		Honeycomb::Vertex vv = *v0;
		output << H.lattice[vv].index << "," 
			<< H.lattice[vv].color << ","
			<< H.lattice[vv].x << "," << H.lattice[vv].y;
		Honeycomb::Lattice::adjacency_iterator n0, n1;
		tie(n0,n1) = adjacent_vertices(*v0, H.lattice);
		while (n0 != n1)
		{
			if (H.lattice[vv].index < H.lattice[*n0].index)
				output << "," <<  H.lattice[*n0].index;
			n0++;
		}
		output << endl;
		++v0;
	}
	return output;
}
