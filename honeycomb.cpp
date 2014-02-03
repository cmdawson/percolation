#include "honeycomb.h"
#include "point3d.h"

using namespace std;
using namespace boost;

const double PI = math::constants::pi<double>();
const double sin_pi_6 = sin(PI/6.0);
const double cos_pi_6 = cos(PI/6.0);
const double dx = 2.0 * cos_pi_6;
const double dy = 1.0 + sin_pi_6;

Honeycomb::Honeycomb(int n) : Lattice(n)
{
    typedef color_traits<default_color_type> Color;
    map<point3d,Vertex> points;
    point3d p0, p1;
    int counter = 0;

    double xmax = 2.0*_n*cos_pi_6;
    double eps = dx*0.01;

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
		points[p0] = add_vertex(adjacency_list);
		adjacency_list[points[p0]].index = counter++;
		adjacency_list[points[p0]].x = p0.x;
		adjacency_list[points[p0]].y = p0.y;
		adjacency_list[points[p0]].color = Color::white();
		if (p0.x < -eps)
		    adjacency_list[points[p0]].traverse = VertexT::NEAR;
		else if (p0.x > xmax-eps)
		    adjacency_list[points[p0]].traverse = VertexT::FAR;
		else
		    adjacency_list[points[p0]].traverse = VertexT::NEITHER;
	    }
	    
	    for (int a=0;a<6;a++)
	    {
		alpha += PI/3.0;
		p1.x = xc + sin(alpha);		
		p1.y = yc + cos(alpha);
		if (points.find(p1) == points.end())
		{
		    points[p1] = add_vertex(adjacency_list);
		    adjacency_list[points[p1]].index = counter++;
		    adjacency_list[points[p1]].x = p1.x;
		    adjacency_list[points[p1]].y = p1.y;
		    adjacency_list[points[p1]].color = Color::white();
		    if (p1.x < -eps)
			adjacency_list[points[p1]].traverse = VertexT::NEAR;
		    else if (p1.x > xmax-eps)
			adjacency_list[points[p1]].traverse = VertexT::FAR;
		    else
			adjacency_list[points[p1]].traverse = VertexT::NEITHER;
		}
		if (!edge(points[p0], points[p1], adjacency_list).second)
		    // TODO edge type?
		    add_edge(points[p0], points[p1], adjacency_list);

		p0 = p1;
	    }
	}
    }
}

void Honeycomb::percolate(const double& p, const double& q)
{
    double EPS = dx * 0.01;
    graph_traits<AdjacencyList>::vertex_iterator v0, v1, va, vj;
    tie(v0, v1) = vertices(adjacency_list);

    // Remove vertices with probability 1-p
    for (va = v0; v0!=v1; v0 = va)
    {
	++va;
	if (random01() > p)
	{
	    clear_vertex(*v0, adjacency_list);
	    remove_vertex(*v0, adjacency_list);
	}
    }
    
    // Remove edges with probability 1-q
    graph_traits<AdjacencyList>::edge_iterator e0, e1, ea;
    tie(e0,e1) = edges(adjacency_list);
    for (ea = e0; e0 != e1; e0 = ea)
    {
	++ea;
	if (abs(adjacency_list[source(*e0,adjacency_list)].y - adjacency_list[target(*e0,adjacency_list)].y) < EPS)
	    continue; // 'vertical' bonds guaranteed
	else if (random01() > q)
	    remove_edge(*e0, adjacency_list);
    }
    
    // Remove orphans and re-index
    int jj = 0;
    tie(v0, v1) = vertices(adjacency_list);
    for (va = v0; v0 != v1; v0 = va)
    {
	++va;
	if (degree(*v0,adjacency_list) == 0)
	    remove_vertex(*v0,adjacency_list);
	else
	    adjacency_list[*v0].index = jj++;
    }	
}
