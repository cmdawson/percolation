#include "diamond.h"
#include "point3d.h"
#include <vector>
#include <map>
#include <iostream>

using namespace std;
using namespace boost;

Diamond::Diamond(int n, int ny, int nz) : Lattice(n, ny, nz)
{
    // cubic unit cell of the diamond 'lattice'
  std:: vector<point3d> ucell{
	point3d(0,0,0), point3d(0,2,2), point3d(2,0,2),
	point3d(2,2,0), point3d(3,3,3), point3d(3,1,1),
	point3d(1,3,1), point3d(1,1,3), point3d(4,0,0),
	point3d(0,4,0), point3d(0,0,4), point3d(4,4,0),
	point3d(0,4,4), point3d(4,0,4), point3d(4,4,4),
	point3d(4,2,2), point3d(2,4,2), point3d(2,2,4)
    };
    // valid edge vectors
  std::vector<point3d> evec{
	point3d(1,1,1), point3d(1,1,-1), point3d(1,-1,1), point3d(-1,1,1)
    };

    map<point3d,Vertex> vertex_map;
    int index = 0;

    // add vertices by translating unit cell
    for (int i=0;i<n;i++)
		for (int j=0;j<ny;j++)
	    for (int k=0;k<nz;k++)
	    {
			point3d tr(4*i,4*j,4*k);
			for (const auto& p : ucell)
			{
		    point3d newp = p+tr;
		    if (vertex_map.find(newp) == vertex_map.end())
		    {//defining which vertices of the diamond are NEAR, FAR and NEITHER for isCrossable()
				vertex_map[newp] = point3d::add_as_vertex(newp, index++, adjacency_list);

				if(n==1)
				{
					if(p.x==0){
											adjacency_list[vertex_map[newp]].traverse= VertexT::NEAR;}
						else if(p.x==4){
							adjacency_list[vertex_map[newp]].traverse= VertexT::FAR;}
					else {adjacency_list[vertex_map[newp]].traverse= VertexT::NEITHER;}
						
				}				
				else if (n>1)
				{
					if(i==0 && p.x==0){
												adjacency_list[vertex_map[newp]].traverse= VertexT::NEAR;}
						else if(i==(n-1) && p.x==4)
							adjacency_list[vertex_map[newp]].traverse=VertexT::FAR;
						else adjacency_list[vertex_map[newp]].traverse=VertexT::NEITHER;
}
				}			 
			}
	    }

    // add edges by searching from each vertex
    for (const auto& p : vertex_map)
    {
	Vertex vp = vertex_map[p.first];
	for (const auto& e : evec)
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
    // Jan 24: percolate and is_crossable() work fine for site and bond percolation in diamond case. 
   vertex_iter v0, v1, va, vj;
	boost::tie(v0,v1) = boost::vertices(adjacency_list);

	int diamond_vertices=0;
	double random_num=1;
    // Remove vertices with probability 1-p
    for (va = v0; v0!=v1; v0 = va)
    {
    	diamond_vertices++;
	++va;
	random_num=random01();
	if (random_num > p)
	{
	    clear_vertex(*v0, adjacency_list);
	    remove_vertex(*v0, adjacency_list);
	}
    }
    
    // Remove edges with probability 1-q
    edge_iter e0, e1, ea;
    tie(e0,e1) = edges(adjacency_list);
    for (ea = e0; e0 != e1; e0 = ea)
    {
		++ea;
	
	if (random01() > q)
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

