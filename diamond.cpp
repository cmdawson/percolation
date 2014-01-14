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


    // Add vertices by translating unit cell
    for (int i=0;i<n;i++)
	for (int j=0;j<n;j++)
	    for (int k=0;k<n;k++)
	    {
		// (add shifted unit cells, append to map)
		point3d tr(4*i,4*j,4*k);
		for (const auto& p : ucell)
		{
		    point3d newp = p+tr;
		    if (vertex_map.find(newp) == vertex_map.end())
			vertex_map[newp] = point3d::add_as_vertex(newp, index++, adjacency_list);
		}
	    }

    // add all edges 
    for (const auto& p : vertex_map)
    {
	Vertex vp = vertex_map[p.first];
	for (const auto& e : edges)
	{
	    point3d nbr = p.first + e;
	    const auto& vn = vertex_map.find(nbr);
	    if (vn != vertex_map.end())
		add_edge(vp, vn->second, adjacency_list);
	}
    }
}


Diamond::~Diamond() {}

void Diamond::percolate(const double& p, const double& q)
{
    // TODO
}
