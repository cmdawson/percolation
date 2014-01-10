#include "lattice.h"
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
    const char* vsuffix = " C   0  0  0  0  0  0  0  0  0  0  0  0";
    const char* esuffix = "  1  0  0  0  0";
    format vformat("%1$10.4f%2$10.4f%3$10.4f"); // 3 floats, width=10
    format eformat("%1$3d%2$3d"); // 2 ints, width=3

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
	++v0;
    }
    
    graph_traits<Lattice::AdjacencyList>::edge_iterator e0, e1;
    tie(e0,e1) = edges(H.adjacency_list);
    while (e0 != e1)
    {
	unsigned src = H.adjacency_list[source(*e0,H.adjacency_list)].index;
	unsigned tgt = H.adjacency_list[target(*e0,H.adjacency_list)].index;
	output << eformat % (src+1) % (tgt+1) << esuffix << endl;
	++e0;
    }

    output << "M  END";
    
    return output;
}
