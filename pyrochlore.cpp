#include "pyrochlore.h"
#include "point3d.h"

using namespace std;
using namespace boost;

static const double SQRT3 = sqrt(3.0);

// A pyrochlore lattice can be constructed by defining tetrahedra centred on
// each vertex of the 'dual' diamond lattice.
Pyrochlore::Pyrochlore(int n) : Diamond(n)
{
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
    while (d0 != d1)
    {
			Vertex v = *d0++;
		// ignore unconnected vertices 
		if (out_degree(v, diamond_adjacency) == 0)
		    continue;

		VertexT vx = diamond_adjacency[v];
		//cout<<"flag for VertexT vx in pyrochlore is "<<vx.traverse<<endl;
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
				vertex_map[tp] = new_vertex;
				tetra[j] = new_vertex;
				if(diamond_adjacency[v].traverse==VertexT::NEAR)
				{
						if(adjacency_list[new_vertex].x==-0.5)
							adjacency_list[new_vertex].traverse=diamond_adjacency[v].traverse;
						else
							adjacency_list[new_vertex].traverse=VertexT::NEITHER;
				}
				else if(diamond_adjacency[v].traverse==VertexT::FAR)
				{
					if(adjacency_list[new_vertex].x==(4*n+0.5))
						adjacency_list[new_vertex].traverse=diamond_adjacency[v].traverse;
					else adjacency_list[new_vertex].traverse=VertexT::NEITHER;
				}
				else adjacency_list[new_vertex].traverse=VertexT::NEITHER;
				//cout<<adjacency_list[new_vertex].x<<" "<<adjacency_list[new_vertex].y<<" "<<adjacency_list[new_vertex].z<<" "<< adjacency_list[new_vertex].traverse<<endl;
	
	 	   }
	 	   else
				tetra[j] = tv->second;		
		}

	// add all 6 edges of this tetrahedron
		/*for (unsigned j=0;j<4;j++)
			for (unsigned k=j+1;k<4;k++)
					add_edge(tetra[j], tetra[k], adjacency_list);*/
			
    }

    //cout << num_vertices(adjacency_list) << endl;
    //cout << num_edges(adjacency_list) << endl;
}

void Pyrochlore::percolate(const double& p, const double& q)
{
    // TODO
}

