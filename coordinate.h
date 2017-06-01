#ifndef _INC_PERC_COORDINATE_H
#define _INC_PERC_COORDINATE_H

#include "graph.h"
#include <functional>

struct coordinate_hash
{
	std::size_t operator()(const Graph::coordinate& c) const 
    {
		size_t seed = 0;
		for (auto e=c.begin(); e!=c.end(); e++)
		{
    		seed ^= std::hash<double>{}(*e) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		}
		return seed;
    }
};

struct coordinate_equals : std::binary_function<Graph::coordinate, Graph::coordinate, bool>
{
	double epsilon;

	coordinate_equals(double e=1.0E-9) : epsilon(e) {}

    bool operator()(const Graph::coordinate& x, const Graph::coordinate& y) const
    {
        return boost::numeric::ublas::norm_1(x-y) < epsilon;
    }
};

#endif
