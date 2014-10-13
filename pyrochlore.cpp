#include "pyrochlore.h"
#include "point3d.h"
#include <time.h>
#include <iostream>

using namespace std;
using namespace boost;

static const double SQRT3 = sqrt(3.0);

// A pyrochlore lattice can be constructed by defining tetrahedra centred on
// each vertex of the 'dual' diamond lattice.

Pyrochlore::Pyrochlore(int n, int ny, int nz, double p, bool Firstrun) : Diamond(n, ny, nz)
{
	if (Firstrun == true)
		setSeed(time(0));
	
    // Copy the (already constructed) Diamond into diamond_adjacency
    diamond_adjacency = AdjacencyList();
    copy_graph(adjacency_list, diamond_adjacency, vertex_index_map(get(&VertexT::index, adjacency_list)));
    adjacency_list.clear();
    
    // Edge vectors
    vector<point3d> evec{
	point3d(-1,-1,-1), point3d(1,1,-1), point3d(1,-1,1), point3d(-1,1,1)
    };
    map<point3d,Vertex> vertex_map;
    int index = 0;

    // Create a tetrahedron for each vertex in the diamond
    vertex_iter d0,d1;
    tie(d0,d1) = vertices(diamond_adjacency);
    int diamond_vertices=0;
    
   int counter = 0; //to count total number of qubits in a pyrochlore lattice of size n * ny *nz
    			//counter is not output anywhere in this version, but a simple cout at the end of pyrochlore constructor will output the 
			//total number of qubits. We didn't cout it because we don't want 100 couts for each sample iteration!
	size=0;
    while (d0 != d1)
    {
    	diamond_vertices++;
			Vertex v = *d0++;
		// ignore unconnected vertices 
		if (out_degree(v, diamond_adjacency) == 0)
		    continue;

		VertexT vx = diamond_adjacency[v];
		Vertex tetra[4];
		point3d P(vx.x, vx.y, vx.z);
		double lambda = int(P.x+P.y+P.z)%4==0 ? 0.5 : -0.5;

		// find all 4 neighbours in a general diamond lattice, define tetrahedron
		// vertex to be halfway along the corresponding edge.
		for (unsigned j=0;j<4;j++)
		{
		    point3d tp = P + lambda*evec[j];
		    const auto& tv = vertex_map.find(tp);
		    if (tv == vertex_map.end())
		    {
				Vertex new_vertex = point3d::add_as_vertex(tp, index++, adjacency_list);//Jan 22: defining the traversal flags for pyrochlore vertices
				
				counter++;
				size++;			
				vertex_map[tp] = new_vertex;				
				tetra[j] = new_vertex;
				if(diamond_adjacency[v].traverse==VertexT::NEAR)
				{
						if(adjacency_list[new_vertex].x==-0.5)
					adjacency_list[new_vertex].traverse=diamond_adjacency[v].traverse;
					else adjacency_list[new_vertex].traverse=VertexT::NEITHER;
				}
				else if(diamond_adjacency[v].traverse==VertexT::FAR)
				{
					if(adjacency_list[new_vertex].x==(4*n+0.5))
						adjacency_list[new_vertex].traverse=diamond_adjacency[v].traverse;
					else adjacency_list[new_vertex].traverse=VertexT::NEITHER;
				}
				else adjacency_list[new_vertex].traverse=VertexT::NEITHER;

	 	   }
	 	   else
			tetra[j] = tv->second;		
		}

//add the six edges of a tetrahedron if we are above the success probability of a fusion gate, which for us is 75%. For failure events (25%), only add two disconnected edges
	
	double random_num=random01();
		if (random_num<=p)
		{
	// add all 6 edges of this tetrahedron
		for (unsigned j=0;j<4;j++)
	    for (unsigned k=j+1;k<4;k++)
			{		add_edge(tetra[j], tetra[k], adjacency_list);
			}
		}
		else
		{
			add_edge(tetra[0], tetra[1], adjacency_list);
			add_edge(tetra[2], tetra[3], adjacency_list);
				
		}
	
    }

    }

void Pyrochlore::percolate(const double& p, const double& q)
{
//implement the second step of our percolation protocol, which is to fuse tetrahedra with each other 56% of the time, i.e. clear a vertex of all bonds with a probability of 44%
 
   vertex_iter v0, v1, va, vj;
	boost::tie(v0,v1) = boost::vertices(adjacency_list);

	int vertex_num=0;
	 
	double random_num=1;
    // Remove vertices with probability 1-p
    for (va = v0; v0!=v1; v0 = va)
    {
    	vertex_num++;
		++va;
		random_num=random01();
		if (random_num > p)
		{
			if(adjacency_list[*v0].traverse==VertexT::NEITHER)//only inner vertices are cleared upon failure, since they are the ones that go through the fusion process
			{
	    	clear_vertex(*v0, adjacency_list);
	    	remove_vertex(*v0, adjacency_list);
	 		}	
		}
    }
    
    int edge_num=0;
    // Remove edges with probability 1-q
	//Our pyrochlore proposal does not need to remove edges, since q = 1 for us. We have left the following code in for reference.
 
    edge_iter e0, e1, ea;
    tie(e0,e1) = edges(adjacency_list);
    for (ea = e0; e0 != e1; e0 = ea)
    {
    	edge_num++;
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

