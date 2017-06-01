#ifndef _INC_PERC_LATTICE_H
#define _INC_PERC_LATTICE_H

#include "unitcell.h"
#include "coordinate.h"
#include <unordered_map>

class Lattice : public Graph
{
private:
	std::vector<vertex_descriptor> _index;
	//std::unordered_map<Graph::coordinate,  

public:
	Lattice(const UnitCell& U, unsigned n);
	


};
#endif
