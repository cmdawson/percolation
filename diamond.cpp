#include "diamond.h"
#include "point3d.h"
#include <vector>
#include <map>


#include <iostream>

using namespace std;
using namespace boost;

Diamond::Diamond(int n) : Lattice(n)
{
    vector<point3d> ucell{
	point3d(0,0,0), point3d(0,2,2), point3d(2,0,2),
	point3d(2,2,0), point3d(3,3,3), point3d(3,1,1),
	point3d(1,3,1), point3d(1,1,3), point3d(4,0,0),
	point3d(0,4,0), point3d(0,0,4), point3d(4,4,0),
	point3d(0,4,4), point3d(4,0,4), point3d(4,4,4),
	point3d(4,2,2), point3d(2,4,2), point3d(2,2,4)
    };

    vector<point3d> edges{
	point3d(1,1,1), point3d(1,1,-1), point3d(1,-1,1), point3d(-1,1,1)
    };


    map<point3d,Vertex> vertex_map;
    int index = 0;

    // Adding a single unit cell:
    for (const auto& p : ucell)
    {
	Vertex v = point3d::add_as_vertex(p, index++, adjacency_list);
	vertex_map[p] = v;
    }

    // (add shifted unit cells, append to map)
    for (const auto& p : vertex_map)
    {
	for (const auto& e : edges)
	{

	    point3d nbr = p.first + e;
	    if (vertex_map.find(nbr) != vertex_map.end())
		add_edge(vertex_map[p.first], vertex_map[nbr], adjacency_list);
	}
    }
}


Diamond::~Diamond() {}

void Diamond::percolate(const double& p, const double& q)
{
    // TODO
}

