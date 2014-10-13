#include "lattice.h"
#include <map>
#include <stdexcept>

#include <boost/format.hpp>

using namespace std;
using namespace boost;


random::uniform_01<random::mt19937,double> Lattice::random01(random::mt19937(42));

/* Output operator
 * Produces a molfile that can be read by, for example, ChemDoodle
 * See http://en.wikipedia.org/wiki/Chemical_table_file for specification or lack
 * thereof. It appears to be a fixed width format, but the specs are so arcane
 * I've had to just verbatim copy the format of examples */

ostream &operator<<(ostream &output, const Lattice& H)
{
    map<Lattice::Vertex, unsigned> oidx;
    unsigned ii = 0;
    format vformat("{\"x\":%1$.2f,\"y\":%2$.2f,\"z\":%3$.2f}"); 

    output << "{\"vert\":[";

    Lattice::vertex_iter v0, v1;
    tie(v0,v1) = vertices(H.adjacency_list);

    if (v0 != v1)
    {
	output << vformat % H.adjacency_list[*v0].x % H.adjacency_list[*v0].y 
		    % H.adjacency_list[*v0].z;
	oidx[*v0] = ii++;
    }
    while (++v0 != v1)
    {
	output << "," << vformat % H.adjacency_list[*v0].x
	    % H.adjacency_list[*v0].y % H.adjacency_list[*v0].z;
	oidx[*v0] = ii++;
    }

    output << "],\"edge\":[";

    graph_traits<Lattice::AdjacencyList>::edge_iterator e0, e1;
    tie(e0,e1) = edges(H.adjacency_list);

    if (e0 != e1)
    {
	unsigned src = oidx[source(*e0,H.adjacency_list)];
	unsigned tgt = oidx[target(*e0,H.adjacency_list)];
	output << "[" << src << "," << tgt << "]";
    }
    while (++e0 != e1)
    {
	unsigned src = oidx[source(*e0,H.adjacency_list)];
	unsigned tgt = oidx[target(*e0,H.adjacency_list)];
	output << ",[" << src << "," << tgt << "]";
    }
    output << "]}";

    return output;

    /*  MDL mol format version
    const char* vsuffix = " C   0  0  0  0  0  0  0  0  0  0  0  0";
    const char* esuffix = "  1  0  0  0  0";
    format vformat("%1$10.4f%2$10.4f%3$10.4f"); // 3 floats, width=10
    format eformat("%1$3d%2$3d"); // 2 ints, width=3
    map<Lattice::Vertex, unsigned> oidx;
    unsigned ii = 0;

    if (num_vertices(H.adjacency_list) > 999)
	throw runtime_error("MOL files can only cope with <999 vertices");

    output << "percolatedlattice" << endl;
    output << " PERC/whatever101" << endl << endl;
    output << eformat % num_vertices(H.adjacency_list) 
		% num_edges(H.adjacency_list) 
	<< "  0  0  0  0  0  0  0  0  1 V2000" << endl;

    Lattice::vertex_iter v0, v1;
    tie(v0,v1) = vertices(H.adjacency_list);
    while (v0 != v1)
    {
	output << vformat % H.adjacency_list[*v0].x 
		    % H.adjacency_list[*v0].y 
		    % H.adjacency_list[*v0].z 
	    << vsuffix << endl;
	oidx[*v0] = ii++;
	++v0;
    }
    
    graph_traits<Lattice::AdjacencyList>::edge_iterator e0, e1;
    tie(e0,e1) = edges(H.adjacency_list);
    while (e0 != e1)
    {
	//unsigned src = H.adjacency_list[source(*e0,H.adjacency_list)].index;
	//unsigned tgt = H.adjacency_list[target(*e0,H.adjacency_list)].index;
	unsigned src = oidx[source(*e0,H.adjacency_list)] + 1;
	unsigned tgt = oidx[target(*e0,H.adjacency_list)] + 1;
	output << eformat % src % tgt << esuffix << endl;
	++e0;
    }

    output << "M  END";
    
    return output; */
}
